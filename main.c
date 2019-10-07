#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>
#include "network.h"
#include "byte_utils.h"
#include "sha_hash.h"


int main( int argc, char *argv[] )
{
  uint64_t answer = 0;
  if(argc != 2)
  {
    printf("Invalid usage\n");
    printf("Correct usage: server <port>\n");
    return 1;
  }
  setup_server(atoi(argv[1]));
  listen_client();
    for(;;) {
        accept_client();

        struct ClientRequest r = retrieve_client_request();
        for (uint64_t i = r.start; i < r.end; i++) {
            if (is_hash_equal(i, r.hash)) {
                answer = i;
                break;
            }
        }
        write_to_client(answer);
    }
  // close_socket();

   return 0;
}
