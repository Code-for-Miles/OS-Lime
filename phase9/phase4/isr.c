/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 4
 *  isr.c, CPE159
 ********************************************/
#include "include.h"
#include "types.h"
#include "data.h"
#include "isr.h"
#include "lib.h"
#include "proc.h"

// to create a process: alloc PID, PCB, and process stack
// build trapframe, initialize PCB, record PID to ready_q
void NewProcISR(func_p_t p) {  // arg: where process code starts
   int pid;
   if(QisEmpty(&avail_q)) {    // may occur if too many been created
      cons_printf("Kernel panic: no more process!\n");
      return;//breakpoint(); // cannot continue, alternative: breakpoint();
   }

   pid = DeQ(&avail_q);                          // alloc PID (1st is 0)
   Bzero((char*)&pcb[pid], sizeof(pcb_t));     // clear PCB
   Bzero((char *)&stack[pid], STACK_SIZE); // clear stack
   pcb[pid].state = READY;// change process state

   EnQ(pid,&ready_q); // queue it

// point TF_p to stack & fill it out
   pcb[pid].TF_p = ((TF_t *)&stack[pid][STACK_SIZE]);                               
   pcb[pid].TF_p--;
   pcb[pid].TF_p->efl = EF_DEFAULT_VALUE|EF_INTR; // enables intr
   pcb[pid].TF_p->cs = get_cs();                  // duplicate from CPU
   pcb[pid].TF_p->eip = (unsigned int)p;                          // set to code
//   pcb[pid].state = READY;
}

// count run time and switch if hitting time limit
void TimerISR(void) {
   int i;
   outportb(PIC_CONTROL, DONE);              // notify PIC getting done
   pcb[cur_pid].time++;                     // count up time
   pcb[cur_pid].life++;                    // count up life

   if(pcb[cur_pid].time == TIME_MAX) {       // if runs long enough
      EnQ(cur_pid, &ready_q);               // move it back to ready_q
      pcb[cur_pid].state = READY;          // change its state          
      cur_pid = -1;
   }
   sys_ticks++;
   for(i=0; i<PROC_MAX; i++) {
     if(pcb[i].state == SLEEPY && pcb[i].wake_time == sys_ticks) {
        EnQ(i,&ready_q);
        pcb[i].state = READY;
     }
  }
}

void GetPidISR(void) {
  //put cur_pid to ebx in the trapframe of process cur_pid
  pcb[cur_pid].TF_p->ebx = cur_pid;
}

void SleepISR(void) {
  //get the sleep second from ebx of the trapframe of the process cur_pid
  int sleep_sec = pcb[cur_pid].TF_p->ebx;
  //set the wake time in the pcb of the process cur_pid to:
  //current OS time + the sleep second*100
  pcb[cur_pid].wake_time = sys_ticks +(sleep_sec*100);
  //change the state of process cur_pid
  pcb[cur_pid].state = SLEEPY;
  //reset cur_pid to ...
  cur_pid = -1;
}

void SetVideoISR(void) {
  //get row from ebx in trapframe of process cur_pid
  int row = pcb[cur_pid].TF_p->ebx;
  //get col from ecx in trapframe of process cur_pid
  int col = pcb[cur_pid].TF_p->ecx;
  //set video pointer (video_p) to:
  video_p = (HOME_POS + 80*(row-1) + (col-1));
  //HOME_POS + (row-1)*80 + (col-1)
}

void WriteISR(void) {
  int col_pos, term;
  //get 'device' from ebx in the trapframe of process cur_pid
  int device = pcb[cur_pid].TF_p->ebx;
  //get 'str' addr from ecx in trapframe of process cur_pid
  char *str =  (char *)pcb[cur_pid].TF_p->ecx;
  if(*str == '\0') return;

  if(device == STDOUT) {  //write to target pc.
     while(*str != '\0') {
         if(video_p >= END_POS) {
            video_p = HOME_POS;
         }
         if((video_p - HOME_POS)%80 == 0)  {
             Bzero((char *) video_p, 160);
         }
         if(*str != '\n') {
            *video_p = *str + VGA_MASK;
            video_p++;
         } else {
            col_pos = (video_p - HOME_POS)%80;
            video_p = video_p + (80 - col_pos);
         }
         str++;
     } 
  } else {
     if(device == TERM0) 
       term = 0; // writing to terminal 0
     else
       term = 1; // writing to terminal 1
     outportb(term_if[term].io, *str);
     str++;
     term_if[term].tx_p = str;
     EnQ(cur_pid, &term_if[term].tx_wait_q);
     pcb[cur_pid].state = WAIT;
     cur_pid = -1;
  }
} 
   
void SemInitISR(void) {
  int sem_id = DeQ(&sem_q);
  if(sem_id == -1) {
    cons_printf("Kernel panic......... in semInit");
    breakpoint();
  }

  Bzero((char *)&sem[sem_id], sizeof(sem_t));
  sem[sem_id].passes = pcb[cur_pid].TF_p->ebx;
 
  *(HOME_POS + 21*80) = sem[sem_id].passes+'0'+VGA_MASK;
}

void SemWaitISR(void) {
  int sem_id = pcb[cur_pid].TF_p->ebx;
 
  if(sem[sem_id].passes == 0) {
    //place this proccess in wait_q
    EnQ(cur_pid,&sem[sem_id].wait_q);
    pcb[cur_pid].state = WAIT;
    cur_pid = -1;
  } else {
   //decrement the passes
   sem[sem_id].passes--;
  }
  *(HOME_POS + 21*80) = sem[sem_id].passes+'0'+VGA_MASK;
}

void SemPostISR(void) {
  int blocked_p, sem_id;
  sem_id = pcb[cur_pid].TF_p->ebx;

  if(!QisEmpty(&sem[sem_id].wait_q)) {
    blocked_p = DeQ(&sem[sem_id].wait_q);
    EnQ(blocked_p,&ready_q);
    pcb[blocked_p].state = READY;
  } else {
    sem[sem_id].passes++;
  }
  *(HOME_POS + 21*80) = sem[sem_id].passes+'0'+VGA_MASK;
}

void TermISR(int index) {
  int event;
  event = inportb(term_if[index].io+IIR);
  if(event == IIR_TXRDY) {
    TermTxISR(index);
  } else if(event == IIR_RXRDY) {
    cons_printf("*"); //for now
  }
  outportb(PIC_CONTROL, term_if[index].done);
}

void TermTxISR(int index) {
  int blocked_p;

  if(QisEmpty(&term_if[index].tx_wait_q)) return;

  if(*term_if[index].tx_p == '\0') {
    blocked_p = DeQ(&term_if[index].tx_wait_q);
    EnQ(blocked_p, &ready_q);
    pcb[blocked_p].state = READY;
  } else {
    outportb(term_if[index].io, *term_if[index].tx_p);
    term_if[index].tx_p++;
  }

}


