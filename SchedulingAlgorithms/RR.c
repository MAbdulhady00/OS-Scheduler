#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <assert.h>
#include "RR.h"
#include "../DataStructures/Circular_Queue.h"
#include "../IO.h"

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process *CurrentProcess;
EXTERN int time_after;
EXTERN FILE *logFile;

int quantum_size;
int curr_quantum = 0;
/*
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
*/

void *RRInit(void *args)
{
    return CreateCircularQueue();
}

void SetQuantum(int q)
{
    quantum_size = q;
}

void RRNewProcessHandler(void *ReadyQueue, process *p)
{
    RREnqueue(ReadyQueue, p);
}

void RRNewProcessFinalizationHandler(void *ReadyQueue)
{
    //printf("Starting A Process\n");
    process *p = RRRunNext(ReadyQueue);
    create_process(p);
    p->waitTime = p->waitTime == 0 ? time_after - p->arrivalTime : p->waitTime;
    CurrentProcess = p;
    logProcess(logFile, p, time_after);
    //printf("Started A Process\n");
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
    return p;
}

void SwitchProcess(void *ReadyQueue)
{
    if (isCircularQueueEmpty((CircularQueue *)ReadyQueue) == false)
    {
        printf("Process %d Switched!\n", CurrentProcess->pid);
        // Stop Current Process
        CurrentProcess->state = STOPPED;
        logProcess(logFile, CurrentProcess, time_after);
        RREnqueue(ReadyQueue, CurrentProcess);

        // Start Next One
        process *p = RRRunNext(ReadyQueue);
        create_process(p);
        p->waitTime = p->waitTime == 0 ? time_after - p->arrivalTime : p->waitTime;
        CurrentProcess = p;
        logProcess(logFile, p, time_after);
    }
    else
    {
        curr_quantum = quantum_size - 1;
    }
}

void RRTimeSlotHandler(void *ReadyQueue)
{
    if (CurrentProcess == NULL)
    {
        return;
    }
    curr_quantum++;
    if (curr_quantum == quantum_size)
    {
        curr_quantum = 0;
        SwitchProcess(ReadyQueue);
    }
}

void RRTerminationHandler(void *ReadyQueue)
{
    //printf("Process %d terminated!\n", CurrentProcess->pid);
    CurrentProcess->state = FINISHED;
    CurrentProcess->finishTime = time_after;
    logProcess(logFile, CurrentProcess, time_after);
    shmctl(CurrentProcess->shmid_process, IPC_RMID, (struct shmid_ds *)0);
    CurrentProcess = NULL;
}

void RRDestroy(void *ReadyQueue)
{
    destroyCircularQueue((CircularQueue *)ReadyQueue);
}