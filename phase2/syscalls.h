/*******************************************
 *  Team Name: OSLime
 *  Name: Mulugeta Engdaw
 *  Phase: 2
 *  syscalls.h, CPE159
 ********************************************/

#ifndef __SYSCALLS__
#define __SYSCALLS__
//prototyping the routines in syscalls.c
void Sleep(int sec);
int GetPid(void);
void SetVideo(int row, int col);
void Write(int device, char *str);
#endif
