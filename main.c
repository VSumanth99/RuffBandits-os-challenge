#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>
#include "network.h"
#include "byte_utils.h"
#include "sha_hash.h"
#include <pthread.h>

#define NUM_THREADS 1000

int k = 0;

void* handle_req(void* t)
{
  int threadNo = ++k;
  struct ClientRequest r = *((struct ClientRequest*)t);
  uint64_t answer = 0;

  for (uint64_t i = r.start; i < r.end; i++) {
      if (is_hash_equal(i, r.hash)) {
          answer = i;
          break;
      }

}
  write_to_client(r.socket, answer);
  pthread_exit(NULL);
  return NULL;
}

int main( int argc, char *argv[] )
{

  pthread_t threads[NUM_THREADS];
  int curr_thread = 0;
  if(argc != 2)
  {
    printf("Invalid usage\n");
    printf("Correct usage: server <port>\n");
    return 1;
  }
  setup_server(atoi(argv[1]));
  listen_client();
  for(;;)
  {
        struct ClientRequest r = accept_and_retrieve_client_request();
        //we have a new client request, create a new thread to handle it
        pthread_create(&threads[curr_thread++], NULL, handle_req, (void*)&r);
  }
  for(int i = 0; i < 5; i++)
  {
    pthread_join(threads[i],NULL);
  }
  close_socket();

   return 0;
}
