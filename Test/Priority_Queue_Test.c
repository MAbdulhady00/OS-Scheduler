#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../DataStructures/Priority_Queue.h"

bool cmp(process *p1, process *p2)
{
  if (p1->priority < p2->priority)
    return true;

  else if (p1->priority == p2->priority)
    return p1->arrivalTime < p2->arrivalTime;

  return false;
}

int qsort_cmp(const void *p1, const void *p2)
{
  bool res = cmp(*((process **)p1), *((process **)p2));
  if (res)
    return -1;

  return 1;
}

const int sz = 1000;
int main()
{
  // Change this if you want a new test vector
  srand(1337);
  PriorityQueue *pq = CreatePriorirtyQueue(cmp);
  process *p;
  process *parr[sz];
  for (int i = 0; i < sz; ++i)
  {
    p = malloc(sizeof(process));
    p->pid = i;
    p->priority = rand() % 11;
    p->arrivalTime = rand() % 25;
    PriorityQueuePush(pq, p);
    parr[i] = p;
  }
  
  qsort(parr, sizeof(parr) / sizeof(*parr), sizeof(*parr), qsort_cmp);

  //Compare heapsort with sorted array
  for (int i = 0; i < sz; ++i)
  {
    p = PriorityQueueGetMin(pq);
    if (p->arrivalTime != parr[i]->arrivalTime || p->priority != parr[i]->priority)
    {
      fprintf(stderr, "Error in priority queue\n");
      printf("Value:    Priority: %d, ArrivalTime: %d\n", p->priority, p->arrivalTime);
      printf("Expected: Priority: %d, ArrivalTime: %d\n", parr[i]->priority, parr[i]->arrivalTime);
      return -1;
    }

    PriorityQueuePop(pq);
  }

  DestroyPriorityQueue(pq);
  printf("Priority queue test completed successfully!\n");
  return 0;
}
