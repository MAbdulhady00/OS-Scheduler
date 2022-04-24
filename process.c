#include "headers.h"

void handler(int signum);
/* Modify this file as needed*/
int* remainingtime;

int main(int agrc, char * argv[])
{
    initClk();
    signal(SIGINT,handler);
    int shmid = atoi(argv[1]);
    
    void *shmaddr = shmat(shmid, (void *)0, 0);
    
    remainingtime = (int*)shmaddr;
    //printf("\nShared memory ID in process = %d\n", shmid);
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    int time_before = getClk(),time_after = getClk();
    while (*remainingtime > 0)
    {
        
        while(time_after >= time_before)
            time_before = getClk();
        *remainingtime = *remainingtime - 1;
        time_after = time_before;
        //sleep(1);
        //printf("from process: remaintime = %d\n",*remainingtime);
        // remainingtime = ??;
    }
    
    destroyClk(false);
    //kill(getppid(),SIGUSR1);
    return 0;
}
void handler(int signum)
{
    destroyClk(false);
    exit(0);
}