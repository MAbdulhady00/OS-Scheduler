#pragma once
#include "../process_data.h"

void *RRInit(void *args);

void SetQuantum(int q);

void RRNewProcessHandler(void *ReadyQueue, process *p);

void RRNewProcessFinalizationHandler(void *ReadyQueue);

void RRTimeSlotHandler(void* ReadyQueue);

void RREnqueue(void *ReadyQueue, process *p);

process *RRRunNext(void *ReadyQueue);

void RRTerminationHandler(void *ReadyQueue);

void RRDestroy(void *ReadyQueue);