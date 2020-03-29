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
#include "lib.h"
#include "include.h"
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

void ChildCode(int device) {
  int pid;
  char str[3];

  pid = GetPid();
//  ppid = GetPpid(); 

  PidStr(pid, str);
  Write(device, "\n\r");
  Write(device, "I am Child, pid ");
  Write(device, str);
  Write(device, "\n\r");
  Sleep(pid);
  Exit(pid*5);
}
/* creates a null-delimited string from pid
 * eliminates redendency
 * */
void PidStr(int pid, char *str) { 
  str[0] = '0' + pid/10;
  str[1] = '0' + pid%10;
  str[2] = '\0';
}

void ChldHandler() {
  int pid, cpid, ec, device;
  char str[3];
  cpid =  Wait(&ec);
  pid = GetPid();
  device = pid%2?TERM1:TERM0; 
  //issue write calls
  PidStr(pid, str);
  Write(device, "my_pid ");
  Write(device, str);

  PidStr(cpid, str);
  Write(device, ", cpid ");
  Write(device, str);

  PidStr(ec, str);
  Write(device, ", ec ");
  Write(device, str);

  Write(device, "\n\r");

  Signal(SIGCHLD, (func_p_t)0);
}

void UserProc(void) {
   int my_pid;
   char str[3];
//   get my PID and make a string from it (null-delimited)
   my_pid = GetPid();
   PidStr(my_pid, str);

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

  PidStr(my_pid, str);
  SetVideo(my_pid+1, 1);
  Write(STDOUT, str);

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
  int pid, device, fg, cpid, ec;
  char str[3], cpstr[3], ecstr[3];
  char buff[BUFF_SIZE];
  
  pid = GetPid();
  device = (pid%2 == 0)?TERM0:TERM1;

  PidStr(pid, str);

  Signal(SIGINT, Ouch);

  while(1) { 
    Write(device, str);
    Write(device, ": Enter: ");
    Read(device, buff);
    Write(device, "\n\rEntered: ");
    Write(device, buff);
    Write(device,"\n\r");
    if(StrCmp(buff, "fork")) {
      fg = 1;
    }else if(StrCmp(buff, "fork&")) {
      fg = 0;
    } else continue;

    if(!fg) Signal(SIGCHLD, ChldHandler);
    
    switch(Fork()) {
      case -1: Write(device, "OS failed to fork!"); break;
      case 0: 
         //ChildCode(device);
       if(-1 == Exec(ChildCode, device)) {
          Write(device, "OS failed to Exec()!\n\r");
          Exit(-1);
       }
      default:
        Sleep(pid * 2);
        if(fg) {
          cpid = Wait(&ec);
          PidStr(cpid, cpstr);
          PidStr(ec, ecstr);
          Write(device, "my_pid ");
          Write(device, str);
          Write(device, " cpid ");
          Write(device, cpstr);
          Write(device, " ec ");
          Write(device, ecstr);
          Write(device, "\n\r");
        }
        break;
    }
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

