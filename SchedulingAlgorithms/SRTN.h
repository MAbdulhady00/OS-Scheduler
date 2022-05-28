#pragma once
#include "../process_data.h"

void *SRTNInit(void *args);

void SRTNEnqueue(void *ReadyQueue, process *p);

process *SRTNRunNext(void *ReadyQueue);

bool SRTNCmp(process *p1, process *p2);
void SRTNNewProcessHandler(void* ReadyQueue, process* p);
void SRTNTerminationHandler(void *ReadyQueue);
void SRTNNewProcessFinalizationHandler(void *ReadyQueue);
void SRTNDestroy(void *ReadyQueue);
bool SRTNEmpty(void *ReadyQueue);