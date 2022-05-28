#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include "HPF.h"
#include "../DataStructures/Priority_Queue.h"
#include "../IO.h"
#include "../process_data.h"

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process *CurrentProcess;
EXTERN FILE *logFile;
EXTERN int time;

bool HPFCmp(process *p1, process *p2)
{
    if (p1->priority < p2->priority)
        return true;

    if (p1->priority == p2->priority && p1->arrivalId < p2->arrivalId)
        return true;

    return false;
}

void *HPFInit(void *args)
{
    return CreatePriorirtyQueue(HPFCmp);
}

void HPFNewProcessHandler(void *ReadyQueue, process *p)
{
    PriorityQueuePush((PriorityQueue *)ReadyQueue, p);
}

void HPFNewProcessFinalizationHandler(void *ReadyQueue)
{
    if(CurrentProcess != NULL || PriorityQueueEmpty((PriorityQueue *)ReadyQueue))
        return;
    process *p = PriorityQueueGetMin((PriorityQueue *)ReadyQueue);
    PriorityQueuePop((PriorityQueue *)ReadyQueue);
    create_process(p);
    p->waitTime = time - p->arrivalTime;
    CurrentProcess = p;
    logProcess(logFile, p, time);
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

    return p;
}

void HPFTerminationHandler(void *ReadyQueue)
{
    printf("Process %d terminated!\n", CurrentProcess->pid);
    CurrentProcess->state = FINISHED;
    CurrentProcess->finishTime = time;
    logProcess(logFile, CurrentProcess, time);
    CurrentProcess = NULL;
    // if (PriorityQueueEmpty((PriorityQueue *)ReadyQueue)) 
    //    return;     

    // process *p = PriorityQueueGetMin((PriorityQueue *)ReadyQueue);
    // PriorityQueuePop((PriorityQueue *)ReadyQueue);
    // create_process(p);
    // p->waitTime = time_after - p->arrivalTime;
    // CurrentProcess = p;
    // logProcess(logFile, p, time_after);
}

void HPFDestroy(void *ReadyQueue)
{
    DestroyPriorityQueue((PriorityQueue *)ReadyQueue);
}