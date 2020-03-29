/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  data.h, CPE159
 ********************************************/
// kernel data are all declared in main.c during bootstrap
// kernel .c code reference them as 'extern'

#ifndef __DATA__
#define __DATA__

#include "types.h"             // defines q_t, pcb_t, PROC_MAX, STACK_SIZE

extern int cur_pid;            // PID of current selected running process
extern q_t avail_q;
extern q_t ready_q,sem_q;           // prototype the rest...
extern char stack[PROC_MAX][STACK_SIZE];
extern pcb_t pcb[PROC_MAX];
extern int sys_ticks;
extern unsigned short *video_p;
extern sem_t sem[SEM_MAX];               //number of kernel semaphores.
extern int car_sem;                      //to hold a sem id for testing

extern term_if_t term_if[TERM_MAX];
#endif                         // endif of ifndef
