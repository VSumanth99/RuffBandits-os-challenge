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
sem_t sem_free_threads; //semaphores to handle the requests in the queue
Queue requests; //create a queue to hold all our requests

int start_request = 0, end_request = 0;
int request_number = 0;
<<<<<<< HEAD
bool found = 0;
=======
>>>>>>> e9170474c0b5232aece03401876ceaca84e9fff8
int file_index = 0;
int boolean_history[1000000];

void* handle_req(void* t)
{
    struct ClientRequest *r = (((Node*)t)->data);
    int req_num = (((Node*)t)->request_number);
    int thread_number = (((Node*)t)->thread_number);
    uint64_t answer = 0;

    //printf("Starting request: %d\n", req_num);
<<<<<<< HEAD
    printf("Starting, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[request_number]);
    for (uint64_t i = r->start; i < r->end; i++) {
        printf("Continue.., Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[request_number]);
        if (boolean_history[request_number]) {
            printf("Closing 1, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[request_number]);
            sem_post(&sem_free_threads);
            printf("Closing 2, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[request_number]);
            pthread_exit(NULL);
            printf("Closing 2, Request number: %d Thread number: %d \n", req_num, thread_number);
            return NULL;
            printf("Closing 3, Request number: %d Thread number: %d \n", req_num, thread_number);

        }
        if (is_hash_equal(i, r->hash)) {
            printf("Continue...is_hash, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[request_number]);
            pthread_mutex_lock(&queue_lock_2);
            found = 1;
            boolean_history[request_number] = 1;
=======
    printf("Starting, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
    for (uint64_t i = r->start; i < r->end; i++) {
        if (boolean_history[req_num]) {
            printf("Closing 1, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
            //free(r);
            sem_post(&sem_free_threads);
            printf("Closing 2, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
            pthread_exit(NULL);
        }
        if (is_hash_equal(i, r->hash)) {
            printf("Continue...is_hash, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
            pthread_mutex_lock(&queue_lock_2);
            boolean_history[req_num] = 1;
>>>>>>> e9170474c0b5232aece03401876ceaca84e9fff8
            pthread_mutex_unlock(&queue_lock_2);
            answer = i;
            break;
        }
    }
    if (answer != 0) {
        write_to_client(r->socket, answer);
        //printf("Closing request: %d\n", ++end_request);
<<<<<<< HEAD
        printf("Closing, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[request_number]);
=======
        printf("Closing, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
>>>>>>> e9170474c0b5232aece03401876ceaca84e9fff8
        close(r->socket);
        free(r);
    }

<<<<<<< HEAD
    printf("Thread exit, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[request_number]);
    sem_post(&sem_free_threads);
    pthread_exit(NULL);
    return NULL;
}

int * split_range(void* t) {
=======
    printf("Thread exit, Request number: %d Thread number: %d Boolean history number: %d\n", req_num, thread_number, boolean_history[req_num]);
    sem_post(&sem_free_threads);
    pthread_exit(NULL);
}

int * split_range(void* t, int *num_values) {
>>>>>>> e9170474c0b5232aece03401876ceaca84e9fff8
    struct ClientRequest *r = (((Node*)t)->data);
    int array_size = 1;
    int difference = r->end - r->start;

    if (difference < 1000) {
        array_size = 1;
    } else if (difference < 10000 ) {
        array_size = 2;
    } else {
        array_size = 4;
    }

    int *ranges = malloc(array_size + 1);
    int quantum = difference / array_size;
    int i;
<<<<<<< HEAD
    for (i = 0; i < array_size; i++) {
        ranges[i] = r->start + i * quantum;
=======
    *num_values = 1;
    for (i = 0; i < array_size; i++) {
        ranges[i] = r->start + i * quantum;
        *num_values += 1;
>>>>>>> e9170474c0b5232aece03401876ceaca84e9fff8
    }
    ranges[i] = r->end;
    return ranges;
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

<<<<<<< HEAD
            //pthread_create(&thread_no, NULL, handle_req, (void*)(n));
            pthread_t thread_no[1000];

            int *ranges = split_range((void*)(n));
            size_t number_of_threads = sizeof(ranges)/sizeof(ranges[0]);
            request_number += 1;
            printf("Request number: %d\n", request_number);
            for (int i = 0; i < number_of_threads; i++) {
=======
            pthread_t thread_no[1000];

            int num_values;
            int *ranges = split_range((void*)(n), &num_values);
            request_number += 1;
            //printf("Request number: %d\n", request_number);
            for (int i = 0; i < num_values - 1; i++) {
>>>>>>> e9170474c0b5232aece03401876ceaca84e9fff8
                sem_wait(&sem_free_threads);

                struct ClientRequest *r = (((Node*)n)->data);
                r->start = *(ranges + i);
                r->end = *(ranges + i + 1);
                (((Node*)n))->request_number = request_number;
                (((Node*)n))->thread_number = i;
                pthread_create(&thread_no[file_index], NULL, handle_req, (void*)(n));
                file_index++;
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
<<<<<<< HEAD
}
=======
}
>>>>>>> e9170474c0b5232aece03401876ceaca84e9fff8
