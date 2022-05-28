#include "IO.h"
#include <stdlib.h>
#include <math.h>

void initializeOut(FILE **logFile, FILE **perfFile, FILE** memlogFile)
{
    // Opening file in reading mode
    *logFile = fopen("scheduler.log", "w+");
    if (logFile == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(*logFile, "#At time x process y state arr w total z remain y wait k\n");
    *perfFile = fopen("scheduler.perf", "w+");
    if (perfFile == NULL)
    {
        printf("Error!");
        exit(1);
    }
    *memlogFile = fopen("memory.log", "w+");
    if (memlogFile == NULL)
    {
        printf("Error!");
        exit(1);
    }
}

void freeOut(FILE *logFile, FILE *perfFile, FILE *memlogFile)
{
    fclose(perfFile);
    fclose(logFile);
    fclose(memlogFile);
}

void logProcess(FILE *logFile, process *p, int clk)
{
    fprintf(logFile, "At time %d process %d ", clk, p->pid);
    double WTA =  ((double)p->finishTime - (double)p->arrivalTime) / p->runningTime;
    WTA = (int)(WTA * 100 + .5) / 100.0;
    switch (p->state)
    {
    case STARTED:
        fprintf(logFile, "started arr %d total %d remain %d wait %d\n", p->arrivalTime, p->runningTime, p->remainingTime, p->waitTime);
        break;
    case RESUMED:
        fprintf(logFile, "resumed arr %d total %d remain %d wait %d\n", p->arrivalTime, p->runningTime, p->remainingTime, p->waitTime);
        break;
    case STOPPED:
        fprintf(logFile, "stopped arr %d total %d remain %d wait %d\n", p->arrivalTime, p->runningTime, p->remainingTime, p->waitTime);
        break;
    case FINISHED:
        fprintf(logFile, "finished arr %d total %d remain %d wait %d TA %d WTA %.2lf\n", p->arrivalTime, p->runningTime, p->remainingTime, p->waitTime , p->finishTime - p->arrivalTime,WTA);
        break;
    default:
        break;
    }
}

void logMEM(FILE *logFileMEM, process *p, int clk)
{
    int to = pow(2, ceil(log2(p->memsize)));
    if(to < 8)
        to = 8;

    --to;
     
    if(p->state == FINISHED)
    {
        fprintf(logFileMEM, "at time %d deallocated %d bytes for process %d from %d to %d \n",clk,p->memsize,p->pid,p->address_position,p->address_position + to);
    }
    else
    {
        fprintf(logFileMEM, "at time %d allocated %d bytes for process %d from %d to %d \n",clk,p->memsize,p->pid,p->address_position,p->address_position + to);
    }
}