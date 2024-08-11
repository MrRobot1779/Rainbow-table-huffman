#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha3.h"        // Inclure le fichier de définition SHA-3
#include "reduce_hash.h" // Inclure la définition de reduce_hash

#define LONGUEUR_MOT_DE_PASSE 5 // Longueur des mots de passe
#define NOMBRE_ENTREES 10       // Nombre de mots de passe
#define NIVEAUX 5               // Nombre de niveaux

typedef struct
{
    char debut[LONGUEUR_MOT_DE_PASSE + 1];
    char fin[LONGUEUR_MOT_DE_PASSE + 1];
} EntreeTable;

// Fonction pour calculer le hash SHA-3 d'un mot de passe
void sha3_256(const char *mot_de_passe, uint8_t *hash)
{
    sha3_context c;
    sha3_Init256(&c);
    sha3_Update(&c, mot_de_passe, strlen(mot_de_passe));
    memcpy(hash, sha3_Finalize(&c), 32);
}

// Fonction pour convertir un tableau d'octets en chaîne hexadécimale
void bin_to_hex(const uint8_t *bin, size_t len, char *out)
{
    for (size_t i = 0; i < len; i++)
    {
        sprintf(out + 2 * i, "%02x", bin[i]);
    }
}

// Fonction pour rechercher un mot de passe dans la table arc-en-ciel
int rechercher_dans_table_arc_en_ciel(const EntreeTable table[NOMBRE_ENTREES], const uint8_t hash_cible[32], char *resultat)
{
    char K[LONGUEUR_MOT_DE_PASSE + 1];
    uint8_t hash[32];
    char hash_hex[65];
    int i, j, k;

    // Boucle de transformation
    for (i = 0; i < NIVEAUX; ++i)
    {
        // Copier le hash cible dans une variable temporaire
        memcpy(hash, hash_cible, 32);

        // Transformer K itérativement
        for (j = 0; j < i; ++j)
        {
            reduce_hash(hash, LONGUEUR_MOT_DE_PASSE, K);
            sha3_256(K, hash);
            bin_to_hex(hash, 32, hash_hex);
            printf("Niveau %d, Hachage intermédiaire %d: %s, Réduction: %s\n", i, j, hash_hex, K);
        }

        // Réduire le dernier hash pour obtenir K
        reduce_hash(hash, LONGUEUR_MOT_DE_PASSE, K);
        printf("Niveau %d, K final: %s\n", i, K);

        // Rechercher K dans la table
        for (j = 0; j < NOMBRE_ENTREES; ++j)
        {
            if (strcmp(table[j].fin, K) == 0)
            {
                // Trouvé, initialiser resultat avec le mot de passe de début correspondant
                strcpy(resultat, table[j].debut);
                printf("Trouvé dans la table : %s\n", resultat);

                // Appliquer la chaîne de transformation inverse pour obtenir le mot de passe initial
                for (k = 0; k < NIVEAUX - 1 - i; ++k)
                {
                    sha3_256(resultat, hash);
                    reduce_hash(hash, LONGUEUR_MOT_DE_PASSE, resultat);
                    bin_to_hex(hash, 32, hash_hex);
                    printf("Chaîne de transformation inverse %d: Hash: %s, Réduction: %s\n", k, hash_hex, resultat);
                }
                return 1;
            }
        }

        // Si non trouvé, transformer K pour la prochaine itération
        sha3_256(K, hash);
    }

    // Si non trouvé après toutes les itérations
    return 0;
}

int main()
{
    // Charger la table de la table arc-en-ciel depuis le fichier
    EntreeTable table[NOMBRE_ENTREES];
    FILE *fichier = fopen("table_arc_en_ciel.txt", "r");
    if (fichier == NULL)
    {
        perror("Impossible d'ouvrir le fichier");
        exit(1);
    }

    // Lire les mots de passe initiaux et finaux dans la table arc-en-ciel
    for (int i = 0; i < NOMBRE_ENTREES; ++i)
    {
        fscanf(fichier, "Début : %s, Fin : %s\n", table[i].debut, table[i].fin);
    }

    fclose(fichier);

    // Hash cible pour la recherche, généré à partir de "UWFNq"
    uint8_t hash_cible[32];
    sha3_256("h1F4G", hash_cible);

    char resultat[LONGUEUR_MOT_DE_PASSE + 1];
    if (rechercher_dans_table_arc_en_ciel(table, hash_cible, resultat))
    {
        printf("Mot de passe trouvé : %s\n", resultat);
    }
    else
    {
        printf("Mot de passe non trouvé dans la table arc-en-ciel.\n");
    }

    return 0;
}
