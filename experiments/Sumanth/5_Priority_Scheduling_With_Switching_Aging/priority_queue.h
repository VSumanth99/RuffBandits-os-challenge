#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "network.h"
#include <stdlib.h>

struct PriorityNode
{
    ClientRequest* r;
    float priority_score;
    int running; //boolean to indicate whether the request is still running or not
    pthread_mutex_t lock; //mutex to ensure thread protection for predicate check
    pthread_cond_t condition; //condition corresponding to the thread
};
typedef struct PriorityNode PriorityNode;

struct PriorityQueue
{
    PriorityNode **arr; //array of pointers
    int length;
    int capacity;
};

typedef struct PriorityQueue PriorityQueue;


void init_heap(PriorityQueue*q, int n);
void add_to_heap(PriorityQueue*q, PriorityNode *r);
PriorityNode* remove_top(PriorityQueue*q);
void remove_from_queue(PriorityQueue*q, PriorityNode *r);
void reheap(PriorityQueue *q);
PriorityNode* peek_priority(PriorityQueue*q);

#endif
