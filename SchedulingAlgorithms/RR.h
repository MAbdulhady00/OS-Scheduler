#pragma once
#include "../process.h"

void *RRInit(void *args);

void RREnqueue(void *ReadyQueue, process *p);

process *RRRunNext(void *ReadyQueue);

void RRTerminationHandler(void *ReadyQueue, process *p);

void RRDestroy(void *ReadyQueue);

bool RREmpty(void *ReadyQueue);