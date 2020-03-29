/***********************************************
 *  NAME: Mulugeta Engdaw
 *  Phase 0, Prep 4, Timer Device
 *  main.c
 **********************************************/
#include <spede/flames.h>
#include <spede/machine/asmacros.h>
#include <spede/machine/io.h>
#include <spede/machine/pic.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>
#include <spede/stdio.h>

#define VGA_MASK 0x0f00
#define TIMER  32
#define PIC_MASK 0x21
#define MASK  ~0x01
#define PIC_CONTROL 0x20
#define DONE 0x60
#define LOOP 166666

void TimerEntry(void);

void TimerCode(void) {
  static long count = 0;
    count++;
    if(count%50 == 0) {
      cons_putchar('.'+VGA_MASK);
    }

    if(count%500 == 0) {
      cons_printf("\n5 second delay! Hit any key, 'b' to break!\n");
    }
    outportb(PIC_CONTROL, DONE);
}

int main(void) {
  char character; long i;
  struct i386_gate *IVT_p;
  IVT_p = get_idt_base();
  
  fill_gate(&IVT_p[TIMER], (int)TimerEntry, get_cs(), ACC_INTR_GATE, 0);
  outportb(PIC_MASK,MASK);
  asm("sti");
  
  while(1) {
    if(cons_kbhit()) {
      character = cons_getchar();
      if(character == 'b') {
        breakpoint();
      } else {
        cons_printf("%c was pressed!!", character);
      }
    }
    for(i=0; i<LOOP; i++) {
      asm("inb $0x80");
    }
  }
  return 0;
}
