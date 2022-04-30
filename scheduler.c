#define scheduler_c
#include <errno.h>
#include "headers.h"
#include "process_data.h"
#include "IO.h"
#include "./DataStructures/Dynamic_Array.h"
#include "./SchedulingAlgorithms/HPF.h"
#include "./SchedulingAlgorithms/SRTN.h"
#define PROCESS_TABLE_INITIAL_CAPACITY 31

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN process *CurrentProcess;
EXTERN int time_after = 0;
EXTERN FILE *logFile, *perfFile;
DynamicArray *ProcessTable; // Might need to change to hashtable

struct msgBuffer msg;
int msgid = 0;
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

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    signal(SIGURG, GenerationFinalize);
    signal(SIGUSR2, NewProcessFinalize);
    signal(SIGCHLD, ProcessTermination);

    // Make SIGUSR1 block SIGUSR2 & SIGCHLD
    struct sigaction setup_action;
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR2);
    sigaddset(&block_mask, SIGCHLD);
    setup_action.sa_handler = NewProcess;
    setup_action.sa_mask = block_mask;
    setup_action.sa_flags = 0;
    sigaction(SIGUSR1, &setup_action, NULL);

    initClk();
    msgid = atoi(argv[1]);
    printf("Scheduler's msgid: %d, Algorithm: %s\n", msgid, argv[2]);
    initialize(atoi(argv[2]));
    printf("Initialized!\n");
    int time_before = -1;
    time_after = getClk();
    while (GenerationRunning || CurrentProcess != NULL)
    {
        while (time_after <= time_before)
        {
            time_after = getClk();
        }
        time_before = getClk();
        printf("CLK: %d\n", time_after);
        if (SchedulingTimeSlotHandler != NULL)
        {
            SchedulingTimeSlotHandler();
        }
    }

    SchedulingDestroy(ReadyQueue);
    // upon termination release the clock resources.
    destroyClk(true);
    return 0;
}

void NewProcess(int signum)
{
    printf("Attempting to recieve msg \n");
    msgrcv(msgid, &msg, sizeof(msg.p), 13, 0);
    printf("Process recieved: %d\n", msg.p.pid);
         
    process *p = malloc(sizeof(process));
    memcpy(p, &(msg.p), sizeof(process));
    push_back(ProcessTable, p);
    SchedulingNewProcessHandler(ReadyQueue, p);
}

void NewProcessFinalize(int signum)
{
    time_after = getClk();
    SchedulingNewProcessFinalizationHandler(ReadyQueue);  
}

void GenerationFinalize(int signum) 
{
    GenerationRunning = false;
}

void ProcessTermination(int signum)
{
    time_after = getClk();
    SchedulingTerminationHandler(ReadyQueue);
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
        SchedulingNewProcessFinalizationHandler = SRTNNewProcessFinalizationHandler; // ?!!!!!!
        SchedulingTerminationHandler = SRTNTerminationHandler;
        SchedulingTimeSlotHandler = NULL;
        SchedulingDestroy = SRTNDestroy;
        break;

    case RR:
        // get quantum
        // quantum = atoi(argv[3]);
        break;
    default:
        break;
    }

    ReadyQueue = SchedulingInit(NULL);

    msg.mtype = 13;

    initializeOut(&logFile, &perfFile);
}

void clearResources(int signum)
{
    printf("Clearing scheduler resources...\n");
    freeOut(logFile, perfFile);
}