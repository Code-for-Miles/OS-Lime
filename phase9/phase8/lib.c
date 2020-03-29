/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
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

int InQ(int ppid, q_t *q) {
  int head, q_size, i;
  head = (*q).head;
  q_size = (*q).size;
  for(i=0; i< head + q_size; i++) {
    if((*q).a[i%Q_SIZE] == ppid) return 1;
  }
  return 0;
}

void DelQ(int ppid, q_t *q) {
  q_t temp;
  int val;

  Bzero((char *)&temp, sizeof(q_t));
  temp.head = temp.tail = (*q).head;

  while(!QisEmpty(q)) {
    val = DeQ(q);
    if(!(val == ppid)) {
      EnQ(val, &temp);
    }
  }
  *q = temp;
}

int StrCmp(char *s1, char *s2) {
  while(*s1 || *s2) {
    if(*s1 != *s2) {
      return 0;
    }
    s1++;
    s2++;
  }
  return 1;
}

void MemCpy(char *dst, char *src, int size) {
  int i;
  if(size <= 0) return;

  for(i = 0; i<size; i++) {
    *dst++ = *src++;
  }
}

