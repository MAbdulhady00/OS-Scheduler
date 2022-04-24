#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "SRTN.h"
#include "../DataStructures/Priority_Queue.h"

static bool compare_remain_time(process *p1, process *p2)
{
    if (*p1->remainingTime < *p2->remainingTime)
        return true;

    else if (*p1->remainingTime == *p2->remainingTime)
        return p1->arrivalTime < p2->arrivalTime;

    return false;
}

void *SRTNInit(void *args)
{
    return CreatePriorirtyQueue(compare_remain_time);
}

void SRTNEnqueue(void *ReadyQueue, process *p)
{
    PriorityQueuePush((PriorityQueue *)ReadyQueue, p);
}

process *SRTNRunNext(void *ReadyQueue)
{
    if (PriorityQueueEmpty((PriorityQueue *)ReadyQueue))
        return NULL;
    process *p = PriorityQueueGetMin((PriorityQueue *)ReadyQueue);
    PriorityQueuePop((PriorityQueue *)ReadyQueue);
    // TODO: FORK next process possibly need to implement a new function as this will be shared among all algorithms

    return p;
}
void SRTNTerminationHandler(void *ReadyQueue, process *p)
{
    // process will already be popped anyway
}

void SRTNDestroy(void *ReadyQueue)
{
    DestroyPriorityQueue((PriorityQueue *)ReadyQueue);
}
bool SRTNEmpty(void *ReadyQueue)
{
    return PriorityQueueEmpty((PriorityQueue *) ReadyQueue) ;
}