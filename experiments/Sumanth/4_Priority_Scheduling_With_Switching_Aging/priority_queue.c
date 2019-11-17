
#include "priority_queue.h"

void init_heap(PriorityQueue*q, int n)
{
    q->arr = malloc(sizeof(PriorityQueue*) * (n+1));
    q->length = 0;
    q->capacity = n+1;
}

void heapify(PriorityQueue*q, int n)
{
    int index = n;
    while(index!=1)
    {
        if(q->arr[index]->priority_score > q->arr[index/2]->priority_score)
        {
            //swap the element with it's parent
            PriorityNode *temp = q->arr[index];
            q->arr[index] = q->arr[index/2];
            q->arr[index/2] = temp;
            index /= 2;
        }
        else
            break;
    }
}

void heapify_down(PriorityQueue*q, int n)
{
    int index = n;
    while(index <= q->length)
    {
        if(index*2 <= q->length && q->arr[index]->priority_score < q->arr[index*2]->priority_score)
        {
            //swap the element with it's child
            PriorityNode *temp = q->arr[index];
            q->arr[index] = q->arr[index*2];
            q->arr[index*2] = temp;
            index = index*2;
        }
        else if(index*2+1 <= q->length && q->arr[index]->priority_score < q->arr[index*2+1]->priority_score)
        {
            //swap the element with it's child
            PriorityNode *temp = q->arr[index];
            q->arr[index] = q->arr[index*2+1];
            q->arr[index*2+1] = temp;
            index = index*2 + 1;
        }
        else
            break;
    }
}

void reheap(PriorityQueue *q)
{
    int index = q->length/2 - 1;
    for(int i = index; i>=1; i--)
    {
      heapify_down(q, i);
    }

}

void add_to_heap(PriorityQueue *q, PriorityNode *r)
{
    /*
    if(q->length == q->capacity)
    {
      //create a new array and copy the old contents into the new array
      PriorityNode **new_arr = malloc(sizeof(PriorityQueue*)*(2*q->capacity-1));
      for(int i = 0; i <= q->length; i++)
      {
        new_arr[i] = q->arr[i];
      }
      q->capacity = 2*q->capacity - 1;
      free(q->arr);
      q->arr = new_arr;
    }*/
    if(q->length == q->capacity)
    {
      printf("OVERFLOW in the queue: capacity %d exceeded\n", q->capacity);
    }
    q->arr[++(q->length)] = r;
    heapify(q, q->length);
}

PriorityNode* remove_top(PriorityQueue *q)
{
    PriorityNode * ret_val = q->arr[1];
    q->arr[1] = q->arr[q->length--];

    q->arr[q->length+1] = NULL;
    heapify_down(q, 1);

    /*
      if(q->length <= q->capacity/4)
      {
        //create a new array and copy the old contents into the new array
        PriorityNode **new_arr = malloc(sizeof(PriorityQueue*)*(q->capacity/2 + 1));
        for(int i = 0; i <= q->length; i++)
        {
          new_arr[i] = q->arr[i];
        }
        q->capacity = q->capacity/2 + 1;
        free(q->arr);
        q->arr = new_arr;
      }*/
    return ret_val;

}

void remove_from_queue(PriorityQueue*q, PriorityNode *r)
{
    for(int i = 1; i <= q->length; i++)
    {
      if(q->arr[i] == r)
      {
        //bring the last element to this spot
        q->arr[i] = q->arr[q->length];
        q->length--;
        reheap(q);
        break;
      }
    }
}

PriorityNode* peek_priority(PriorityQueue *q)
{
  if(q->length == 0)
    return NULL;
  else
    return q->arr[1];
}

/*
int main(int argc, char const *argv[]) {
  PriorityQueue q;
  init_heap(&q, 10);

  PriorityNode *n1 = malloc(sizeof(PriorityNode));
  n1->priority_score = 10.0f;

  PriorityNode *n2 = malloc(sizeof(PriorityNode));
  n2->priority_score = 20.0f;
  PriorityNode *n3 = malloc(sizeof(PriorityNode));
  n3->priority_score = 30.0f;
  PriorityNode *n4 = malloc(sizeof(PriorityNode));
  n4->priority_score = 5.0f;

  add_to_heap(&q, n1);
  add_to_heap(&q, n2);
  add_to_heap(&q, n3);
  add_to_heap(&q, n4);

  for(int i = 1; i <= 4; i++)
  {
    printf("%f\n", q.arr[i]->priority_score);
  }

  return 0;
}
*/
