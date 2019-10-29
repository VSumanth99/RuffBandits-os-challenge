
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
*end and priority value, and the corresponding socket
*/

struct ClientRequest
{
  uint8_t hash[32];
  uint64_t start;
  uint64_t end;
  uint8_t priority;
  int socket;
};
typedef struct ClientRequest ClientRequest;

void setup_server(int portno);
void listen_client();
ClientRequest accept_and_retrieve_client_request();
void write_to_client(int client_socket, uint64_t message);
void close_socket();

#endif
