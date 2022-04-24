#include "headers.h"
#include "process.h"

process p1;
void create_process(process * forked_process);
void handler(int signum);
int main(int agrc, char * argv[])
{
    //initClk();
    p1.runningTime = 10;
    create_process(&p1);
    // int pid,shmid;
    // int shmkey = ftok("keyfile",65);
    // shmid = shmget(IPC_PRIVATE, 4, IPC_CREAT | 0644);
    
    // if (shmid == -1)
    // {
    //     perror("Error in create");
    //     exit(-1);
    // }
    // else
    //     printf("\nShared memory ID = %d\n", shmid);

    // pid = fork();
    // if(pid == -1)
    // {
    //     printf("Error in forking");
    // }
    // if (pid == 0)
    // {
    //     printf("Hello from process");
    //     void *shmaddr = shmat(shmid, (void *)0, 0);
    //     int* remaining_time = (int*) shmaddr;   
    //     *remaining_time = 3;
    //     //sleep(5);
    //     char str[20];
    //     sprintf(str,"%d",shmid);
    //     int x = execl("./process.out","./process.out",str,NULL);
    //     if(x == -1)
    //     {
    //         printf("Error in execution");
    //     }
    // }
    printf("Child process ID = %d\n",p1.pid);
    signal(SIGINT,handler);
    while(*(p1.remainingTime) >0)
    {
    //void *shmaddr = shmat(p1.shmid_process, (void *)0, 0);
    //int* remaining_time = (int*) shmaddr;
    
    //printf("parent: remaining time = %d\n",*remaining_time);
    //*remaining_time = -1;
    
    printf("parent: remaining time = %d\n",*(p1.remainingTime));
    sleep(1);
    //printf("parent: shmaddr  = %d \n",*(int*)shmaddr);
    }
    
    sleep(1);
    printf("process %d finished \n",p1.pid);
    shmdt(p1.remainingTime);
    shmctl(p1.shmid_process, IPC_RMID, (struct shmid_ds *)0);
    //destroyClk(false);
    
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
        printf("Hello from process before creation \n");
        //void *shmaddr = shmat(shmid, (void *)0, 0);
        //int* remaining_time = (int*) shmaddr;   
        //*remaining_time = 3;
        //sleep(5);
        char str[20];
        sprintf(str,"%d",forked_process->shmid_process);         // copy shmid to str to be send to each process
        int x = execl("./process.out","./process.out",str,NULL); // create process
        if(x == -1)
        {
            printf("Error in execution");
        }
    }   
}
void handler(int signum)
{
    //shmdt(shmaddr);
    shmctl(p1.shmid_process, IPC_RMID, (struct shmid_ds *)0);
    kill(p1.pid,SIGINT);
    exit(0);
}