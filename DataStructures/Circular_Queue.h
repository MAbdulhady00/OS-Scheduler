#pragma once
#include "../process.h"
#define element_t process * // Change this to change Circular Queue type used

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
} CircularQueue;

CircularQueue *CreateCircularQueue();

node *newNode(element_t v);

int isCircularQueueEmpty(CircularQueue *Q);

void enqueueCircularQueue(CircularQueue *Q, element_t element);

element_t peekCircularQueue(CircularQueue *Q);

void advanceCircularQueue(CircularQueue *Q);

element_t dequeueCircularQueue(CircularQueue *Q);

void destroyCircularQueue(CircularQueue *Q);