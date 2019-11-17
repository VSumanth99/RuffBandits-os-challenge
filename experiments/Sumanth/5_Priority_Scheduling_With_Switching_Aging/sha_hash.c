#include "sha_hash.h"

int is_hash_equal(uint64_t number, uint8_t check_hash[32])
{
    ArrayInteger8 temp;
    temp.number = number;
    unsigned char hash[32];
    SHA256((unsigned char*)temp.array, 8, hash);

    for(int i = 0; i < 32; i++)
    {
        if(hash[i] != check_hash[31-i])
            return 0;
    }
    return 1;
}
