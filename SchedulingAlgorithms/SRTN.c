#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <signal.h>
#include "SRTN.h"
#include "../DataStructures/Priority_Queue.h"

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process* CurrentProcess;

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

void SRTNNewProcessHandler(void* ReadyQueue, process* p) {
    if(CurrentProcess != NULL && p->runningTime < *(CurrentProcess->remainingTime)) {
        CurrentProcess->state = STOPPED;
        //TODO: LOG PROCESS STOPPED
        kill((pid_t)CurrentProcess->pid, SIGSTOP);
        create_process(p);
        PriorityQueuePush((PriorityQueue *)ReadyQueue, CurrentProcess);
        CurrentProcess = p;
    }
    else if(CurrentProcess != NULL) {
        //TODO: LOG ENQUEUE
        PriorityQueuePush((PriorityQueue *)ReadyQueue, p);
    }
    else {
        //TODO LOG STARTED
        create_process(p);
        CurrentProcess = p;
    }
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

void SRTNTerminationHandler(void *ReadyQueue)
{
    if (PriorityQueueEmpty((PriorityQueue *)ReadyQueue)) {
        CurrentProcess = NULL;
    }
    else {
        CurrentProcess = PriorityQueueGetMin((PriorityQueue *)ReadyQueue);
        PriorityQueuePop((PriorityQueue *)ReadyQueue);
    }
}

void SRTNDestroy(void *ReadyQueue)
{
    DestroyPriorityQueue((PriorityQueue *)ReadyQueue);
}
bool SRTNEmpty(void *ReadyQueue)
{
    return PriorityQueueEmpty((PriorityQueue *) ReadyQueue) ;
}