/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 4
 *  syscalls.h, CPE159
 ********************************************/

#ifndef __SYSCALLS__
#define __SYSCALLS__
//prototyping the routines in syscalls.c
void Sleep(int sec);
int GetPid(void);
void SetVideo(int row, int col);
void Write(int device, char *str);
int SemInit(int arg);
void SemWait(int arg);
void SemPost(int arg);
#endif
