#include "headers.h"
#include "process.h"
#include "./SchedulingAlgorithms/HPF.h"
#define PROCESS_TABLE_SIZE 31

int main(int argc, char *argv[])
{
    initClk();
    process *ProcessTable[PROCESS_TABLE_SIZE];
    void *ReadyQueue;
    void *(*SchedulingInit)(void *);
    void (*SchedulingEnqueue)(void *, process *);
    process *(*SchedulingRunNext)(void *);
    void (*SchedulingTerminationHandler)(void *, process *);
    void (*SchedulingTimeSlotHandler)();
    void (*SchedulingDestroy)(void *);

    int algorithmType = atoi(argv[1]);
    switch (algorithmType)
    {
    case 0:
    //    SchedulingInit = HPFInit;
      //  SchedulingEnqueue = HPFEnqueue;
      //  SchedulingRunNext = HPFRunNext;
      //  SchedulingTerminationHandler = HPFTerminationHandler; // possibly null
      //  SchedulingTimeSlotHandler = NULL;
      //  SchedulingDestroy = HPFDestroy;

      //  ReadyQueue = SchedulingInit(NULL);
        break;

    default:
        break;
    }

    while (true) // loop
    {
        // if(message recieved from process generator)
        // add process to ready queue
        // Create p
        // SchedulingEnqueue(ReadyQueue, p);

        //
    }

    SchedulingDestroy(ReadyQueue);
    // upon termination release the clock resources.
    destroyClk(true);
}
