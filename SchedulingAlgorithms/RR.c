#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <assert.h>
#include "RR.h"
#include "../DataStructures/Circular_Queue.h"
#include "../IO.h"
#include <signal.h>

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
    // printf("Starting A Process\n");

    if (CurrentProcess != NULL || isCircularQueueEmpty((CircularQueue*) ReadyQueue))
    {
        return;
    }
    process *p = RRRunNext(ReadyQueue);
    curr_quantum = 0;
    if (p->state == STOPPED)
    {
        kill(p->pWaitId, SIGCONT);
        p->state = RESUMED;
        printf("Process %d Resumed! remain time %d\n", p->pid, *p->remainingTime);
        p->waitTime = time_after - p->arrivalTime - p->runningTime + *p->remainingTime;
        logProcess(logFile, p, time_after);
        CurrentProcess = p;
    }
    else
    {
        create_process(p);
        p->state = STARTED;
        p->waitTime = p->waitTime == 0 ? time_after - p->arrivalTime : p->waitTime;
        CurrentProcess = p;
        p->waitTime = time_after - p->arrivalTime - p->runningTime + *p->remainingTime;
        logProcess(logFile, p, time_after);
    }

    // printf("Started A Process\n");
}

void RREnqueue(void *ReadyQueue, process *p)
{
    enqueueCircularQueue((CircularQueue *)ReadyQueue, p);
    printf("######enqueued process id = %d\n", p->pid);
}

process *RRRunNext(void *ReadyQueue)
{
    if (isCircularQueueEmpty((CircularQueue *)ReadyQueue))
        return NULL;
    process *p = dequeueCircularQueue((CircularQueue *)ReadyQueue);
    printf("######dequeued process id = %d\n", p->pid);
    return p;
}

void SwitchProcess(void *ReadyQueue)
{

    if (isCircularQueueEmpty((CircularQueue *)ReadyQueue) == false)
    {
        if (CurrentProcess != NULL)
        {
            if (*CurrentProcess->remainingTime > 0)
                RREnqueue(ReadyQueue, CurrentProcess);
            else
            {
                CurrentProcess->state = FINISHED;
                // logProcess(logFile, CurrentProcess, time_after);
                return;
            }

            printf("Process %d Switched!\n", CurrentProcess->pid);
            // Stop Current Process
            kill((pid_t)CurrentProcess->pWaitId, SIGSTOP);
            CurrentProcess->state = STOPPED;
            logProcess(logFile, CurrentProcess, time_after);
            printf("Process %d Stopped!remain time %d\n", CurrentProcess->pid, *CurrentProcess->remainingTime);
        }

        // Start Next One
        // process *p = RRRunNext(ReadyQueue);
        // create_process(p);
        // p->waitTime = p->waitTime == 0 ? time_after - p->arrivalTime : p->waitTime;
        // CurrentProcess = p;
        // logProcess(logFile, p, time_after);
        process *p = RRRunNext(ReadyQueue);
        if (p->state == STOPPED)
        {
            kill(p->pWaitId, SIGCONT);
            p->state = RESUMED;
            printf("Process %d Resumed! remain time %d\n", p->pid, *p->remainingTime);
            logProcess(logFile, p, time_after);
            CurrentProcess = p;
        }
        else
        {
            create_process(p);
            p->state = STARTED;
            p->waitTime = p->waitTime == 0 ? time_after - p->arrivalTime : p->waitTime;
            CurrentProcess = p;
            logProcess(logFile, p, time_after);
        }
    }
    else
    {
        // curr_quantum = quantum_size - 1;
        // curr_quantum = 0;
    }
}

void RRTimeSlotHandler(void *ReadyQueue)
{
    if (CurrentProcess == NULL)
    {
        curr_quantum = 0;
        SwitchProcess(ReadyQueue);
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
    printf("Process %d terminated!\n", CurrentProcess->pid);
    CurrentProcess->state = FINISHED;
    CurrentProcess->finishTime = time_after;
    CurrentProcess->waitTime = time_after - CurrentProcess->arrivalTime - CurrentProcess->runningTime + *CurrentProcess->remainingTime;
    logProcess(logFile, CurrentProcess, time_after);
    shmctl(CurrentProcess->shmid_process, IPC_RMID, (struct shmid_ds *)0);
    CurrentProcess = NULL;
    curr_quantum = 0;
    SwitchProcess(ReadyQueue);
}

void RRDestroy(void *ReadyQueue)
{
    destroyCircularQueue((CircularQueue *)ReadyQueue);
}