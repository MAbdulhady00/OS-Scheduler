#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "RR.h"
#include "../DataStructures/Circular_Queue.h"

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process* CurrentProcess;

int quantum_size;
int curr_quantum;

void *RRInit(void *args)
{
    quantum_size = *((int*)args);
    curr_quantum = quantum_size;
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
    process *p = peekCircularQueue((CircularQueue *)ReadyQueue);

    // TODO: FORK next process possibly need to implement a new function as this will be shared among all algorithms

    return p;
}

void RRClkHandler(void * ReadyQueue)
{
    
}

void RRTerminationHandler(void *ReadyQueue, process *p)
{
    curr_quantum = quantum_size;
}

void RRDestroy(void *ReadyQueue)
{
    destroyCircularQueue((CircularQueue *)ReadyQueue);
}