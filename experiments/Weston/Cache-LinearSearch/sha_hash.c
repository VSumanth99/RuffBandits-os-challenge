#include "sha_hash.h"

int is_hash_equal(uint64_t number, uint8_t check_hash[32])
{
  ArrayInteger8 temp;
  temp.number = number;
  unsigned char hash[32];
  SHA256((unsigned char*)temp.array, 8, hash);

  printf("Reverse hashed %lu\n",number);
  printf("Now comparing: ");
  for (int i=0;i<32;i++) {
    printf("%d ",hash[i]);
  }
  printf("\nTo: ");
  for (int i=31;i>=0;i--) {
    printf("%d ",check_hash[i]);
  }
  printf("\n");

  for(int i = 0; i < 32; i++)
  {
    printf("At char index: %d. First char is = '%d' Second is = '%d'\n",i,hash[i],check_hash[31-i]);
    if(hash[i] != check_hash[31-i])
      return 0;
  }
  return 1;
}
