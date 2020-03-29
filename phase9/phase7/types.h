/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  types.h, CPE159
 ********************************************/
#ifndef __TYPES__
#define __TYPES__

#include "constants.h"

typedef void (*func_p_t)(void); // void-return function pointer type

typedef enum {AVAIL, READY, RUN, SLEEPY, WAIT} state_t;

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
  func_p_t siginit_handler_p;
  int ppid;
} pcb_t;                     

typedef struct {             // generic queue type
 int a[PROC_MAX];
 int size;
 int tail;
 int head;
} q_t;

typedef struct {
  int passes;         //max num of processes that can pass
  q_t wait_q;         //blocked proc ID's
} sem_t;

typedef struct {
  int io,           //I/O # of terminal prot (TERM0_IO and TERM1_IO)
      done;         // done signal to PIC (TERM0_DONE and TERM1_DONE)
  char *tx_p;       //point to a character in the string
  char *rx_p;       // point to inside the buffer
  q_t tx_wait_q;    // TX requester (wait for completion)
  q_t rx_wait_q;    // waiting for terminal KB input
} term_if_t;

#endif
