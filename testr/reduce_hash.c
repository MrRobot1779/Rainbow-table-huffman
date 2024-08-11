#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "reduce_hash.h"

// Fonction pour convertir une chaîne hexadécimale en tableau d'octets
void hex_to_bin(const char *hex, uint8_t *bin)
{
    for (int i = 0; i < 32; ++i)
    {
        sscanf(hex + 2 * i, "%2hhx", &bin[i]);
    }
}

void reduce_hash(const uint8_t bin_hash[32], size_t n, char *result)
{
    size_t block_size = 8 * n;
    size_t num_blocks = 256 / block_size;
    uint8_t blocks[num_blocks][n];

    memset(blocks, 0, sizeof(blocks));

    size_t p = 0;
    for (size_t i = 0; i < num_blocks; ++i)
    {
        memcpy(blocks[i], bin_hash + p, n);
        p = p + n;
    }

    uint8_t reduced[n];
    memset(reduced, 0, sizeof(reduced));

    for (size_t i = 0; i < num_blocks; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            reduced[j] ^= blocks[i][j];
        }
    }

    for (size_t i = 0; i < n; ++i)
    {
        int val = reduced[i] % 62;
        if (val < 10)
        {
            result[i] = '0' + val; // 0-9
        }
        else if (val < 36)
        {
            result[i] = 'A' + (val - 10); // A-Z
        }
        else
        {
            result[i] = 'a' + (val - 36); // a-z
        }
    }
    result[n] = '\0';
}

int main()
{
    const char *hash_hex = "e2eaff317cb7b8b6ab62af4da3ad68ac77c9727db26d3e49e3f5d93e8e8c5d6d";
    uint8_t hash_bin[32];
    hex_to_bin(hash_hex, hash_bin);

    int n = 5; // Exemple avec 5 caractères
    char result[n + 1];

    reduce_hash(hash_bin, n, result);
    printf("Reduced hash: %s\n", result);

    return 0;
}
