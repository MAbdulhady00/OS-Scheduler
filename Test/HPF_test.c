#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../process.h"
#include "../SchedulingAlgorithms/HPF.h"

const int sz = 4;

int main(int argc, char *argv[])
{
    bool visualtest = true;
    process *p;
    process *ProcessTable[sz + 1];
    int atime = 0;
    for (int i = 1; i <= sz; i++)
    {
        p = malloc(sizeof(process));
        p->pid = i;
        atime += rand() % (11); // processes arrives in order
        p->arrivalTime = atime;
        p->runningTime = rand() % (30) + 1;
        p->priority = rand() % (11);
        ProcessTable[i] = p;
    }
    int clk = 0;
    int i = 1;
    p = NULL;
    void *ReadyQueue;

    ReadyQueue = HPFInit(NULL);
    while (i <= sz || p != NULL)
    {
        while (i <= sz && ProcessTable[i]->arrivalTime == clk)
        {
            HPFEnqueue(ReadyQueue, ProcessTable[i]);
            if (visualtest)
                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d Enqueued\n", clk,
                       ProcessTable[i]->pid, ProcessTable[i]->priority, ProcessTable[i]->arrivalTime, ProcessTable[i]->runningTime);

            ++i;
        }

        if (p != NULL && --p->runningTime == 0)
        {
            HPFTerminationHandler(ReadyQueue, p);
            if (visualtest)
                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d ended\n", clk,
                       p->pid, p->priority, p->arrivalTime);
            p = NULL;
        }

        if (p == NULL)
        {
            p = HPFRunNext(ReadyQueue);
            if (visualtest && p != NULL)
                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d Started\n", clk,
                       p->pid, p->priority, p->arrivalTime, p->runningTime);
        }

        ++clk;
    }
}