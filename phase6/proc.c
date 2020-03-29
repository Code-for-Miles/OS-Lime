/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  proc.c, CPE159
 ********************************************/
// all processes are coded here
// processes do not access kernel data, must use syscalls

#include "constants.h" // include only these 2 files
#include "syscalls.h"
#include "data.h"//extern int car_sem;
void InitProc(void) {
   int i;
   car_sem = SemInit(3);
   while(1) {
      SetVideo(1, 1);         // pos video
      Write(STDOUT, ".");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU

      SetVideo(1, 1);         // pos video
      Write(STDOUT, " ");
      for(i=0; i<LOOP/2; i++)asm("inb $0x80"); // busy CPU
   }
}

void Ouch(void) {
  int pid,device;
  pid = GetPid();
  device = (pid%2==0)? TERM0: TERM1;
  Write(device, "Ouch don't touch that!\n\r");
}

void UserProc(void) {
   int my_pid;
   char str[3];
   //char erase[2];
//   get my PID and make a string from it (null-delimited)
   my_pid = GetPid();
   str[0] = '0'+ my_pid/10;
   str[1] = '0'+ my_pid%10;
   str[2] = '\0';
  // set video cursor to beginning of my row,
  //video_p = (unsigned short *)(HOME_POS + my_pid*180);
  SetVideo(my_pid+1,1);
  // write out that extra long msg to test line wrapped and erasure
  Write(STDOUT,"Hello ladies and gentleman!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nThis is a cruel assignment and it should be banned!"); 
  // sleep for 2 seconds2)
  Sleep(2);

   while(1) {
    //  call service to set video cursor to beginning of my row
      SetVideo(my_pid+1,1);
      // call service to write out my PID str
      Write(STDOUT, str);
     // call service to sleep for 2 seconds
      Sleep(2);
     // call service to set video cursor to beginning of my row
      SetVideo(my_pid+1,1);
     // call service to erase my PID str (with "--")
      Write(STDOUT, "--");
     // call service to sleep for 2 seconds
      Sleep(2);
   }
}

void CarProc(void) {
  char str[3];
  int my_pid = GetPid();
  
  str[0] = '0'+my_pid/10;
  str[1] = '0'+my_pid%10;
  str[2] = '\0';

  SetVideo(my_pid+1, 1);
  Write(STDOUT, (char *)&str);

  while(1) {
    SetVideo(my_pid+1, 7);
    Write(STDOUT, "I'm off....       ");
    Sleep(2);
    SemWait(car_sem);
    SetVideo(my_pid+1, 7);
    Write(STDOUT, "I'm on the bridge!");
    Sleep(2);
    SemPost(car_sem);
  }
}

void TermProc() {
  int pid, device;
  char str[3];
  
  pid = GetPid();
  if(pid%2 == 0) device = TERM0;
  else device =TERM1;

  str[0] = '0' + pid/10;
  str[1] = '0' +  pid%10;
  str[2] = '\0';

  Signal(SIGINT, Ouch);

  while(1) { 
    char buff[BUFF_SIZE];
    Write(device, str);
    Write(device, ": Enter: ");
    Read(device, buff);
    Write(device, "\n\rEntered: ");
    Write(device, buff);
    Write(device,"\n\r");
  }
}


void Wrapper(func_p_t handler_p) {  //arg implanted in the stack
  asm("pushal");              // save regs
  handler_p();                //call user's signal handler
  asm("popal");
  asm("movl %%ebp, %%esp;
       popl %%ebp;
       ret $4"::);    //skip ouch address
}





