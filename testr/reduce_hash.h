#ifndef REDUCE_HASH_H
#define REDUCE_HASH_H

#include <stddef.h>
#include <stdint.h>

void hex_to_bin(const char *hex, uint8_t *bin);
void reduce_hash(const uint8_t bin_hash[32], size_t n, char *result);

#endif // REDUCE_HASH_H
