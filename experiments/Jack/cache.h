#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <stdio.h>

#define CACHE_SIZE 100

struct CacheItem {
   uint8_t key[32];
   uint64_t data;
};

typedef struct CacheItem CacheItem;

struct Cache {
    CacheItem* table[CACHE_SIZE];
       int size;
};

typedef struct Cache Cache;

uint64_t checkCache(Cache* c, uint8_t check_hash[32]);
void insertCache(Cache* c, uint8_t k[32], uint64_t d);
int compareKey(uint8_t k1[32], uint8_t k2[32]);

#endif
