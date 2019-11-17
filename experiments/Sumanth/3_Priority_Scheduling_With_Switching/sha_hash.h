#ifndef SHA_HASH_H
#define SHA_HASH_H

#include <openssl/sha.h>
#include "byte_utils.h"

int is_hash_equal(uint64_t number, uint8_t check_hash[32]);

#endif
