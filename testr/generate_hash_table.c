#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "sha3.h"
#include "reduce_hash.h"

#define LONGUEUR_MOT_DE_PASSE 5 // Longueur des mots de passe
#define NOMBRE_ENTREES 10       // Nombre de mots de passe
#define NIVEAUX 5               // Nombre de niveaux

typedef struct
{
    char debut[LONGUEUR_MOT_DE_PASSE + 1];
    char fin[LONGUEUR_MOT_DE_PASSE + 1];
} EntreeTable;

// Fonction pour générer un mot de passe aléatoire de longueur LONGUEUR_MOT_DE_PASSE
void generer_mot_de_passe_aleatoire(char *mot_de_passe, size_t longueur)
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (size_t i = 0; i < longueur; i++)
    {
        int key = rand() % (int)(sizeof(charset) - 1);
        mot_de_passe[i] = charset[key];
    }
    mot_de_passe[longueur] = '\0';
}

// Fonction pour calculer le hash SHA-3 d'un mot de passe
void sha3_256(const char *mot_de_passe, uint8_t *hash)
{
    sha3_context c;
    sha3_Init256(&c);
    sha3_Update(&c, mot_de_passe, strlen(mot_de_passe));
    memcpy(hash, sha3_Finalize(&c), 32);
}

// Fonction pour générer la table arc-en-ciel
void generer_table_arc_en_ciel(EntreeTable table[NOMBRE_ENTREES])
{
    char reduit[LONGUEUR_MOT_DE_PASSE + 1]; // Niveau de réduction
    uint8_t hash[32];                       // Stocker le hash SHA-3

    // Générer des mots de passe aléatoires de longueur LONGUEUR_MOT_DE_PASSE et les stocker dans la table arc-en-ciel
    for (int i = 0; i < NOMBRE_ENTREES; ++i)
    {
        generer_mot_de_passe_aleatoire(table[i].debut, LONGUEUR_MOT_DE_PASSE);
        strcpy(table[i].fin, table[i].debut); // Initialement, la valeur est le même mot de passe

        // Calculer la chaîne de réduction jusqu'au dernier niveau
        for (int niveau = 0; niveau < NIVEAUX; ++niveau)
        {
            sha3_256(table[i].fin, hash);                     // Calculer le hash SHA-3
            reduce_hash(hash, LONGUEUR_MOT_DE_PASSE, reduit); // Réduire le hash

            // Mettre à jour les mots de passe pour le niveau suivant
            strcpy(table[i].fin, reduit);
        }
    }
}

int main()
{
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

    // Créer la table arc-en-ciel
    EntreeTable table[NOMBRE_ENTREES];
    generer_table_arc_en_ciel(table);

    // Sauvegarder la table arc-en-ciel dans un fichier
    FILE *fichier = fopen("table_arc_en_ciel.txt", "w");
    if (fichier == NULL)
    {
        perror("Impossible d'ouvrir le fichier");
        exit(1);
    }

    for (int i = 0; i < NOMBRE_ENTREES; ++i)
    {
        fprintf(fichier, "Début : %s, Fin : %s\n", table[i].debut, table[i].fin);
    }

    fclose(fichier);

    printf("Table arc-en-ciel générée\n");

    return 0;
}
