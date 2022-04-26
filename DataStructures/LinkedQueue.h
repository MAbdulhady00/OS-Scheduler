#pragma once
#include "../process_data.h"
#define element_t process * // Change this to change LinkedQueue type used
struct node
{
    element_t val;
    struct node* next;
};
typedef struct node node;
typedef struct
{
    node *front;
    node *rear;
} LinkedQueue;

LinkedQueue *CreateLinkedQueue();

node *newNode(element_t v);

void enqueueLinkedQueue(LinkedQueue *Q, element_t element);

void dequeueLinkedQueue(LinkedQueue *Q);

void DestroyLinkedQueue(LinkedQueue *Q);