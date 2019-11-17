#include "queue.h"

void enqueue(Queue *q, Node *n)
{
    if(q->front == NULL)
    {
        q->front = n;
        q->back = n;
        n->next = n->prev = NULL;
    }
    else
    {
        n->next = q->back;
        q->back->prev = n;
        n->prev = NULL;
        q->back = n;
    }
}

Node* dequeue(Queue *q)
{
    if(q->front == NULL)
    {
        printf("Underflow occured!!!\n");
        return NULL;
    }
    else if(q->front == q->back)
    {
        Node *ret = q->front;
        q->front = q->back = NULL;
        return ret;
    }
    Node * ret = q->front;
    q->front->prev->next = NULL;
    q->front = q->front->prev;
    return ret;
}

Node* peek(Queue *q)
{
    return q->front;
}

void printQueue(Queue *q)
{
    Node *a = q->front;
    while(a!=NULL)
    {
        printf("%d\n", a->data->priority);
        a = a->prev;
    }
}