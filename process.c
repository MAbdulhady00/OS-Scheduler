#include "headers.h"

void handler(int signum);
void handler_continue(int signum);
int time_before = 0,time_after = 0;

int main(int agrc, char * argv[])
{
    initClk();

    int remainingTime = atoi(argv[1]);
    time_before = getClk();
    time_after = getClk();
    while (remainingTime > 0)
    {     
        while(time_after >= time_before)
            time_before = getClk();

        --remainingTime;
        time_after = time_before;
    }
    
    shmdt(shmaddr);
    destroyClk(false);
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
