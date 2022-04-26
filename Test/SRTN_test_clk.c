
#include"../headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../process.h"
#include "../SchedulingAlgorithms/SRTN.h"

const int sz = 4;

void create_process(process * forked_process);

int main(int argc, char *argv[])
{
    pid_t id = fork();
    if(id == 0) {
        execl("./clk.out", "./clk.out", NULL);
    }

    sleep(1);
    initClk();
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
        p->remainingTime = malloc(sizeof(int));
        *p->remainingTime = p->runningTime;
        p->priority = rand() % (11);
        ProcessTable[i] = p;
        create_process(p);
        kill((pid_t)p->pid,SIGSTOP);
    }
    int clk = 0;
    int i = 1;
    p = NULL;
    void *ReadyQueue;

    ReadyQueue = SRTNInit(NULL);
    int time_before = getClk() ,time_after =getClk();
    while (i <= sz || p != NULL)
    {

        clk = getClk();

        while (i <= sz && ProcessTable[i]->arrivalTime == clk)
        {
            if(p!=NULL)
            {
                if(ProcessTable[i]->runningTime < *p->remainingTime) 
                {
                   
                    kill((pid_t)p->pid,SIGSTOP);
                    process* temp = p;
                    p = ProcessTable[i];
                    SRTNEnqueue(ReadyQueue,temp);
                    kill((pid_t)p->pid,SIGCONT);
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
               
                SRTNEnqueue(ReadyQueue, ProcessTable[i]);
                if (visualtest)
                    printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d  Enqueued\n", clk,
                        ProcessTable[i]->pid, ProcessTable[i]->priority, ProcessTable[i]->arrivalTime, ProcessTable[i]->runningTime);

                ++i;
            }

        }


        if (p != NULL && *p->remainingTime == 0)
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
            
            // if(p!=NULL)
            // {
            //     //printf("Start process 1\n");
                
            //     //printf("Start process 1\n");    
            // }
            
            if (visualtest && p != NULL)
            {

                printf("At time %d:\nProcess %d, Prioriy: %d, Arrival Time: %d, Running Time: %d,Remaining Time: %d Started\n", clk,
                       p->pid, p->priority, p->arrivalTime, p->runningTime,*p->remainingTime);
                 kill((pid_t)p->pid,SIGCONT);
            }
        }
        while(time_after>= time_before)
            time_before = getClk();
        time_after = getClk();

        //++clk;
    }
    int stat_loc =0;
    for(int i=1;i<=sz;i++)
    {
        printf("Clear resource process %d \n" ,ProcessTable[i]->pid);
        shmctl(ProcessTable[i]->shmid_process, IPC_RMID, (struct shmid_ds *)0);
        wait(&stat_loc);
        //kill(ProcessTable[i]->pid,SIGINT);   
    }
    destroyClk(true);
    return 0;
}
void create_process(process * forked_process)
{
    int shmid = shmget(IPC_PRIVATE, 4, IPC_CREAT | 0644); // create shared memory for process remaining time
    
    forked_process->shmid_process = shmid;           // store shmid of process to be sent & deleted later
    void *shmaddr = shmat(shmid, (void *)0, 0);
    //int remain_time = *forked_process->remainingTime;
    forked_process->remainingTime = (int*)shmaddr;
    *forked_process->remainingTime =  forked_process->runningTime;
    forked_process->pid = fork();                    // store pid of process
    
    if(forked_process->pid  == -1)
    {
        printf("Error in forking");
    }
    if (forked_process->pid  == 0)
    {
        //printf("Hello from process %d before creation \n",getpid());
        //void *shmaddr = shmat(shmid, (void *)0, 0);
        //int* remaining_time = (int*) shmaddr;   
        //*remaining_time = 3;
        //sleep(5);
        char str[20];
        sprintf(str,"%d",forked_process->shmid_process);         // copy shmid to str to be send to each process
        //sleep(0);
        int x = execl("./process.out","./process.out",str,NULL); // create process
        if(x == -1)
        {
            printf("Error in execution");
        }
    }   

}