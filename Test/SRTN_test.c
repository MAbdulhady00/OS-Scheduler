#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../process.h"
#include "../SchedulingAlgorithms/SRTN.h"

const int sz = 4;

int main(int argc, char *argv[])
{
    bool visualtest = true;
    process *p;
    process *ProcessTable[sz + 1];
    int atime = 0;
    printf("Start SRTN\n");
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
    int i = 1;
    p = NULL;
    void *ReadyQueue;

    ReadyQueue = SRTNInit(NULL);
    while (i <= sz || p != NULL)
    {
        while (i <= sz && ProcessTable[i]->arrivalTime == clk)
        {
            //process temp_p = *ProcessTable[i];
            if(p!=NULL)
            {
                if(ProcessTable[i]->runningTime < *p->remainingTime) 
                {
                    process* temp = p;
                    p = ProcessTable[i];
                    SRTNEnqueue(ReadyQueue,temp);
                    if (visualtest)
                    {
                        printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d, Remaining Time: %d  Stopped\n", clk,
                            temp->pid, temp->priority, temp->arrivalTime, temp->runningTime,*temp->remainingTime);
                        printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d  Started\n", clk,
                            ProcessTable[i]->pid, ProcessTable[i]->priority, ProcessTable[i]->arrivalTime, ProcessTable[i]->runningTime);
                    
                    
                    }
                    ++i;
                } 
                else
                {
                    SRTNEnqueue(ReadyQueue, ProcessTable[i]);
                    if (visualtest)
                        printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d  Enqueued\n", clk,
                            ProcessTable[i]->pid, ProcessTable[i]->priority, ProcessTable[i]->arrivalTime, ProcessTable[i]->runningTime);

                    ++i;
                    
                }
            }
            else
            {
               
                if(SRTNEmpty(ReadyQueue))
                    {
                        p = ProcessTable[i];
                    }
                    else
                    {

                        SRTNEnqueue(ReadyQueue, ProcessTable[i]);
                        if (visualtest)
                            printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d  Enqueued\n", clk,
                                ProcessTable[i]->pid, ProcessTable[i]->priority, ProcessTable[i]->arrivalTime, ProcessTable[i]->runningTime);

                        ++i;
                    }
            }

        }
        ++clk;

        if (p != NULL && --*p->remainingTime == 0)
        {
            SRTNTerminationHandler(ReadyQueue, p);
            if (visualtest)
                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d ended\n", clk,
                       p->pid, p->priority, p->arrivalTime);
            p = NULL;
        }
        if (p == NULL)
        {
            p = SRTNRunNext(ReadyQueue);
            //--*p->remainingTime;
            if (visualtest && p != NULL)
            {
                //--*p->remainingTime;
                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d,Remaining Time: %d Started\n", clk,
                       p->pid, p->priority, p->arrivalTime, p->runningTime,*p->remainingTime);
            }
                
        }

        
    }
}