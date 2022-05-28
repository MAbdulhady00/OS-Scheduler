#pragma once

// Information about a process (PCB)
enum pState
{
    STARTED = 0,
    RESUMED = 1,
    STOPPED = 2,
    FINISHED = 3
};
typedef struct
{
    int pid;
    int priority;
    int arrivalTime;
    int runningTime;
    int remainingTime;
    int shmid_process;  
    int waitTime;       
    int finishTime;     
    enum pState state;  
    int pWaitId;
    int arrivalId;
    int memsize;
    int address_position;
} process;

void create_process(process * forked_process);
