/*******************************************
 *  Team Name: OSLime
 *  Name: Mulugeta Engdaw
 *  Phase: 2
 *  types.h, CPE159
 ********************************************/
#ifndef __TYPES__
#define __TYPES__

#include "constants.h"

typedef void (*func_p_t)(void); // void-return function pointer type

typedef enum {AVAIL, READY, RUN, SLEEPY} state_t;

typedef struct {
   //unsigned int reg[8];
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
   unsigned int entry;
   unsigned int eip;
   unsigned int cs;
   unsigned int efl;
} TF_t;

typedef struct {
  // ...                       // read in 1.html
  TF_t *TF_p;
  int time;
  int life;
  int wake_time;
  state_t state;
} pcb_t;                     

typedef struct {             // generic queue type
 int a[PROC_MAX];
 int size;
 int tail;
 int head;
} q_t;

#endif
