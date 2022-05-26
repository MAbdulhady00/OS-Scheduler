#include "headers.h"

void clearResources(int);
void ReadInput(LinkedQueue *queue);
int AskUser(int *);
void Initialize(int *, int, int);

LinkedQueue *queue = NULL, *bufferQueue = NULL;
void *newProcess;
int msgid;
bool Interuption = true;

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    queue = CreateLinkedQueue();
    bufferQueue = CreateLinkedQueue();
    printf("Reading \n");
    ReadInput(queue);
    printf("Finish Reading \n");
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int algo;
    int quantum;
    if (argc <= 1)
        quantum = AskUser(&algo);
    else
    {
        algo = atoi(argv[1]);
        if (algo == 3)
            quantum = atoi(argv[2]);
    }
    // 3. Initiate and create the scheduler and clock processes.
    int schedulerPId;
    Initialize(&schedulerPId, algo, quantum);
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    // TODO Generation Main Loop
    process *p;
    int time, count, time_after = getClk(), time_before = getClk();
    struct msgBuffer msg;
    while (queue->front)
    {
        // 5. Create a data structure for processes and provide it with its parameters.
        p = queue->front->val;
        count = 0;
        while (p->arrivalTime <= time)
        {
            // Count the amount of processe in current time
            dequeueLinkedQueue(queue);
            enqueueLinkedQueue(bufferQueue, p);
            ++count;
            if (queue->front == NULL)
                break;
            p = queue->front->val;
        }

        while (count > 0)
        {
            p = bufferQueue->front->val;
            msg.p = *p;
            msg.mtype = count + 1;
            printf("PID: %d, Msg sent I will block now at Time %d\n", msg.p.pid, time);
            msgsnd(msgid, &msg, sizeof(msg.p), 0);
            printf("msg send!\n");
            --count;
            dequeueLinkedQueue(bufferQueue);
        }

        msg.mtype = 1;
        msgsnd(msgid, &msg, sizeof(msg.p), 0);

        // Sleep for next clk cycle
        while (time_after <= time_before)
        {
            time_after = getClk();
        }
        time_before = getClk();

        ++time;
    }

    kill(schedulerPId, SIGURG);
    //msg.mtype = 1;
    //msgsnd(msgid, &msg, sizeof(msg.p), 0);
    // Sleep to give a chance for scheduler to recieve last msg(TODO find a better way)
    // sleep(1);
    int stat_loc = 0;
    waitpid(schedulerPId, &stat_loc, 0);

    // 7. Clear clock and other resources
    Interuption = false;
    clearResources(SIGINT);

    return 0;
}

void clearResources(int signum)
{
    signal(SIGINT, NULL);
    // TODO Clears all resources in case of interruption
    printf("Clearing Resources Before Existing...\n");
    if (queue != NULL)
        DestroyLinkedQueue(queue);
    if (bufferQueue != NULL)
        DestroyLinkedQueue(bufferQueue);

    msgctl(msgid, IPC_RMID, (struct msqid_ds *)0);
    destroyClk(false);
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
    int arrivalId = 1;
    while ((read = getline(&line, &len, file)) != -1)
    {
        if (line[0] == '#')
            continue;
        tmp = malloc(sizeof(process));
        tmp->pid = atoi(strtok(line, "\t"));
        tmp->arrivalTime = atoi(strtok(NULL, "\t"));
        tmp->runningTime = atoi(strtok(NULL, "\t"));
        tmp->priority = atoi(strtok(NULL, "\t"));
        tmp->arrivalId = arrivalId++;
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

void Initialize(int *schedulerPid, int algo, int q)
{
    printf("Started Initializing...\n");
    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0644);

    struct msqid_ds ctrl;
    msgctl(msgid, IPC_STAT, &ctrl);
    ctrl.msg_qbytes = sizeof(msgBuffer);
    msgctl(msgid, IPC_SET, &ctrl);

    printf("Msg Queue Id of New Process is %d\n", msgid);
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
            char str[20], str2[20], str3[20];
            sprintf(str, "%d", msgid); // copy msgid to str to be send to each process
            sprintf(str2, "%d", algo);
            int x;
            if (algo == 3)
            {
                sprintf(str3, "%d", q);
                int x = execl("./scheduler.out", "./scheduler.out", str, str2, str3, NULL); // create process
            }
            else
            {
                int x = execl("./scheduler.out", "./scheduler.out", str, str2, NULL); // create process
            }
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