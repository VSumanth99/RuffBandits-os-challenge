#ifndef SHA_HASH_H
#define SHA_HASH_H

#include <openssl/sha.h>
#include "byte_utils.h"
#include <stdbool.h>
#include <semaphore.h>
#include "network.h"

int is_hash_equal(uint64_t number, uint8_t check_hash[32]);

#endif