#pragma once
#include "../process_data.h"

void *HPFInit(void *args);

void HPFNewProcessHandler(void *ReadyQueue, process *p);

void HPFNewProcessFinalizationHandler(void *ReadyQueue);

void HPFEnqueue(void *ReadyQueue, process *p);

process *HPFRunNext(void *ReadyQueue);

void HPFTerminationHandler(void *ReadyQueue);

void HPFDestroy(void *ReadyQueue);