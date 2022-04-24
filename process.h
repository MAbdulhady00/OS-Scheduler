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
    int *remainingTime; // sabry update process block
    int shmid_process;  // sabry update process block
    int waitTime;       // Kamal update
    int finishTime;     // kamal update -> to calcualte TA Time & WTA
    enum pState state;  // kamal update 
} process;