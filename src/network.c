#include "network.h"

int server_socket, newserver_socket;
int client_length;
struct sockaddr_in server_address, client_address;

void setup_server(int portno)
{
  /*
  * This function sets up the server socket
  * It sets up the server at 192.168.101.10 with port number portno
  *
  */
  int n;

  //Setup a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket < 0) {
     perror("ERROR opening socket");
     exit(1);
  }

  //Setup the server address
  bzero((char *) &server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  //connect to the appropriate ip address
  server_address.sin_addr.s_addr = inet_addr("192.168.101.10");
  server_address.sin_port = htons(portno);

  //bind the server socket to the server address
  if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
     perror("ERROR on binding");
     exit(1);
  }
  int option = 1;
  if(setsockopt(server_socket,SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0)
  {

      printf("setsockopt failed\n");
      close(server_socket);
      exit(2);

  }

}

void listen_accept_client()
{
    //start listening for connections from the client
    listen(server_socket,5);
    client_length = sizeof(client_address);

    //accept connection from the client
    newserver_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);

    if (newserver_socket < 0) {
       perror("ERROR on accept");
       exit(1);
    }

}

ClientRequest retrieve_client_request()
{

  char input[49];

  //after connection is established, receive a 49 byte message
  bzero(input,49);

  int n = read(newserver_socket, input, 49);
  if (n < 0) {
     perror("ERROR reading from socket");
     exit(1);
  }

  uint8_t hash[32];
  for(int i=0; i < 32; i++)
     hash[i] = input[31-i];

 uint8_t p = (uint8_t) *(&input[48]);
 ArrayInteger8 start_arr, end_arr;
 for(int i = 0; i < 8; i++)
 {
   start_arr.array[7-i] = (uint8_t) *(&input[32+i]);
   end_arr.array[7-i] = (uint8_t) *(&input[40+i]);
 }

 ClientRequest r;
 //r = malloc(sizeof(ClientRequest));
 r.start = start_arr.number;
 r.end = end_arr.number;
 r.priority = p;
 for(int i = 0; i < 32; i++)
  r.hash[i] = hash[i];

  return r;
}


void write_to_client(uint64_t message)
{
  ArrayInteger8 a;
  a.number=message;
  char buffer[8];
  for(int i = 0; i < 8; i++)
    buffer[i] = (char) a.array[7-i];
  int n = write(newserver_socket, buffer, 8);
  if (n < 0) {
     perror("ERROR writing to socket");
     exit(1);
  }
}

void close_socket()
{
  close(server_socket);
}
