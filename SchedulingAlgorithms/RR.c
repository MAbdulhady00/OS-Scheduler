#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "RR.h"
#include "../DataStructures/Circular_Queue.h"

void *RRInit(void *args)
{
    return CreateCircularQueue();
}

void RREnqueue(void *ReadyQueue, process *p)
{
    enqueueCircularQueue((CircularQueue *)ReadyQueue, p);
}

process *RRRunNext(void *ReadyQueue)
{
    if (isCircularQueueEmpty((CircularQueue *)ReadyQueue))
        return NULL;
    process *p = dequeueCircularQueue((CircularQueue *)ReadyQueue);

    // TODO: FORK next process possibly need to implement a new function as this will be shared among all algorithms

    return p;
}

void RRTerminationHandler(void *ReadyQueue, process *p)
{
    // process will already be popped anyway
}

void RRDestroy(void *ReadyQueue)
{
    destroyCircularQueue((CircularQueue *)ReadyQueue);
}
bool RREmpty(void *ReadyQueue)
{
    return isCircularQueueEmpty((CircularQueue *) ReadyQueue) ;
}