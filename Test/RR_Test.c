#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../process.h"
#include "../SchedulingAlgorithms/RR.h"

const int sz = 4;

int main(int argc, char *argv[])
{
    bool visualtest = true;
    process *p;
    process *ProcessTable[sz + 1];
    int atime = 0;
    printf("Start RR\n");
    for (int i = 1; i <= sz; i++)
    {
        p = malloc(sizeof(process));
        p->pid = i;
        atime += rand() % (11); // processes arrives in order
        p->arrivalTime = atime;
        p->runningTime = rand() % (30) + 1;
        p->remainingTime = malloc(sizeof(int));
        *p->remainingTime = p->runningTime;
        p->priority = rand() % (11);
        ProcessTable[i] = p;
    }
    int clk = 0;
    int quantum = 3;
    int i = 1;
    p = NULL;
    void *ReadyQueue;

    ReadyQueue = RRInit(NULL);
    while (i <= sz || p != NULL)
    {
        while (i <= sz && ProcessTable[i]->arrivalTime == clk)
        {
            //process temp_p = *ProcessTable[i];
            if(p != NULL)
            {
                RREnqueue(ReadyQueue, ProcessTable[i]);
                if (visualtest)
                    printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d  Enqueued\n", clk,
                        ProcessTable[i]->pid, ProcessTable[i]->priority, ProcessTable[i]->arrivalTime, ProcessTable[i]->runningTime);
                ++i;
            }
            else
            {
                if(RREmpty(ReadyQueue))
                {
                    p = ProcessTable[i];
                }
                else
                {
                    RREnqueue(ReadyQueue, ProcessTable[i]);
                    if (visualtest)
                        printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d  Enqueued\n", clk,
                            ProcessTable[i]->pid, ProcessTable[i]->priority, ProcessTable[i]->arrivalTime, ProcessTable[i]->runningTime);

                    ++i;
                }
            }
        }

        ++clk;

        if(p != NULL)
        {
            (*p->remainingTime)--;
        }

        if (p != NULL && (*p->remainingTime) <= 0)
        {
            RRTerminationHandler(ReadyQueue, p);
            if (visualtest)
                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d ended\n", clk,
                       p->pid, p->priority, p->arrivalTime);
            p = NULL;
        }
        
        if( p != NULL && !RREmpty(ReadyQueue))
        {
            if(clk % quantum == 0)
            {
                RREnqueue(ReadyQueue,p);
                if (visualtest)
                    printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d, Remaining Time: %d Stopped\n", clk,
                       p->pid, p->priority, p->arrivalTime, p->runningTime,*p->remainingTime);
                p = RRRunNext(ReadyQueue);
                if (visualtest)
                    printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d, Remaining Time: %d Started\n", clk,
                       p->pid, p->priority, p->arrivalTime, p->runningTime,*p->remainingTime);
            }
        }

        if (p == NULL)
        {
            p = RRRunNext(ReadyQueue);
            //--*p->remainingTime;
            if (visualtest && p != NULL)
            {
                //--*p->remainingTime;
                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d, Remaining Time: %d Started\n", clk,
                       p->pid, p->priority, p->arrivalTime, p->runningTime,*p->remainingTime);
            }
                
        }
    }
}