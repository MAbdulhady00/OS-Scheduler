#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "HPF.h"
#include "../DataStructures/Priority_Queue.h"

static bool cmp(process *p1, process *p2)
{
    if (p1->priority < p2->priority)
        return true;

    else if (p1->priority == p2->priority)
        return p1->arrivalTime < p2->arrivalTime;

    return false;
}

void *HPFInit(void *args)
{
    return CreatePriorirtyQueue(cmp);
}

void HPFEnqueue(void *ReadyQueue, process *p)
{
    PriorityQueuePush((PriorityQueue *)ReadyQueue, p);
}

process *HPFRunNext(void *ReadyQueue)
{
    if (PriorityQueueEmpty((PriorityQueue *)ReadyQueue))
        return NULL;
    process *p = PriorityQueueGetMin((PriorityQueue *)ReadyQueue);
    PriorityQueuePop((PriorityQueue *)ReadyQueue);
    // TODO: FORK next process possibly need to implement a new function as this will be shared among all algorithms

    return p;
}
void HPFTerminationHandler(void *ReadyQueue, process *p)
{
    // process will already be popped anyway
}

void HPFDestroy(void *ReadyQueue)
{
    DestroyPriorityQueue((PriorityQueue *)ReadyQueue);
}