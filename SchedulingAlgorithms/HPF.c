#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include "HPF.h"
#include "../DataStructures/Priority_Queue.h"
#include "../IO.h"

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process *CurrentProcess;
EXTERN FILE *logFile;
EXTERN int time_after;

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
    p->waitTime = time_after - p->arrivalTime;
    CurrentProcess = p;
    logProcess(logFile, p, time_after);
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
    CurrentProcess->finishTime = time_after;
    logProcess(logFile, CurrentProcess, time_after);
    shmctl(CurrentProcess->shmid_process, IPC_RMID, (struct shmid_ds *)0);
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