#define scheduler_c
#include <math.h>
#include "headers.h"
#include "process_data.h"
#include "IO.h"
#include "./Memory/Memory_Manager.h"
#include "./DataStructures/Dynamic_Array.h"
#include "./DataStructures/Priority_Queue.h"
#include "./SchedulingAlgorithms/HPF.h"
#include "./SchedulingAlgorithms/SRTN.h"
#include "./SchedulingAlgorithms/RR.h"

#define PROCESS_TABLE_INITIAL_CAPACITY 31
#define WAITING_QUEUE_SIZE 9

#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN SortedLinkedList** MEM;
EXTERN int max_available;
EXTERN process *CurrentProcess = NULL;
EXTERN int time = 0;
EXTERN FILE *logFile, *perfFile, *memlogFile;
EXTERN double sumWTA = 0, sumWTASq = 0, sumWaiting = 0;
EXTERN int nProcess = 0, sumIdleTime = 0;;
DynamicArray *ProcessTable; // Might need to change to hashtable
//LinkedQueue* Waiting_Queue;
struct msgBuffer msg;
int msgid = 0;
bool recievedFromGenerator = true;
bool GenerationRunning = true;

void *ReadyQueue;
void *WaitingQueue[WAITING_QUEUE_SIZE];
void *(*SchedulingInit)(void *);
void (*SchedulingNewProcessHandler)(void *, process *);
void (*SchedulingAttemptRunNext)(void *);
void (*SchedulingTerminationHandler)(void *);
bool (*SchedulingCompare)(process*, process*);
void (*SchedulingTimeSlotHandler)();
void (*SchedulingDestroy)(void *);

void NewProcess(int signum);
void ProcessTermination(int signum);
void NewProcessFinalize(int signum);
void GenerationFinalize(int signum);
void clearResources(int signum);


void WaitingNewProcessHandler(process* p);
process* WaitingGetAvailableProcess();

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
    process* AvailableProcess = NULL;
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

            // push_back(ProcessTable, p);
            WaitingNewProcessHandler(p);
        }

        AvailableProcess = WaitingGetAvailableProcess();
        while(AvailableProcess != NULL) {
            //Process status: waiting ---> ready
            SchedulingNewProcessHandler(ReadyQueue, AvailableProcess);
            AvailableProcess->address_position = allocate_MEM(AvailableProcess->memsize);
            printf("Allocated mem for %d, pos: %d, memsize: %d\n", AvailableProcess->pid, AvailableProcess->address_position, AvailableProcess->memsize);
            logMEM(memlogFile, AvailableProcess, time);
            AvailableProcess = WaitingGetAvailableProcess();
        }

        if (SchedulingTimeSlotHandler != NULL)
        {
            SchedulingTimeSlotHandler(ReadyQueue);
        }
        
        SchedulingAttemptRunNext(ReadyQueue);
        while (CurrentProcess != NULL && CurrentProcess->remainingTime <= 0)
        {
            waitpid(CurrentProcess->pWaitId, NULL, 0);
            deallocate_MEM(CurrentProcess->address_position, CurrentProcess->memsize);
            CurrentProcess->state = FINISHED;
            logMEM(memlogFile, CurrentProcess, time);
            SchedulingTerminationHandler(ReadyQueue);

            AvailableProcess = WaitingGetAvailableProcess();
            if(AvailableProcess != NULL)  {
                SchedulingNewProcessHandler(ReadyQueue, AvailableProcess);
                AvailableProcess->address_position = allocate_MEM(AvailableProcess->memsize);
                printf("Allocated mem for %d, pos: %d, memsize: %d\n", AvailableProcess->pid, AvailableProcess->address_position, AvailableProcess->memsize);
                logMEM(memlogFile, AvailableProcess, time);
            }
            
            SchedulingAttemptRunNext(ReadyQueue);
        }

        if(CurrentProcess == NULL)
            ++sumIdleTime;

        while (time_after <= time_before)
        {
            time_after = getClk();
        }
        ++time;

        if(CurrentProcess != NULL)
            --CurrentProcess->remainingTime;
        
        time_before = getClk();
        printf("CLK: %d\n", time);
    }
    logPerf(perfFile);
    SchedulingDestroy(ReadyQueue);
    // upon termination release the clock resources.
    // With true sent all other processes will die too
    destroyClk(true);
    return 0;
}

void GenerationFinalize(int signum)
{
    GenerationRunning = false;
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
        SchedulingAttemptRunNext = HPFNewProcessFinalizationHandler;
        SchedulingTerminationHandler = HPFTerminationHandler;
        SchedulingCompare = HPFCmp;
        SchedulingTimeSlotHandler = NULL;
        SchedulingDestroy = HPFDestroy;
        break;

    case SRTN:
        SchedulingInit = SRTNInit;
        SchedulingNewProcessHandler = SRTNNewProcessHandler;
        SchedulingAttemptRunNext = SRTNNewProcessFinalizationHandler;
        SchedulingTerminationHandler = SRTNTerminationHandler;
        SchedulingCompare = SRTNCmp;
        SchedulingTimeSlotHandler = NULL;
        SchedulingDestroy = SRTNDestroy;
        break;

    case RR:
        SetQuantum(quantum);
        SchedulingInit = RRInit;
        SchedulingNewProcessHandler = RRNewProcessHandler;
        SchedulingAttemptRunNext = RRNewProcessFinalizationHandler;
        SchedulingTerminationHandler = RRTerminationHandler;
        SchedulingCompare = RRCmp;
        SchedulingTimeSlotHandler = RRTimeSlotHandler;
        SchedulingDestroy = RRDestroy;
        break;
    default:
        break;
    }

    ReadyQueue = SchedulingInit(NULL);

    //Initialize waiting queues
    for(int i = 0; i < WAITING_QUEUE_SIZE; ++i)
        WaitingQueue[i] = CreatePriorirtyQueue(SchedulingCompare);

    //Initialize buddy memory allocator
    MEM_init();

    initializeOut(&logFile, &perfFile, &memlogFile);
}

void WaitingNewProcessHandler(process* p) {
    int index = ceil(log2(p->memsize));
    if(p->memsize == 0)
        index = 0;

    PriorityQueuePush(WaitingQueue[index], p);
}

process* WaitingGetAvailableProcess() {
    setmaxavailable();
    int n = max_available + 3;
    if(n > 8)
        n = 8;

    process* available = NULL;
    process* curr;
    int pos = -1;
    for(int i = 0; i <= n; ++i) {
        if(!PriorityQueueEmpty(WaitingQueue[i])) {
            curr = PriorityQueueGetMin(WaitingQueue[i]);
            if(available == NULL) {
                available = curr;
                pos = i;
            }
            //Get best process in waiting that has size <= maxavailable
            else if(SchedulingCompare(curr, available)) {
                available = curr;
                pos = i;
            }
        }
    }

    if(pos >= 0) {
        PriorityQueuePop(WaitingQueue[pos]);
    }

    return available;
}

void clearResources(int signum)
{
    printf("Clearing scheduler resources...\n");
    freeOut(logFile, perfFile, memlogFile);
}