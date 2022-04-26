#pragma once
#include <stdio.h>
#include "process_data.h"

void initializeOut(FILE **logFile, FILE **perfFile);

void freeOut(FILE *log, FILE *perf);

void logProcess(FILE *logFile, process *p, int clk);