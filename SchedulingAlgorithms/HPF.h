#pragma once
#include "../process.h"

void *HPFInit(void *args);

void HPFEnqueue(void *ReadyQueue, process *p);

process *HPFRunNext(void *ReadyQueue);

void HPFTerminationHandler(void *ReadyQueue, process *p);

void HPFDestroy(void *ReadyQueue);