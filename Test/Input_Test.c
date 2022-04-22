#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../DataStructures/LinkedQueue.h"
void readInput(LinkedQueue *Q)
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    process *tmp;
    // Opening file in reading mode
    file = fopen("processes.txt", "r");
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
    fclose(file);
}
void test(LinkedQueue *Q)
{
    printf("#id arrival runtime priority\n");
    process *p;
    while (Q->front)
    {
        p = Q->front->val;
        printf("%d\t%d\t%d\t%d\n", p->pid, p->arrivalTime, p->runningTime, p->priority);
        dequeueLinkedQueue(Q);
        free(p);
    }
}
int main()
{
    LinkedQueue *Q = CreateLinkedQueue();
    readInput(Q);
    test(Q);
    DestroyLinkedQueue(Q);
    printf("Read Input test completed successfully!\n");
    return 0;
}
