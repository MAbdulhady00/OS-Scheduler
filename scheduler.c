#define scheduler_c
#include <errno.h>
#include "headers.h"
#include "process_data.h"
#include "IO.h"
#include "./DataStructures/Dynamic_Array.h"
#include "./SchedulingAlgorithms/HPF.h"
#include "./SchedulingAlgorithms/SRTN.h"
#include "./SchedulingAlgorithms/RR.h"

#define PROCESS_TABLE_INITIAL_CAPACITY 31

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process *CurrentProcess = NULL;
EXTERN int time = 0;
EXTERN FILE *logFile, *perfFile;
DynamicArray *ProcessTable; // Might need to change to hashtable

struct msgBuffer msg;
int msgid = 0;
bool recievedFromGenerator = true;
bool GenerationRunning = true;

void *ReadyQueue;
void *(*SchedulingInit)(void *);
void (*SchedulingNewProcessHandler)(void *, process *);
void (*SchedulingNewProcessFinalizationHandler)(void *);
void (*SchedulingTerminationHandler)(void *);
void (*SchedulingTimeSlotHandler)();
void (*SchedulingDestroy)(void *);

void NewProcess(int signum);
void ProcessTermination(int signum);
void NewProcessFinalize(int signum);
void GenerationFinalize(int signum);
void clearResources(int signum);

typedef enum AlgorithmType
{
    HPF = 1,
    SRTN = 2,
    RR = 3,
} AlgorithmType;
void initialize(AlgorithmType);
int quantum = 0;

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    signal(SIGURG, GenerationFinalize);
    // signal(SIGUSR2, NewProcessFinalize);
    // signal(SIGALRM, ProcessTermination);

    // Make SIGUSR1 block SIGUSR2 & SIGCHLD
    // struct sigaction setup_action;
    // sigset_t block_mask;
    // sigemptyset(&block_mask);
    // sigaddset(&block_mask, SIGUSR2);
    // sigaddset(&block_mask, SIGCHLD);
    // setup_action.sa_handler = NewProcess;
    // setup_action.sa_mask = block_mask;
    // setup_action.sa_flags = 0;
    // sigaction(SIGUSR1, &setup_action, NULL);

    // struct sigaction setup_action2;
    // sigset_t block_mask2;
    // sigemptyset(&block_mask2);
    // sigaddset(&block_mask2, SIGUSR1);
    // sigaddset(&block_mask2, SIGCHLD);
    // setup_action.sa_handler = NewProcessFinalize;
    // setup_action.sa_mask = block_mask2;
    // setup_action.sa_flags = 0;
    // sigaction(SIGUSR2, &setup_action2, NULL);

    // struct sigaction setup_action3;
    // sigset_t block_mask3;
    // sigemptyset(&block_mask3);
    // sigaddset(&block_mask3, SIGUSR1);
    // sigaddset(&block_mask3, SIGUSR2);
    // setup_action.sa_handler = ProcessTermination;
    // setup_action.sa_mask = block_mask3;
    // setup_action.sa_flags = 0;
    // sigaction(SIGCHLD, &setup_action3, NULL);

    initClk();
    msgid = atoi(argv[1]);
    if (argc > 3)
    {
        quantum = atoi(argv[3]);
        printf("Scheduler's msgid: %d, Algorithm: %s, Quantum: %d\n", msgid, argv[2], quantum);
    }
    else
    {
        printf("Scheduler's msgid: %d, Algorithm: %s\n", msgid, argv[2]);
    }
    initialize(atoi(argv[2]));
    printf("Initialized!\n");
    int time_before, time_after;
    while (GenerationRunning || CurrentProcess != NULL)
    {
        time_before = getClk();
        time_after = getClk();
        while (GenerationRunning)
        {
            printf("Attempting to recieve msg \n");
            msgrcv(msgid, &msg, sizeof(msg.p), 0, 0);
            printf("recieved type: %ld\n", msg.mtype);
            if (msg.mtype <= 1)
                break;

            process *p = malloc(sizeof(process));

            memcpy(p, &(msg.p), sizeof(process));
            p->remainingTime = malloc(sizeof(int));
            *p->remainingTime = p->runningTime;

            push_back(ProcessTable, p);
            SchedulingNewProcessHandler(ReadyQueue, p);
        }

        if (SchedulingTimeSlotHandler != NULL)
        {
            SchedulingTimeSlotHandler(ReadyQueue);
        }
        
        SchedulingNewProcessFinalizationHandler(ReadyQueue);
        while (CurrentProcess != NULL && *CurrentProcess->remainingTime <= 0)
        {
            waitpid(CurrentProcess->pWaitId, NULL, 0);
            SchedulingTerminationHandler(ReadyQueue);
            SchedulingNewProcessFinalizationHandler(ReadyQueue);
        }
        
        


        while (time_after <= time_before)
        {
            time_after = getClk();
        }
        ++time;

        if(CurrentProcess != NULL)
            --*CurrentProcess->remainingTime;
        
        time_before = getClk();
        printf("CLK: %d\n", time);
    }

    SchedulingDestroy(ReadyQueue);
    // upon termination release the clock resources.
    // With true sent all other processes will die too
    destroyClk(true);
    return 0;
}

void NewProcess(int signum)
{
    return; // lol
}

void NewProcessFinalize(int signum)
{
    // time_after = getClk();
    // SchedulingNewProcessFinalizationHandler(ReadyQueue);
}

void GenerationFinalize(int signum)
{
    GenerationRunning = false;
}

void ProcessTermination(int signum)
{
    waitpid(CurrentProcess->pWaitId, NULL, 0);
    SchedulingTerminationHandler(ReadyQueue);
    if (!recievedFromGenerator)
        SchedulingNewProcessFinalizationHandler(ReadyQueue);
}

void initialize(AlgorithmType algorithmType)
{
    CurrentProcess = NULL;

    ProcessTable = CreateDynamicArray(PROCESS_TABLE_INITIAL_CAPACITY);

    switch (algorithmType)
    {
    case HPF:
        SchedulingInit = HPFInit;
        SchedulingNewProcessHandler = HPFNewProcessHandler;
        SchedulingNewProcessFinalizationHandler = HPFNewProcessFinalizationHandler;
        SchedulingTerminationHandler = HPFTerminationHandler;
        SchedulingTimeSlotHandler = NULL;
        SchedulingDestroy = HPFDestroy;
        break;

    case SRTN:
        SchedulingInit = SRTNInit;
        SchedulingNewProcessHandler = SRTNNewProcessHandler;
        SchedulingNewProcessFinalizationHandler = SRTNNewProcessFinalizationHandler;
        SchedulingTerminationHandler = SRTNTerminationHandler;
        SchedulingTimeSlotHandler = NULL;
        SchedulingDestroy = SRTNDestroy;
        break;

    case RR:
        SetQuantum(quantum);
        SchedulingInit = RRInit;
        SchedulingNewProcessHandler = RRNewProcessHandler;
        SchedulingNewProcessFinalizationHandler = RRNewProcessFinalizationHandler;
        SchedulingTerminationHandler = RRTerminationHandler;
        SchedulingTimeSlotHandler = RRTimeSlotHandler;
        SchedulingDestroy = RRDestroy;
        break;
    default:
        break;
    }

    ReadyQueue = SchedulingInit(NULL);

    initializeOut(&logFile, &perfFile);
}

void clearResources(int signum)
{
    printf("Clearing scheduler resources...\n");
    freeOut(logFile, perfFile);
    for (int i = 0; i < ProcessTable->size; i++)
    {
        process *p = ProcessTable->data[i];
        shmctl(p->shmid_process, IPC_RMID, (struct shmid_ds *)0);
    }
}