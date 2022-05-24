#include "IO.h"
#include <stdlib.h>

void initializeOut(FILE **logFile, FILE **perfFile)
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
}

void freeOut(FILE *logFile, FILE *perfFile)
{
    fclose(perfFile);
    fclose(logFile);
}

void logProcess(FILE *logFile, process *p, int clk)
{
    fprintf(logFile, "At time %d process %d ", clk, p->pid);
    switch (p->state)
    {
    case STARTED:
        fprintf(logFile, "started arr %d total %d remain %d wait %d\n", p->arrivalTime, p->runningTime, *p->remainingTime, p->waitTime);
        break;
    case RESUMED:
        fprintf(logFile, "resumed arr %d total %d remain %d wait %d\n", p->arrivalTime, p->runningTime, *p->remainingTime, p->waitTime);
        break;
    case STOPPED:
        fprintf(logFile, "stopped arr %d total %d remain %d wait %d\n", p->arrivalTime, p->runningTime, *p->remainingTime, p->waitTime);
        break;
    case FINISHED:
        fprintf(logFile, "finished arr %d total %d remain %d wait %d TA %d WTA %.2lf\n", p->arrivalTime, p->runningTime, *p->remainingTime, p->waitTime, p->finishTime - p->arrivalTime, ((double)p->finishTime - (double)p->arrivalTime) / p->runningTime);
        break;
    default:
        break;
    }
}