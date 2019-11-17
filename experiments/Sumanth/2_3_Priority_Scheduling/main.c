#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>
#include "network.h"
#include "byte_utils.h"
#include "sha_hash.h"
#include <pthread.h>
#include <semaphore.h>
#include "priority_queue.h"

#define NUM_THREADS 4

int k = 0;

pthread_mutex_t queue_lock; //mutex to ensure thread protection for request queue
sem_t sem_free_threads; //semaphores to handle the requests in the queue
PriorityQueue requests; //create a queue to hold all our requests

int start_request = 0, end_request = 0;

float calc_score(uint64_t start, uint64_t end, uint8_t priority)
{
    return 100.0f*priority/(end-start);
}

void* handle_req(void* t)
{
  int threadNo = ++k;
  struct ClientRequest *r = ((PriorityNode*)t)->r;
  uint64_t answer = 0;
  printf("Starting request: %d\n", ++start_request);

  for (uint64_t i = r->start; i < r->end; i++) {
      if (is_hash_equal(i, r->hash)) {
          answer = i;
          break;
      }

  }
  write_to_client(r->socket, answer);
  printf("Closing request: %d\n", ++end_request);
  close(r->socket);
  free(r);
  sem_post(&sem_free_threads);
  pthread_exit(NULL);
  return NULL;
}

void* scheduler()
{
  while(1)
  {
    //check for requests and complete them
    PriorityNode*n = NULL;
    pthread_mutex_lock(&queue_lock);
    if(peek_priority(&requests) != NULL)
    {
      n = remove_top(&requests);
    }
    pthread_mutex_unlock( &queue_lock);
    //process the request
    if(n!=NULL)
    {
      sem_wait(&sem_free_threads);
      pthread_t thread_no;

      pthread_create(&thread_no, NULL, handle_req, (void*)(n));
    }
  }
}

int main( int argc, char *argv[] )
{
  if(argc != 2)
  {
    printf("Invalid usage\n");
    printf("Correct usage: server <port>\n");
    return 1;
  }
  pthread_t scheduler_thread;
  //initialise multi-threading constructs
  pthread_mutex_init(&queue_lock, NULL);
  sem_init(&sem_free_threads, 0, NUM_THREADS);

  //at any given time, we do not expect more than 1000 requests in the queue at the same time
  init_heap(&requests, 1000);
  int curr_thread = 0;

  setup_server(atoi(argv[1]));
  listen_client();
  //start the scheduler
  pthread_create(&scheduler_thread, NULL, scheduler, NULL);
  for(;;)
  {

        //we have a new client request, add it to the request queue
        struct ClientRequest *r = accept_and_retrieve_client_request();
        PriorityNode *n = malloc(sizeof(PriorityNode));
        n->r = r;
        n->priority_score = calc_score(r->start, r->end, r->priority);
        pthread_mutex_lock(&queue_lock);
        //printf("Added request number:%d\n", ++start_request);
        add_to_heap(&requests, n);
        pthread_mutex_unlock(&queue_lock);

  }

  close_socket();

   return 0;
}
