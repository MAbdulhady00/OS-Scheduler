#pragma once
#include "../process_data.h"

void *HPFInit(void *args);

bool HPFCmp(process *p1, process *p2);

void HPFNewProcessHandler(void *ReadyQueue, process *p);

void HPFNewProcessFinalizationHandler(void *ReadyQueue);

void HPFEnqueue(void *ReadyQueue, process *p);

process *HPFRunNext(void *ReadyQueue);

void HPFTerminationHandler(void *ReadyQueue);

void HPFDestroy(void *ReadyQueue);