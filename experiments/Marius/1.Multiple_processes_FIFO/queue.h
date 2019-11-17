#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdio.h>
#include "network.h"

struct Node
{
  ClientRequest* data;
  struct Node *next;
  struct Node *prev;
};


typedef struct Node Node;

struct Queue
{
  Node* front;
  Node* back;
};

typedef struct Queue Queue;

void enqueue(Queue *q, Node *n);
Node* dequeue(Queue *q);
Node* peek(Queue *q);

#endif
