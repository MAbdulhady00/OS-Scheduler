#include <stdlib.h>
#include <string.h>
#include "Circular_Queue.h"

CircularQueue *CreateCircularQueue()
{
    CircularQueue *Q = malloc(sizeof(CircularQueue));
    Q->front = NULL;
    Q->rear = NULL;
}

node *newNode(element_t v)
{
    node *temp = malloc(sizeof(node));
    temp->val = v;
    temp->next = NULL;
}

int isCircularQueueEmpty(CircularQueue *Q)
{
    if(Q->front == NULL)
        return 1;
    else
        return 0;
}

void enqueueCircularQueue(CircularQueue *Q, element_t element)
{
    // Create a new node
    node *tmp = newNode(element);
    // If Circular Queue is empty, then new node is front and rear both
    if (!Q->rear)
    {
        tmp->next = tmp;    
        Q->front = Q->rear = tmp;
        return;
    }
    // Add the new node at the end of Circular Queue and change rear
    tmp->next = Q->rear->next;
    Q->rear->next = tmp;
    Q->rear = tmp;
}

element_t dequeueCircularQueue(CircularQueue *Q)
{
    if (Q->front != NULL)
    {
        node *tmp = Q->front;
        if(Q->front != Q->rear)
        {
            Q->front = Q->front->next;
            Q->rear->next = Q->front;   
        }
        else
        {
            Q->front = NULL;
            Q->rear = NULL;
        }
        return tmp->val;
    }
    // empty Circular Queue
    return NULL;
}

void destroyCircularQueue(CircularQueue *Q)
{
    node* f  = Q->front;
    while (Q->front != NULL)
    {
        node *tmp = Q->front;
        Q->front = Q->front->next;
        free(tmp->val);
        free(tmp);
        if(Q->front == f)
            return;
    }
}

element_t peekCircularQueue(CircularQueue *Q)
{
    if(Q->front == NULL)
        return NULL;
    return Q->front->val;
}

void advanceCircularQueue(CircularQueue *Q)
{
    Q->front = Q->front->next;
    Q->rear = Q->rear->next;
}