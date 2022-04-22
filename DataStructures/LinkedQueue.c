#include <stdlib.h>
#include <string.h>
#include "LinkedQueue.h"
LinkedQueue *CreateLinkedQueue()
{
    LinkedQueue *Q = malloc(sizeof(LinkedQueue));
    Q->front = NULL;
    Q->rear = NULL;
}
node *newNode(element_t v)
{
    node *temp = malloc(sizeof(node));
    temp->val = v;
    temp->next = NULL;
}
void enqueueLinkedQueue(LinkedQueue *Q, element_t element)
{
    // Create a new node
    node *tmp = newNode(element);
    // If LinkedQueue is empty, then new node is front and rear both
    if (!Q->rear)
    {
        Q->front = Q->rear = tmp;
        return;
    }
    // Add the new node at the end of LinkedQueue and change rear
    Q->rear->next = tmp;
    Q->rear = tmp;
}
void dequeueLinkedQueue(LinkedQueue *Q)
{
    // if LinkedQueue is empty return
    if (Q->front)
    {
        node *tmp = Q->front;
        Q->front = Q->front->next;
        free(tmp);
    }
    // empty LinkedQueue
    if (Q->front == NULL)
        Q->rear = NULL;
}

void DestroyLinkedQueue(LinkedQueue *Q)
{
    while (Q->front)
    {
        node *tmp = Q->front;
        Q->front = Q->front->next;
        free(tmp->val);
        free(tmp);
    }
}