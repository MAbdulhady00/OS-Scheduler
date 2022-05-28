#pragma once
#include <stdio.h>
#include "process_data.h"

void initializeOut(FILE **logFile, FILE **perfFile, FILE** memlogFile);

void freeOut(FILE *logFile, FILE *perfFile, FILE *memlogFile);

void logProcess(FILE *logFile, process *p, int clk);

void logMEM(FILE *logFileMEM, process *p, int clk);