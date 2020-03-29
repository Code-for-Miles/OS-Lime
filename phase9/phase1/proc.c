/*******************************************
 *  Team Name: OSLime
 *  Name: Mulugeta Engdaw
 *  Phase: 1
 *  proc.c, CPE159
 ********************************************/
// all user processes are coded here
// processes do not R/W kernel data or code, must use syscalls

#include "include.h"
#include "data.h"
#include "proc.h"

void InitProc(void) {
   int i;
   int j;
   unsigned short *p;

   p = (unsigned short *)LEFT_CORNER; //   point p to 0xb8000; // upper-left corner of display
   while(1) {
      *p = '.' + VGA_MASK;
      for(i=0; i<LOOP/2; i++) {
        asm("inb $0x80");
      }
      *p = ' ' + VGA_MASK;
      for(j=0; j<LOOP/2; j++) {
        asm("inb $0x80");
      }
  }
}

void UserProc(void) {
   int i;
   unsigned short *p;
   while(1) {
      p = (unsigned short *)(LEFT_CORNER + (cur_pid*160));
      *p = (cur_pid/10 + '0') + VGA_MASK;
      p++;
      *p = (cur_pid%10 + '0') +VGA_MASK;

      for(i=0; i<LOOP/2; i++) {
         asm("inb $0x80");
      }
      *p = ' ' + VGA_MASK;
      p--;
      *p = ' ' + VGA_MASK;
      for(i=0; i<LOOP/2; i++) {
        asm("inb $0x80");
      }
   }
}
