/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  main.c, CPE159
 ********************************************/
// OS bootstrap and The Kernel for OS 
#include "include.h"  // given SPEDE stuff
#include "types.h"    // kernle data types
#include "lib.h"      // small handly functions
#include "proc.h"     // all user process code here
#include "isr.h"      // kernel ISR code
#include "entry.h"    // entries to kernel (TimerEntry, etc.)

/* 
 * kernel data are all here:
 */
int cur_pid;                        // current running PID; if -1, none selected
q_t ready_q, avail_q, sem_q, wait_q;               // avail PID and those created/ready to run
pcb_t pcb[PROC_MAX];                // Process Control Blocks
char stack[PROC_MAX][STACK_SIZE];   // process runtime stacks
int sys_ticks;
unsigned short *video_p;
sem_t sem[SEM_MAX];               //number of kernel semaphores.
int car_sem;                      //to hold a sem id for testing

term_if_t term_if[TERM_MAX];

void TermInit(int);

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
   fill_gate(
             &IVT_p[SYSCALL],
             (int)SyscallEntry,
             get_cs(),
             ACC_INTR_GATE,
             0);                     // fill out IVT for Syscall
   fill_gate(
             &IVT_p[TERM0],
             (int)Term0Entry,
             get_cs(),
             ACC_INTR_GATE,
             0);                    //fill out IVT for Term0
   fill_gate(
             &IVT_p[TERM1],
             (int)Term1Entry,
             get_cs(),
             ACC_INTR_GATE,
             0);                    //fill out IVT for Term1

   outportb(PIC_MASK, MASK);         // mask out PIC for timer
   
   Bzero((char*)&ready_q, sizeof(q_t));                      // clear 2 queues
   Bzero((char*)&avail_q, sizeof(q_t));
   Bzero((char*)&wait_q, sizeof(q_t));
  //adding procs to avail_q
   for(i=0; i<PROC_MAX; i++) {
      EnQ(i,&avail_q);
      pcb[i].state = AVAIL;
   }
   cur_pid = -1;
   //put semaphore ID's to sem_q
   Bzero((char*)&sem_q, sizeof(q_t));
   for(i=0; i<SEM_MAX; i++) {
     EnQ(i,&sem_q);
   }
   sys_ticks = 0;
   video_p = (unsigned short *)HOME_POS;
   TermInit(0);
   TermInit(1);
}

void TermInit(int index) {
  int i;
  Bzero((char *)&term_if[index], sizeof(term_if_t));
  if(index == 0) {
    term_if[0].io = TERM0_IO;
    term_if[0].done = TERM0_DONE;
  } else {
    term_if[1].io = TERM1_IO;
    term_if[1].done = TERM1_DONE;
  }

  outportb(term_if[index].io+CFCR, CFCR_DLAB);  // CFCR_DLAB is 0x80
  outportb(term_if[index].io+BAUDLO, LOBYTE(115200/9600)); //period of each of 9600 bauds
  outportb(term_if[index].io+BAUDHI, HIBYTE(115200/9600));
  outportb(term_if[index].io+CFCR, CFCR_PEVEN|CFCR_PENAB|CFCR_7BITS);
  outportb(term_if[index].io+IER, 0);
  outportb(term_if[index].io+MCR, MCR_DTR|MCR_RTS|MCR_IENABLE);
  for(i=0; i<LOOP/2; i++) asm("inb $0x80");
  outportb(term_if[index].io+IER, IER_ERXRDY|IER_ETXRDY);  // enable tx and rx intr
  for(i=0; i<LOOP/2; i++) asm("inb $0x80");
  inportb(term_if[index].io);  // clear key entered at PROCOMM screen
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
   // handle ISR event routines
   switch(pcb[cur_pid].TF_p->entry) {
      case GETPID: GetPidISR(); break;
      case SLEEP: SleepISR(); break;
      case SETVIDEO: SetVideoISR(); break;
      case WRITE:  WriteISR(); break;
      case TIMER: TimerISR(); break;
      case SEMINIT: SemInitISR(); break;
      case SEMWAIT: SemWaitISR(); break;
      case SEMPOST: SemPostISR(); break;
      case TERM0: TermISR(0); break;
      case TERM1: TermISR(1); break;
      case READ: ReadISR(); break;
      case SIGNAL: SignalISR(); break;
      case FORK: ForkISR(); break;
      case GETPPID: GetPpidISR(); break;
      case WAITCALL: WaitISR(); break;
      case EXIT: ExitISR(); break;
      default: break;
   }
   
   if(cons_kbhit()) {
      ch = cons_getchar();
      switch(ch) {
        case 'b': breakpoint(); break;
        case 'n': NewProcISR(UserProc); break;
        case 'c': NewProcISR(CarProc); break;
        case 't': NewProcISR(TermProc); break;
        default: break;
      }
   }
   Scheduler();
   Loader(pcb[cur_pid].TF_p);
}

