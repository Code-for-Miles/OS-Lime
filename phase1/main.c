/*******************************************
 *  Team Name: OSLime
 *  Name: Mulugeta Engdaw
 *  Phase: 1
 *  main.c, CPE159
 ********************************************/
// OS bootstrap and The Kernel for OS 
#include "include.h"  // given SPEDE stuff
#include "types.h"    // kernle data types
#include "lib.h"      // small handly functions
#include "proc.h"     // all user process code here
#include "isr.h"      // kernel ISR code
#include "entry.h"    // entries to kernel (TimerEntry, etc.)

// kernel data are all here:
int cur_pid;                        // current running PID; if -1, none selected
q_t ready_q, avail_q;               // avail PID and those created/ready to run
pcb_t pcb[PROC_MAX];                // Process Control Blocks
char stack[PROC_MAX][STACK_SIZE];   // process runtime stacks

void InitKernel(void) {             // init and set up kernel!
   int i;
   struct i386_gate *IVT_p;         // IVT's DRAM location
   IVT_p = get_idt_base();          // get IVT location
   fill_gate(
             &IVT_p[TIMER],
             (int)TimerEntry,
             get_cs(),
             ACC_INTR_GATE,
             0);                      // fill out IVT for timer
   outportb(PIC_MASK, MASK);         // mask out PIC for timer
   
   Bzero((char*)&ready_q, sizeof(q_t));                      // clear 2 queues
   Bzero((char*)&avail_q, sizeof(q_t));
  //adding procs to avail_q
   for(i=0; i<PROC_MAX; i++) {
      EnQ(i,&avail_q);
      pcb[i].state = AVAIL;
   }
   cur_pid = -1;

}

void Scheduler(void) {             // choose a cur_pid to run
   //if cur_pid is greater than 0, just return; // a user PID is already picked
   if(cur_pid > 0) {
      return;
   }
   //if ready_q is empty && cur_pid is 0, just return; // InitProc OK
   if(QisEmpty(&ready_q)&& cur_pid==0) {
      return;
   }
    //if ready_q is empty && cur_pid is -1 {
   if(QisEmpty(&ready_q) && cur_pid == -1) {
      cons_printf ("Kernel panic: no process to run!\n");
      breakpoint();                                  // to GDB we go
   }
   
   if(cur_pid != -1) {
     EnQ(cur_pid, &ready_q);
     pcb[cur_pid].state = READY; 
   }
   cur_pid = DeQ(&ready_q);
   pcb[cur_pid].time = 0;
   pcb[cur_pid].state = RUN;
}

int main(void) {                       // OS bootstraps
   //initialize the kernel-related stuff by calling InitKernel()
   InitKernel();
   //create InitProc process;            // create InitProc
   NewProcISR(InitProc);
   Scheduler();
   Loader(pcb[0].TF_p);         // load proc to run
   return 0; // statement never reached, compiler needs it for syntax
}

void TheKernel(TF_t *TF_p) {           // kernel runs
   char ch;
   // save TF addr
   pcb[cur_pid].TF_p = TF_p;
   // handle tiemr event
   TimerISR();
   if(cons_kbhit()) {
      ch = cons_getchar();
      if(ch == 'b') {
         breakpoint();
      } else if(ch == 'n') {
         NewProcISR(UserProc);
      }
   }
   Scheduler();
   Loader(pcb[cur_pid].TF_p);
}

