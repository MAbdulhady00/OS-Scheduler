#include "headers.h"

void clearResources(int);
void ReadInput(LinkedQueue *queue);
int AskUser(int *);
void Initialize(int *);

LinkedQueue *queue = NULL;
void *newProcess;
int msgid;

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    queue = CreateLinkedQueue();
    ReadInput(queue);
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int algo;
    int quantum;
    quantum = AskUser(&algo);
    // 3. Initiate and create the scheduler and clock processes.
    int schedulerPId;
    Initialize(&schedulerPId);
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    // TODO Generation Main Loop
    process *p;
    int Time;
    struct msgBuffer msg;
    msg.mtype = 13;
    while (queue->front)
    {
        Time = getClk();
        // 5. Create a data structure for processes and provide it with its parameters.
        p = queue->front->val;
        if (p->arrivalTime <= Time)
        {
            msg.p = *p;
            fflush(stdout);
            // 6. Send the information to the scheduler at the appropriate time.
            msgsnd(msgid, &msg, sizeof(msg), !IPC_NOWAIT);
            printf("Msg sent I will block now at Time %d\n", Time);
            kill(schedulerPId, SIGUSR1);
            //? msgrcv(msgid, &msg, sizeof(msg), 13, 0);
            dequeueLinkedQueue(queue);
        }
    }
    // 7. Clear clock and other resources
    clearResources(SIGINT);
}

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
    printf("Clearing Resources Before Existing...\n");
    if (queue != NULL)
        DestroyLinkedQueue(queue);
    msgctl(msgid, IPC_RMID, (struct msqid_ds *)0);
    destroyClk(true);
    // Must Be Removed ----------------------------------------------------------------------------------------------------
    //TODO kill(clock,SIGINT);
    killpg(getpgrp(), SIGKILL);
    printf("Exiting...\n");
}

#pragma region IO Methods
char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
}

void ReadInput(LinkedQueue *Q)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    process *tmp;
    FILE *file = fopen("processes.txt", "r");
    if (NULL == file)
    {
        perror("file can't be opened \n");
    }

    while ((read = getline(&line, &len, file)) != -1)
    {
        if (line[0] == '#')
            continue;
        tmp = malloc(sizeof(process));
        tmp->pid = atoi(strtok(line, "\t"));
        tmp->arrivalTime = atoi(strtok(NULL, "\t"));
        tmp->runningTime = atoi(strtok(NULL, "\t"));
        tmp->priority = atoi(strtok(NULL, "\t"));
        enqueueLinkedQueue(Q, tmp);
    }
    if (line)
        free(line);
}

void arrow(int rp, int p)
{
    if (rp == p)
    {
        printf("--> ");
    }
    else
    {
        printf("    ");
    }
}

int AskUser(int *position)
{
#define MAX 3
#define MIN 1
    (*position) = 1;
    int keypressed = 0, Quantum = 0;
    while (keypressed != 10)
    {

        system("clear");
        arrow(1, (*position));
        printf("1.HPF\n");
        arrow(2, (*position));
        printf("2.SRTN\n");
        arrow(3, (*position));
        printf("3.RR\n");
        keypressed = getch();
        if (keypressed == 66 && (*position) != MAX)
            (*position)++;
        else if (keypressed == 65 && (*position) != MIN)
            (*position)--;
    }
    if ((*position) == 3)
    {
        system("clear");
        printf("Please Enter the Quantum: ");
        scanf("%d", &Quantum);
    }
    return Quantum;
}

#pragma endregion

#pragma region Initization Methods

void Initialize(int *schedulerPid)
{
    printf("Started Initializing...\n");
    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0644);
    
    struct msqid_ds ctrl;
    msgctl(msgid, IPC_STAT, &ctrl);
    ctrl.msg_qbytes = sizeof(msgBuffer);
    msgctl(msgid, IPC_SET, &ctrl);


    printf("Shared Memeory Id of New Process is %d\n", msgid);
    int pid = fork();
    if (pid == -1)
    {
        printf("Error in forking");
        exit(1);
    }
    else
    {
        // Child
        if (pid == 0)
        {
            char str[20];
            sprintf(str, "%d", msgid);                                      // copy shmid to str to be send to each process
            int x = execl("./scheduler.out", "./scheduler.out", str, NULL); // create process
            if (x == -1)
            {
                printf("Error in execution");
                exit(1);
            }
        }
        // Parent
        else
        {
            *schedulerPid = pid;
            // Rest Of Initialization
            int cpid = fork();
            if (cpid == -1)
            {
                printf("Error in forking");
                exit(1);
            }
            else
            {
                // child
                if (cpid == 0)
                {
                    int x = execl("./clk.out", "./clk.out", NULL, NULL); // create process
                    if (x == -1)
                    {
                        printf("Error in execution");
                        exit(1);
                    }
                }
                // Parent
                else
                {
                    // End Of Initialization
                    printf("Finished Initialization Successfully;\n");
                }
            }
        }
    }
}

#pragma endregion