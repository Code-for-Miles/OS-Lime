/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  proc.h, CPE159
 ********************************************/
#ifndef __PROC__
#define __PROC__
 
void InitProc(void);
void UserProc(void);
void CarProc(void);
void TermProc(void);
void Ouch(void);
void Wrapper(func_p_t);
void ChildCode(void);
char *PidStr(int pid, char *str);
#endif
