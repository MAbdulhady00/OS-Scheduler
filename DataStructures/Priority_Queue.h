#pragma once
#include "Dynamic_Array.h"

// Provides a priority queue using a minheap
typedef struct
{
    DynamicArray *arr;
    bool (*cmp)(element_t, element_t);
} PriorityQueue;

PriorityQueue *CreatePriorirtyQueue(bool (*cmp)(element_t, element_t));

void PriorityQueuePush(PriorityQueue *pq, element_t element);

void PriorityQueuePop(PriorityQueue *pq);

element_t PriorityQueueGetMin(PriorityQueue *pq);

void DestroyPriorityQueue(PriorityQueue *pq);