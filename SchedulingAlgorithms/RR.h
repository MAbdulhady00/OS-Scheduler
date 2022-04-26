#pragma once
#include "../process_data.h"

void *RRInit(void *args);

void RREnqueue(void *ReadyQueue, process *p);

process *RRRunNext(void *ReadyQueue);

void RRTerminationHandler(void *ReadyQueue, process *p);

void RRDestroy(void *ReadyQueue);