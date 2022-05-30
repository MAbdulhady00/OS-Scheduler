#include "IO.h"
#include <stdlib.h>
#include <math.h>

#define APPROXIMATE2(floatingNumber) floatingNumber = (int)(floatingNumber * 100 + .5) / 100.0;
#ifdef scheduler_c
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN double sumWTA, sumWTASq, sumWaiting;
EXTERN int nProcess, sumIdleTime;
int finishtime = 0;

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
        ;
        double WTA =  ((double)p->finishTime - (double)p->arrivalTime) / p->runningTime;
        APPROXIMATE2(WTA);
        sumWTA += WTA;
        sumWTASq += WTA * WTA;
        sumWaiting += p->waitTime;
        ++nProcess;
        finishtime = p->finishTime;
        fprintf(logFile, "finished arr %d total %d remain %d wait %d TA %d WTA %.2lf\n", p->arrivalTime, p->runningTime, *p->remainingTime, p->waitTime , p->finishTime - p->arrivalTime,WTA);
        break;
    default:
        break;
    }
}

void logPerf(FILE *perfFile) {
    double avgWta = sumWTA / nProcess;
    double stdWTA = sqrt(sumWTASq / nProcess - avgWta * avgWta);
    double avgWait = sumWaiting / nProcess;

    APPROXIMATE2(avgWta);
    APPROXIMATE2(stdWTA);
    APPROXIMATE2(avgWait);

    fprintf(perfFile, "CPU utilization = %.2lf%%\n", (double)(finishtime - sumIdleTime + 1) / finishtime * 100);
    fprintf(perfFile, "Avg WTA = %.2lf\n", avgWta);
    fprintf(perfFile, "Avg Waiting = %.2lf\n", avgWait);
    fprintf(perfFile, "Std WTA = %.2lf", stdWTA);
}