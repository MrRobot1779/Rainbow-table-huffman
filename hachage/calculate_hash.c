#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sha3.h"

void print_hash(const uint8_t *hash, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void sha3_256(const char *input, uint8_t *hash)
{
    sha3_context c;
    sha3_Init256(&c);
    sha3_Update(&c, input, strlen(input));
    memcpy(hash, sha3_Finalize(&c), 32);
}

int main()
{
    const char *password = "I54US";
    uint8_t hash[32];

    sha3_256(password, hash);
    printf("SHA-3 256 hash of '%s': ", password);
    print_hash(hash, 32);

    return 0;
}
