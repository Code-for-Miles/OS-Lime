/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  constants.h, CPE159
 ********************************************/
// constants.h, 159
// /gaia/home/project/spede2/Target-i386/Common/include/spede/machine

#ifndef __CONSTANTS__
#define __CONSTANTS__

#define VGA_MASK 0x0f00     // foreground bright white, background black
#define TIMER 32            // TIMER constant identifier
#define PIC_MASK 0x21       // Programmable Interrupt Controller I/O
#define MASK ~0x19          // mask for Programmable Interrupt Controller
#define PIC_CONTROL 0x20    // Programmable Interrupt Controller I/O
#define DONE 0x60

#define LOOP 166666          // slow: 1666666 times calling asm("inb $0x80");
#define TIME_MAX 350         // max timer count, then rotate process
#define PROC_MAX 20          // max number of processes
#define Q_SIZE 20            // queuing capacity
#define STACK_SIZE 4096     // process stack in bytes

#define HOME_POS  (unsigned short *)0xb8000
#define END_POS  (unsigned short *)(0xb8000+(24*160))

#define SLEEP  162
#define GETPID 20
#define STDOUT 1

#define READ 3
#define WRITE 4
#define SETVIDEO 100
#define SYSCALL 128

#define SEM_MAX 20
#define SEMINIT 101
#define SEMWAIT 102
#define SEMPOST 103

//#define IO_MASK ~0x19  
#define TERM_MAX 2    //num of terminal ports
#define TERM0 35
#define TERM1 36
#define TERM0_IO 0x2f8 // same as COM2_IOBASE from rs232.h
#define TERM1_IO 0x3e8 // same as COM3_IOBASE from rs232.h
#define TERM0_DONE 0x63
#define TERM1_DONE 0x64

#define BUFF_SIZE 101

#define SIGNAL 48
#define SIGINT 2

#define FORK 2
#define GETPPID 64
#define CTRL_C 0x3

#endif
