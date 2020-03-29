/*******************************************
 *  Team Name: OSLime
 *  Name: Mulugeta Engdaw
 *  Phase: 2
 *  data.h, CPE159
 ********************************************/
// kernel data are all declared in main.c during bootstrap
// kernel .c code reference them as 'extern'

#ifndef __DATA__
#define __DATA__

#include "types.h"             // defines q_t, pcb_t, PROC_MAX, STACK_SIZE

extern int cur_pid;            // PID of current selected running process
extern q_t avail_q;
extern q_t ready_q;           // prototype the rest...
extern char stack[PROC_MAX][STACK_SIZE];
extern pcb_t pcb[PROC_MAX];
extern int sys_ticks;
extern unsigned short *video_p;

#endif                         // endif of ifndef
