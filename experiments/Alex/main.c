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
#include <unistd.h>
#include <stdbool.h>

#define NUM_THREADS 4

int k = 0;

pthread_mutex_t queue_lock; //mutex to ensure thread protection for request queue
pthread_mutex_t queue_lock_2; //mutex to ensure thread protection for request queue
pthread_mutex_t queue_lock_3; //mutex to ensure thread protection for request queue
sem_t sem_free_threads; //semaphores to handle the requests in the queue
Queue requests; //create a queue to hold all our requests

int start_request = 0, end_request = 0;
int request_number = 0;
int boolean_history[1000000];

struct thread_args {
    struct ClientRequest *r;
    long start;
    long end;
    int request_number;
    int thread_number;
};

void* handle_req(void* argp)
{
    //sleep(1);
    struct thread_args *args = argp;

    struct ClientRequest *r = args->r;
    int req_num = args->request_number;
    int thread_number = args->thread_number;
    long start = args->start;
    long end = args->end;
    uint64_t answer = 0;

    printf("Starting, Request number: %d Thread number: %d Boolean history number: %d R start: %lu R End: %lu \n", req_num, thread_number, boolean_history[req_num], start, end);
    for (uint64_t i = start; i < end; i++) {
        // if it has been found already the correct value from another thread then exit current thread.
        if (boolean_history[req_num]) {
            printf("Exit bad thread, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
            sem_post(&sem_free_threads);
            pthread_exit(NULL);
        }
        if (is_hash_equal(i, r->hash)) {
            printf("Hash is found, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
            boolean_history[req_num] = 1; // set boolean value as 1 indicating that the value has been found.
            answer = i;
            break;
        }
    }
    if (answer != 0) {
        write_to_client(r->socket, answer);
        printf("Closing good thread, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
        close(r->socket);
        free(r);
    }

    printf("Exit thread, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
    sem_post(&sem_free_threads);
    pthread_exit(NULL);
}

int * split_range(void* t, int *num_values, long *ranges) {
    struct ClientRequest *r = (((Node*)t)->data);
    int array_size = 4;
    int difference = r->end - r->start;
    int quantum = difference / array_size;
    int i;
    *num_values = 0;
    for (i = 0; i < array_size; i++) {
        ranges[i] = r->start + i * quantum;
        *num_values += 1;
    }
    ranges[i] = r->end;
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
        pthread_mutex_unlock(&queue_lock);

        //process the request
        if(n!=NULL)
        {

            pthread_t thread_no[1000];
            int num_values;
            // Splitting the searching range to 4 parts. Each part is assign to one thread.
            long ranges[5] = {};
            split_range((void*)(n), &num_values, ranges);
            request_number += 1;
            //printf("Request number: %d\n", request_number);

            struct thread_args args[num_values ];
            // below we needed assign the 4 parts of the range to each thread
            int file_index = 0;
            int file_index_2 = 0;
            for (int i = 0; i < num_values; i++) {
                int j = i;
                sem_wait(&sem_free_threads);

                args[i].r = (((Node*)n)->data);
                args[i].start = ranges[i];
                args[i].end = ranges[i + 1];
                args[i].request_number = request_number;
                args[i].thread_number = j;
                pthread_create(&thread_no[file_index], NULL, handle_req, &args[i]);
                file_index++;
            }
            // waiting for the threads to exit before server another request
            for (int i = 0; i < num_values; i++) {
                pthread_join(thread_no[file_index_2], NULL);
                file_index_2++;
            }
        }

    }
}

int main( int argc, char *argv[] )
{
    printf("My PID is %d\n", getpid());

    if(argc != 2)
    {
        printf("Invalid usage\n");
        printf("Correct usage: server <port>\n");
        return 1;
    }
    pthread_t scheduler_thread;
    //initialise multi-threading constructs
    pthread_mutex_init(&queue_lock, NULL);
    pthread_mutex_init(&queue_lock_2, NULL);
    pthread_mutex_init(&queue_lock_3, NULL);
    sem_init(&sem_free_threads, 0, NUM_THREADS);

    int curr_thread = 0;

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