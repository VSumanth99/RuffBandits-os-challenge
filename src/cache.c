#include "cache.h"
#include <stdlib.h>

uint64_t checkCache(Cache* c, uint8_t check_hash[32]) {
    //printf("Checking cache for hash: ");
    for (int i=0;i<32;i++) {
        //printf("%d ",check_hash[i]);
    }
    //printf("\n");

    int code = check_hash[0] + check_hash[1] + check_hash[2] + check_hash[3];

    if (code>999) {
        code = code - 20;
    }

    printf("Calculated hash code: %d\n",code); 
        
    if (compareKey(c->table[code]->key,check_hash)) {
        printf("Hash was found!\n");
        return c->table[code]->data;   
    } else {
        printf("Hash was not found!\n");
    }
    return -1;
}

void insertCache(Cache* c, uint8_t k[32], uint64_t d) {


    //printf("Inserting value into table:\nKey:");
    for (int i=0;i<32;i++) {
        //printf("%d ",k[i]);
    }
    //printf("\nValue: %lu\n",d);

    CacheItem * newItem = malloc(sizeof(CacheItem));

    
    for (int i=0;i<32;i++) {
        newItem->key[i] = k[i];
    }
    
    newItem->data=d;

    int code = k[0] + k[1] + k[2] + k[3];

    //printf("Calculated code: %d\n",code);

    c->table[code]=newItem;

    printf("Entered the following into index:%d\n",code);
    //printf("Key: ");
    for (int j=0;j<32;j++) {
        //printf("%d ",c->table[code]->key[j]);
    }
    //printf("\nVal: %lu",c->table[code]->data);
    //printf("\n");
    return;
}



int compareKey(uint8_t k1[32], uint8_t k2[32]) {
    if (k1==NULL) {
        //printf("The table is empty at that index!\n");
        return 0;
    }
    
    //printf("Table is filled at that index.\nFirst key is this: ");
    for (int i=0;i<32;i++) {
        //printf("%d ",k1[i]);
    }
    //printf("\nSecond key is this: ");
    for (int i=0;i<32;i++) {
        //printf("%d ",k2[i]);
    }
    //printf("\n");

    for (int i=0;i<32;i++) {
        //printf("At char index: %d. First char is = '%d' Second is = '%d'\n",i,k1[i],k2[i]);
        if (k1[i]!=k2[i]) {
            //printf("Characters are different! Not the same hash.\n");
            return 0;
        }
    }
    //printf("Keys were the same!\n");
    return 1;

}

