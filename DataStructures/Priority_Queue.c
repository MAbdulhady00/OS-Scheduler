#include <stdbool.h>
#include <stdlib.h>
#include "Priority_Queue.h"

#define PRIORITY_QUEUE_DEFAULT_CAPACITY 64
#define PQ(i) pq->arr->data[i]
#define PQ_PARENT(i) (i - 1) / 2
#define PQ_LEFT(i) 2 * i + 1
#define PQ_RIGHT(i) 2 * i + 2
#define PQ_SIZE(pq) pq->arr->size

PriorityQueue *CreatePriorirtyQueue(bool (*cmp)(element_t, element_t))
{
    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    pq->cmp = cmp;
    pq->arr = CreateDynamicArray(PRIORITY_QUEUE_DEFAULT_CAPACITY);
    pq->fifo = 0;
    return pq;
}

static void heap_swap(PriorityQueue *pq, int i, int j)
{
    element_t tmp = PQ(i);
    PQ(i) = PQ(j);
    PQ(j) = tmp;
}

static void heapify_up(PriorityQueue *pq)
{
    int curr = PQ_SIZE(pq) - 1, parent = PQ_PARENT(curr);

    while (curr != 0 && pq->cmp(PQ(curr), PQ(parent)))
    {
        heap_swap(pq, curr, parent);
        curr = parent;
        parent = PQ_PARENT(curr);
    }
}

static void heapify_down(PriorityQueue *pq)
{
    int curr = 0, next = 1, size = PQ_SIZE(pq);
    while (next < size)
    {
        if (PQ_RIGHT(curr) < size && pq->cmp(PQ(PQ_RIGHT(curr)), PQ(next)))
            ++next;
        if (pq->cmp(PQ(curr), PQ(next)))
            break;

        heap_swap(pq, curr, next);
        curr = next;
        next = PQ_LEFT(curr);
    }
}

void PriorityQueuePush(PriorityQueue *pq, element_t element)
{
    // Insert element
    push_back(pq->arr, element);
    // Fix heap
    heapify_up(pq);
}

void PriorityQueuePop(PriorityQueue *pq)
{
    // Swap first element with last
    heap_swap(pq, 0, PQ_SIZE(pq) - 1);
    // Remove min element
    pop_back(pq->arr);
    // Fix heap
    heapify_down(pq);
}

element_t PriorityQueueGetMin(PriorityQueue *pq)
{
    return PQ(0);
}

bool PriorityQueueEmpty(PriorityQueue *pq) 
{
    return pq->arr->size == 0;
}

void DestroyPriorityQueue(PriorityQueue *pq)
{
    DestroyDynamicArray(pq->arr);
    free(pq);
}
