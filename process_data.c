#include "process_data.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

void create_process(process * forked_process)
{   
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
        sprintf(str,"%d", forked_process->remainingTime);         // copy remaining time
        int x = execl("./process.out","./process.out",str,NULL); // create process
        if(x == -1)
        {
            printf("Error in execution");
        }
    }   

}