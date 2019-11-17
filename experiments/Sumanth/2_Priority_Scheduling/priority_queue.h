#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "network.h"
#include <stdlib.h>

struct PriorityNode
{
  ClientRequest* r;
  float priority_score;
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
PriorityNode* peek_priority(PriorityQueue*q);

#endif
