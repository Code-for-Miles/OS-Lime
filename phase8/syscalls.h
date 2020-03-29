/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  syscalls.h, CPE159
 ********************************************/
#ifndef __SYSCALLS__
#define __SYSCALLS__
#include "types.h"
//prototyping the routines in syscalls.c
void Sleep(int sec);
int GetPid(void);
void SetVideo(int row, int col);
void Write(int device, char *str);
void Read(int device, char *buff);
int SemInit(int arg);
void SemWait(int arg);
void SemPost(int arg);
void Signal(int, func_p_t);
int GetPpid(void);
int Fork(void);
int Wait(int *ec_p);
void Exit(int ec);
#endif
