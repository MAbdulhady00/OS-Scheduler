#include "headers.h"
#include "process.h"
#include "./SchedulingAlgorithms/HPF.h"
#define PROCESS_TABLE_SIZE 31

void create_process(process * forked_process);
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
void create_process(process * forked_process)
{
    int shmid = shmget(IPC_PRIVATE, 4, IPC_CREAT | 0644); // create shared memory for process remaining time
    
    forked_process->shmid_process = shmid;           // store shmid of process to be sent & deleted later
    void *shmaddr = shmat(shmid, (void *)0, 0);
    //int remain_time = *forked_process->remainingTime;
    forked_process->remainingTime = (int*)shmaddr;
    *forked_process->remainingTime =  forked_process->runningTime;
    forked_process->pid = fork();                    // store pid of process
    
    if(forked_process->pid  == -1)
    {
        printf("Error in forking");
    }
    if (forked_process->pid  == 0)
    {
        
        char str[20];
        sprintf(str,"%d",forked_process->shmid_process);         // copy shmid to str to be send to each process
        int x = execl("./process.out","./process.out",str,NULL); // create process
        if(x == -1)
        {
            printf("Error in execution");
        }
    }   

}