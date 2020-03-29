/*******************************************
 *line.10
 Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  syscalls.c, CPE159
 ********************************************/
// calls to OS services

#include "constants.h"
#include "syscalls.h"

void Sleep(int sec) {     // # of 1/100 of a second
   asm("movl %0, %%eax;     // service #162 (SLEEP)
        movl %1, %%ebx;     // sleep period in ticks
        int $128"
        :
        : "g" (SLEEP), "g" (sec)
        : "eax", "ebx"       // used registers
   );
}

int GetPid(void) {
   int pid;

   asm("movl %1, %%eax;     // service #20 (GETPID)
        int $128;           // interrupt!
        movl %%ebx, %0"     // after, copy eax to variable 'pid'
        : "=g" (pid)         // output
        : "g" (GETPID)       // input
        : "eax", "ebx"       // used registers
   );

   return pid;
}

void SetVideo(int row, int col) {
  asm("movl %0, %%eax;      //copy device type eax
       movl %1, %%ebx;      //copy row to ebx
       movl %2, %%ecx;      // copy col to ecx
       int $128"            //interrupt
       :                    //no output
       :"g"(SETVIDEO),"g"(row), "g"(col)  // inputs
       : "eax","ebx", "ecx"       // used registers
  );
}

void Write(int device, char *str) {
  asm("movl %0, %%eax;                //copy device type to eax
       movl %1, %%ebx;               //copy device type to ebx
       movl %2, %%ecx;               //copy *str to ecx
       int $128"                     //interrupt
       :                             // no output
       :"g"(WRITE),"g"(device), "g"((int)str)     
       :"eax","ebx", "ecx"
  );
}


void Read(int device, char *buff) {
  asm("movl %0, %%eax;                //copy device type to eax
       movl %1, %%ebx;               //copy device type to ebx
       movl %2, %%ecx;               //copy *buff to ecx
       int $128"                     
       :                             // no output
       :"g"(READ),"g"(device), "g"((int)buff)     
       :"eax","ebx", "ecx"
  );
}

int SemInit(int passes) {
  int sem_id;
  asm("movl %1, %%eax;
       movl %2, %%ebx;
       int $128;
       movl %%ecx, %0"
       : "=g" (sem_id)
       : "g"(SEMINIT), "g"(passes)
       : "eax","ebx", "ecx"
  );
  return sem_id;
}

void SemWait(int sem_id) {
  asm("movl %0, %%eax;
       movl %1, %%ebx;
       int $128"
       :
       :"g" (SEMWAIT), "g" (sem_id)
       :"eax", "ebx"
  );
}

void SemPost(int sem_id) {
  asm("movl %0, %%eax;
       movl %1, %%ebx;
       int $128"
       :
       :"g" (SEMPOST), "g" (sem_id)
       : "eax", "ebx"
  );
}

void Signal(int sig_num, func_p_t p) {
  asm("movl %0, %%eax;
       movl %1, %%ebx;
       movl %2, %%ecx;
       int $128"
       :
       :"g"(SIGNAL), "g"(sig_num), "g"((int)p)
       :"eax","ebx","ecx"
  );  
}
