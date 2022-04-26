#define scheduler_c
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
int msgid;

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
    signal(SIGUSR2, NewProcessFinalize);
    signal(SIGCHLD, ProcessTermination);

    // Make SIGUSR1 block SIGUSR2
    struct sigaction setup_action;
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR2);
    setup_action.sa_handler = NewProcess;
    setup_action.sa_mask = block_mask;
    setup_action.sa_flags = 0;
    sigaction(SIGUSR1, &setup_action, NULL);

    initClk();
    msgid = atoi(argv[1]);
    printf("Scheduler's msgid: %s, Algorithm: %s\n", argv[1], argv[2]);
    initialize(atoi(argv[2]));
    printf("Initialized!\n");
    int time_before = -1;
    time_after = getClk();
    while (true)
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
}

void NewProcess(int signum)
{
    printf("Attempting to recieve msg \n");
    msgrcv(msgid, &msg, sizeof(msg), 13, 0);
    printf("Process recieved: %d\n", msg.p.pid);

    process *p = malloc(sizeof(process));
    memcpy(p, &(msg.p), sizeof(process));
    push_back(ProcessTable, p);
    SchedulingNewProcessHandler(ReadyQueue, p);
}

void NewProcessFinalize(int signum)
{
    SchedulingNewProcessFinalizationHandler(ReadyQueue);
    
}

void ProcessTermination(int signum)
{
    //wait(NULL);
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

    initializeOut(&logFile, &perfFile);
}

void clearResources(int signum)
{
    freeOut(logFile, perfFile);
}