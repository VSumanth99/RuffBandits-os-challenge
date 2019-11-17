#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>
#include "network.h"
#include "byte_utils.h"
#include "sha_hash.h"
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"
#include "cache.h"


#define NUM_THREADS 4

int k = 0;

pthread_mutex_t queue_lock; //mutex to ensure thread protection for request queue
sem_t sem_free_threads; //semaphores to handle the requests in the queue
Queue requests; //create a queue to hold all our requests
Cache *cache; // crate a cache to hold the results of our successful reverse hashes

int start_request = 0, end_request = 0;

void* handle_req(void* t)
{
  int threadNo = ++k;
  struct ClientRequest *r = (((Node*)t)->data);
  uint64_t answer = 0;
  printf("Starting request: %d\n", ++start_request);

  printf("This is the hash: ");
  for (int i=0;i<32;i++) {
    printf("%d ",r->hash[i]);
  }
  printf("\n");

  answer = checkCache(cache, r->hash);

  if (answer==-1) {
    printf("Cache lookup failed. Reverse hasing.");
    for (uint64_t i = r->start; i < r->end; i++) {
      printf("Trying number: %lu\n",i);
      if (is_hash_equal(i, r->hash)) {
          answer = i;
          printf("Got a correct answer: %lu\n",answer);
          insertCache(cache, r->hash, answer);
          break;
      }
    }
  } else {
    printf("Found answer in cache!\n");
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
    Node*n = NULL;
    pthread_mutex_lock(&queue_lock);
    if(peek(&requests) != NULL)
    {
      n = dequeue(&requests);
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

  int curr_thread = 0;

  // Setup Cache for reusing the results of our hashing
  cache = malloc(sizeof(Cache));
  *cache->table = calloc(sizeof(CacheItem),CACHE_SIZE);
  cache->size = 0;

  // NULL all initialy values.
  for (int i=0;i<CACHE_SIZE;i++) {
    cache->table[i]=NULL;
  }

  setup_server(atoi(argv[1]));
  listen_client();
  //start the scheduler
  pthread_create(&scheduler_thread, NULL, scheduler, NULL);
  for(;;)
  {

        //we have a new client request, add it to the request queue
        struct ClientRequest *r = accept_and_retrieve_client_request();
        Node *n = malloc(sizeof(Node));

        n->data = r;
        pthread_mutex_lock(&queue_lock);
        //printf("Added request number:%d\n", ++start_request);
        enqueue(&requests, n);
        pthread_mutex_unlock(&queue_lock);

  }

  close_socket();

   return 0;
}
