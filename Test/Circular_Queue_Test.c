#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../DataStructures/Circular_Queue.h"

const int sz = 10;
int main()
{
  // Change this if you want a new test vector
  srand(1337);
  CircularQueue *cq = CreateCircularQueue();
  process *p;
  process *parr[sz];
  for (int i = 0; i < sz; ++i)
  {
    p = malloc(sizeof(process));
    p->pid = i;
    p->priority = rand() % 11;
    p->arrivalTime = rand() % 25;
    enqueueCircularQueue(cq, p);
    parr[i] = p;
  }
  
  //Compare heapsort with sorted array
  for (int i = 0; i < (sz * 3) + (3 * 2); ++i)
  {
    p = peekCircularQueue(cq);

    if(i == sz)
    {
        process *p;
        p = malloc(sizeof(process));
        p->pid = sz;
        p->priority = rand() % 11;
        p->arrivalTime = rand() % 25;
        enqueueCircularQueue(cq, p);
        p = malloc(sizeof(process));
        p->pid = sz + 1;
        p->priority = rand() % 11;
        p->arrivalTime = rand() % 25;
        enqueueCircularQueue(cq, p);
        p = malloc(sizeof(process));
        p->pid = sz + 2;
        p->priority = rand() % 11;
        p->arrivalTime = rand() % 25;
        enqueueCircularQueue(cq, p);
    }

    printf("Value:  %d  Priority: %d, ArrivalTime: %d\n",p->pid, p->priority, p->arrivalTime);

    if(p->pid % 3 == 0)
        p = dequeueCircularQueue(cq);
    else
        advanceCircularQueue(cq);
  }

  destroyCircularQueue(cq);
  printf("Priority queue test completed successfully!\n");
  return 0;
}
