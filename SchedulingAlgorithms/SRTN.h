#pragma once
#include "../process.h"

void *SRTNInit(void *args);

void SRTNEnqueue(void *ReadyQueue, process *p);

process *SRTNRunNext(void *ReadyQueue);

void SRTNTerminationHandler(void *ReadyQueue, process *p);

void SRTNDestroy(void *ReadyQueue);
bool SRTNEmpty(void *ReadyQueue);