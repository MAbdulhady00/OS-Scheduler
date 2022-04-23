#pragma once

// Information about a process (PCB)
typedef struct
{
    int pid;
    int priority;
    int arrivalTime;
    int runningTime;
    int* remainingTime; // sabry update process block
    int shmid_process;  // sabry update process block
} process;