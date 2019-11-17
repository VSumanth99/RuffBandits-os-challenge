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
#include "cache.h"


#define NUM_THREADS 4

int k = 0;

pthread_mutex_t requests_lock; //mutex to ensure thread protection for request queue
pthread_mutex_t running_requests_lock; //mutex to ensure thread protection for running requests queue
sem_t sem_free_threads; //semaphores to handle the requests in the queue
PriorityQueue requests; //create a queue to hold all our requests
PriorityQueue running_requests, paused_requests; //queues for holding all the running and paused processes
Cache *cache; // create a cache to hold the results of our successful reverse hashes

int start_request = 0, end_request = 0;

/* Convention: All running processes have negative priority score
This is because we want to ensure that the running_requests priority queue is a min heap
This ensures that we pause the minimum priority tasks

All paused processes have positive priority score so that we again have a max heap
*/

float calc_score(uint64_t start, uint64_t end, uint8_t priority)
{
    return 100000.0f*priority/(end-start);
}

void* handle_req(void* t)
{
    int threadNo = ++k;
    PriorityNode *node = ((PriorityNode*)t);
    struct ClientRequest *r = node->r;
    uint64_t answer = 0;

    //printf("Starting request: %d\n", ++start_request);
    
    answer = checkCache(cache, r->hash);

    if (answer==-1) {
        for (uint64_t i = r->start; i < r->end; i++)
        {
            if(i%1000 == 0 )
            {
                //update the priority queue with the new length
                node->priority_score = -1.0f * calc_score(i, r->end, r->priority);
                pthread_mutex_lock(&running_requests_lock);
                reheap(&running_requests);
                pthread_mutex_unlock(&running_requests_lock);

                pthread_mutex_lock(&node->lock);

                //suspend the thread
                if(!node->running)
                {
                sem_post(&sem_free_threads);
            //    while(!node->running)
                pthread_cond_wait(&node->condition, &node->lock);
                sem_wait(&sem_free_threads);
                    //after this point, we can resume operation
                }
                pthread_mutex_unlock(&node->lock);
            }
            if (is_hash_equal(i, r->hash))
            {
                answer = i;
                insertCache(cache, r->hash, answer);
                break;
            }

        }
    }

    
    write_to_client(r->socket, answer);
    //printf("Closing request: %d\n", ++end_request);
    close(r->socket);
    free(r);
    pthread_mutex_lock(&running_requests_lock);
    remove_from_queue(&running_requests, node);
    pthread_mutex_unlock(&running_requests_lock);
    int threads;
    sem_getvalue(&sem_free_threads, &threads);
    sem_post(&sem_free_threads);
    pthread_exit(NULL);
    return NULL;
}

void* scheduler()
{
    while(1)
    {
        //check for requests and complete them
        PriorityNode *n = NULL;
        PriorityNode *request_top = peek_priority(&requests);
        PriorityNode *paused_top = peek_priority(&paused_requests);

        pthread_mutex_lock(&requests_lock);
        if(request_top != NULL || paused_top != NULL)
        {
            if((request_top !=NULL) && (paused_top==NULL || request_top->priority_score > -1.0f * paused_top->priority_score))
            {
              n = remove_top(&requests);
            }
            else
            {
                n = remove_top(&paused_requests);
                n->priority_score *= -1.0f;
                //signal the condition on the node
                pthread_mutex_lock(&n->lock);
                n->running = 1;
                pthread_mutex_unlock(&n->lock);
                pthread_cond_signal(&n->condition);

                //ensure that another thread is not added
                n = NULL;
            }
        }

        pthread_mutex_unlock( &requests_lock);
        //process the request
        if(n!=NULL)
        {
            //first we check if our current priorities are higher than any running threads
            pthread_mutex_lock(&running_requests_lock);
            PriorityNode *running_node = peek_priority(&running_requests);
            pthread_mutex_unlock(&running_requests_lock);
            if(running_node!=NULL && -1.0f * running_node->priority_score < n->priority_score)
            {
                //pause the thread with the low priority
                pthread_mutex_lock(&running_node->lock);
                running_node->running = 0;
                pthread_mutex_unlock(&running_node->lock);
                //remove the thread from the running queue and add it to the paused queue
                pthread_mutex_lock(&running_requests_lock);
                remove_top(&running_requests);
                pthread_mutex_unlock(&running_requests_lock);
                //fix the priority of the node to be paused
                running_node->priority_score *= -1.0f;
                add_to_heap(&paused_requests, running_node);
            }

            sem_wait(&sem_free_threads);
            pthread_t thread_no;
            n->running = 1;
            n->priority_score *= -1.0f;
            //add the node to the queue of running processes
            pthread_mutex_lock(&running_requests_lock);
            add_to_heap(&running_requests, n);
            pthread_mutex_unlock(&running_requests_lock);

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
    pthread_mutex_init(&requests_lock, NULL);
    pthread_mutex_init(&running_requests_lock, NULL);
    sem_init(&sem_free_threads, 0, NUM_THREADS);

    //at any given time, we do not expect more than 1000 requests in the queue at the same time
    init_heap(&requests, 1000);
    init_heap(&paused_requests, 1000);
    init_heap(&running_requests, 4);

    int curr_thread = 0;

    // Setup Cache for reusing the results of our hashing
    cache = malloc(sizeof(Cache));
    *cache->table = calloc(sizeof(CacheItem),CACHE_SIZE);

    setup_server(atoi(argv[1]));
    listen_client();
    //start the scheduler
    pthread_create(&scheduler_thread, NULL, scheduler, NULL);
    for(;;)
    {

        //we have a new client request, add it to the request queue
        struct ClientRequest *r = accept_and_retrieve_client_request();
        //setup the node that will go on the PriorityQueue requests
        PriorityNode *n = malloc(sizeof(PriorityNode));
        n->r = r;
        n->priority_score = calc_score(r->start, r->end, r->priority);
        pthread_mutex_init(&n->lock, NULL);
        pthread_cond_init(&n->condition, NULL);
        pthread_mutex_lock(&requests_lock);
        add_to_heap(&requests, n);
        pthread_mutex_unlock(&requests_lock);

    }

    close_socket();

    return 0;
}
