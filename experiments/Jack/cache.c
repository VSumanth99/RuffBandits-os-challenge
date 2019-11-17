#include "cache.h"
#include <stdlib.h>

uint64_t checkCache(Cache* c, uint8_t check_hash[32]) {
    printf("Checking cache for hash: ");
    for (int i=0;i<32;i++) {
        printf("%d ",check_hash[i]);
    }
    printf("\n");

    for (int i=0;i<CACHE_SIZE;i++) {
        printf("Looking at index: %d\n",i);

        if (compareKey(c->table[i]->key,check_hash)) {
            printf("Hash was found!\n");
            return c->table[i]->data;
        } else {
            printf("Hash was not found at index!\n");
        }

    }
    printf("Hash was not found at any index!\n");
    return -1;
}


void insertsort(Cache c){
  int i = c->size;
  uint8_t k[32];
  int check = 1;
  //for loop to iterate over the table[] array
  while(i>1 && check == 1){
    // for loop to iterate of the char array
    for (int j=0;j<32;j++) {
      int x = (int)(c->table[i-2]->key[j]);
      int y = (int)(c->table[i-1]->key[j]);
      if(x==y || x<y){
        int check = 0;
      }
      // if x is greater than y swaps the values and moves to next table[] vals
        if(x>y){
          int check = 1;
          unit64_t temp = c->table[j-2]->data;
          c->table[j-2]->data = c->table[j-1]->data;
          c->table[j-1]->data = temp;

          // swaps key values for array
          for (int l=0;l<32;l++) {
            unit8_t k[l] = c->table[j-2]->key[l];
            c->table[j-2]->key[l] = c->table[j-1]->key[l];
            c->table[j-1]->key[l] = k[l];
          }
          break;
        }
    }
    i -= 1;
  }
  return c;
}

uint64_t checkCacheBinary(Cache* c, uint8_t check_hash[32]) {
    printf("Checking cache for hash: ");
    for (int i=0;i<32;i++) {
        printf("%d ",check_hash[i]);
    }
    printf("\n");

    int i=0;
    int j=CACHE_SIZE-1;

    while (true){
      if(j<i){
        break;
      }
      int m = floor(i+j/2);
      if (compareKey(c->table[m]->key,check_hash)) {
          printf("Hash was found!\n");
          return c->table[m]->data;}

      for (int l=0;l<32;l++) {
          int x = (int)(c->table[m]->key[l]);
          int y = (int) check_hash[l];
          if(x<y){
            int i = m + 1;
            break;
          }else if(x>y){
            int j = m-1;
            break;
          }
      }
    }
    printf("Hash was not found at any index!\n");
    return -1;
}


void insertCache(Cache* c, uint8_t k[32], uint64_t d) {

    int i = c->size;

    printf("Inserting value into table:\nKey:");
    for (int i=0;i<32;i++) {
        printf("%d ",k[i]);
    }
    printf("\nValue: %lu\n",d);

    CacheItem * newItem = malloc(sizeof(CacheItem));


    for (int i=0;i<32;i++) {
        newItem->key[i] = k[i];
    }

    newItem->data=d;


    printf("Jumping to index: %d. It's empty!\n",i);

    c->table[c->size]=newItem;

    printf("Entered the following into index:%d ",i);
    printf("Key: ");
    for (int j=0;j<32;j++) {
        printf("%d ",c->table[i]->key[j]);
    }
    printf("\nVal: %lu",c->table[i]->data);
    printf("\n");
    c->size++;
    return;
}



int compareKey(uint8_t k1[32], uint8_t k2[32]) {
    if (k1==NULL) {
        printf("The table is empty at that index!\n");
        return 0;
    }

    printf("Table is filled at that index.\nFirst key is this: ");
    for (int i=0;i<32;i++) {
        printf("%d ",k1[i]);
    }
    printf("\nSecond key is this: ");
    for (int i=0;i<32;i++) {
        printf("%d ",k2[i]);
    }
    printf("\n");

    for (int i=0;i<32;i++) {
        printf("At char index: %d. First char is = '%d' Second is = '%d'\n",i,k1[i],k2[i]);
        if (k1[i]!=k2[i]) {
            printf("Characters are different! Not the same hash.\n");
            return 0;
        }
    }
    printf("Keys were the same!\n");
    return 1;

}
