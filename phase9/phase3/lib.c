/*******************************************
 *  Team Name: OSLime
 *  Name: Mulugeta Engdaw
 *  Phase: 2
 *  lib.c, CPE159
 ********************************************/
#include "include.h"
#include "types.h"
#include "data.h"

// clear DRAM data block, zero-fill it
void Bzero(char *p, int bytes) {
  while(bytes>0) {
    *p = (char)0;
    p++;
    bytes--;
  }
}

int QisEmpty(q_t *p) { // return 1 if empty, else 0
  return (*p).size==0?1:0;
}

int QisFull(q_t *p) { // return 1 if full, else 0
  return (*p).size==Q_SIZE?1:0;  
}

// dequeue, 1st integer in queue
// if queue empty, return -1
int DeQ(q_t *p) { // return -1 if q[] is empty
   if(QisEmpty(p)) {
     return -1;
   } else {
     int val = (*p).a[(*p).head];
     (*p).head = ((*p).head + 1) % Q_SIZE;
     (*p).size--;
     return val;
   }
}

// if not full, enqueue integer to tail slot in queue
void EnQ(int to_add, q_t *p) {
   if(QisFull(p)) {
      cons_printf("Kernel panic: queue is full, cannot EnQ!\n");
   } else {
      (*p).a[(*p).tail] = to_add;
      (*p).tail = ((*p).tail + 1) % Q_SIZE;
      (*p).size ++;
   }
}

