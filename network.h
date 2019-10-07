
#ifndef NETWORK_H
#define NETWORK_H


#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include "byte_utils.h"
/*
*The following is a struct used for dealing with the server request.
*It retrieves the hash value received from the server along with the start,
*end and priority value
*/

struct ClientRequest
{
  uint8_t hash[32];
  uint64_t start;
  uint64_t end;
  uint8_t priority;
};
typedef struct ClientRequest ClientRequest;

void setup_server(int portno);
void listen_accept_client();
ClientRequest retrieve_client_request();
void write_to_client(uint64_t message);
void close_socket();

#endif
