#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <signal.h>
#include <sys/shm.h>
#include "SRTN.h"
#include "../DataStructures/Priority_Queue.h"
#include "../IO.h"

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process* CurrentProcess;
EXTERN FILE *logFile;
EXTERN int time_after;

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
        kill((pid_t)CurrentProcess->pWaitId, SIGSTOP);
        //CurrentProcess->state = STOPPED;
        printf("Process %d Stopped!\n", CurrentProcess->pid);
        logProcess(logFile, CurrentProcess, time_after);
        create_process(p);
        PriorityQueuePush((PriorityQueue *)ReadyQueue, CurrentProcess);
        CurrentProcess = p;
        printf("Process %d Started!\n", CurrentProcess->pid);
        p->waitTime = time_after - p->arrivalTime;
        logProcess(logFile, p, time_after);
    }
    else if(CurrentProcess != NULL) {
        //TODO: LOG ENQUEUE
        PriorityQueuePush((PriorityQueue *)ReadyQueue, p);
    }
    else {
        //TODO LOG STARTED
        create_process(p);
        p->state = STARTED;
        CurrentProcess = p;
        p->waitTime = time_after - p->arrivalTime;
        printf("Process %d Started!\n", CurrentProcess->pid);
        logProcess(logFile, p, time_after);
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
    if (p->state != STOPPED)
    {
        kill(p->pWaitId,SIGCONT);
        p->state = RESUMED;
        printf("Process %d Resumed!\n", CurrentProcess->pid);
        //p->waitTime = time_after - p->arrivalTime;
        logProcess(logFile, p, time_after);
    }
    else
    {
        create_process(p);
        p->state = STARTED;
        printf("Process %d Started!\n", CurrentProcess->pid);
        p->waitTime = time_after - p->arrivalTime;
        logProcess(logFile, p, time_after);
    }
    CurrentProcess = p;
    PriorityQueuePop((PriorityQueue *)ReadyQueue);
    // TODO: FORK next process possibly need to implement a new function as this will be shared among all algorithms

    return p;
}

void SRTNNewProcessFinalizationHandler(void *ReadyQueue)
{
    if(CurrentProcess != NULL)
        return;
    process *p = PriorityQueueGetMin((PriorityQueue *)ReadyQueue);
    PriorityQueuePop((PriorityQueue *)ReadyQueue);
    if (p->state != STOPPED)
        {
            kill(p->pWaitId,SIGCONT);
            p->state = RESUMED;
            //p->waitTime = time_after - p->arrivalTime;
            printf("Process %d Resumed!\n", CurrentProcess->pid);
            logProcess(logFile, p, time_after);
        }
        else
        {
            create_process(p);
            p->state = STARTED;
            p->waitTime = time_after - p->arrivalTime;
            printf("Process %d Started!\n", CurrentProcess->pid);
            logProcess(logFile, p, time_after);
        }
        CurrentProcess = p;
}
void SRTNTerminationHandler(void *ReadyQueue)
{
    printf("Process %d terminated!\n", CurrentProcess->pid);
    CurrentProcess->state = FINISHED;
    CurrentProcess->finishTime = time_after;
    logProcess(logFile, CurrentProcess, time_after);
    shmctl(CurrentProcess->shmid_process, IPC_RMID, (struct shmid_ds *)0);
    CurrentProcess = NULL;
    if (PriorityQueueEmpty((PriorityQueue *)ReadyQueue)) {
        CurrentProcess = NULL;
    }
    else {

        process * p = PriorityQueueGetMin((PriorityQueue *)ReadyQueue);
        PriorityQueuePop((PriorityQueue *)ReadyQueue);
        if (p->state != STOPPED)
        {
            kill(p->pWaitId,SIGCONT);
            p->state = RESUMED;
            printf("Process %d Resumed!\n", CurrentProcess->pid);
            //p->waitTime = time_after - p->arrivalTime;
            logProcess(logFile, p, time_after);
        }
        else
        {
            create_process(p);
            p->state = STARTED;
            printf("Process %d Started!\n", CurrentProcess->pid);
            p->waitTime = time_after - p->arrivalTime;
            logProcess(logFile, p, time_after);
        }
        CurrentProcess = p;
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