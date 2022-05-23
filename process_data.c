#include "process_data.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

void create_process(process * forked_process)
{
    if (1)
    {
        int shmid = shmget(IPC_PRIVATE, 4, IPC_CREAT | 0644); // create shared memory for process remaining time
        
        forked_process->shmid_process = shmid;           // store shmid of process to be sent & deleted later
        void *shmaddr = shmat(shmid, (void *)0, 0);
        //int remain_time = *forked_process->remainingTime;
        forked_process->remainingTime = (int*)shmaddr;
        *forked_process->remainingTime =  forked_process->runningTime;
    }
    
    printf("Creating process for %d\n", forked_process->pid);
    fflush(stdout);
    forked_process->state = STARTED;
    forked_process->pWaitId = fork();                    // store pid of process
    
    if(forked_process->pWaitId  == -1)
    {
        printf("Error in forking");
    }
    if (forked_process->pWaitId  == 0)
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