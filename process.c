#include "headers.h"

void handler(int signum);
void handler_continue(int signum);
int time_before = 0,time_after = 0;
/* Modify this file as needed*/
int* remainingtime;

int main(int agrc, char * argv[])
{
    initClk();

    int shmid = atoi(argv[1]);
    
    void *shmaddr = shmat(shmid, (void *)0, 0);
    signal(SIGCONT,handler_continue);
    remainingtime = (int*)shmaddr;
    //printf("\nShared memory ID in process = %d\n", shmid);
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    time_before = getClk();
    time_after = getClk();
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
    kill(getppid(),SIGALRM);
    return 0;
}
void handler_continue(int signum)
{
    time_after = getClk();
    time_before = getClk();
}
void handler(int signum)
{
    destroyClk(false);
    exit(0);
}
