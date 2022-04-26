#pragma once
#include "../process_data.h"

void *SRTNInit(void *args);

void SRTNEnqueue(void *ReadyQueue, process *p);

process *SRTNRunNext(void *ReadyQueue);

void SRTNNewProcessHandler(void* ReadyQueue, process* p);
void SRTNTerminationHandler(void *ReadyQueue);

void SRTNDestroy(void *ReadyQueue);
bool SRTNEmpty(void *ReadyQueue);