/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  proc.h, CPE159
 ********************************************/
#ifndef __PROC__
#define __PROC__
 
void PidStr(int pid, char *str); 
void InitProc(void);
void UserProc(void);
void CarProc(void);
void TermProc(void);
void Ouch(void);
void PidStr(int pid, char *str);
void Wrapper(func_p_t);
void ChildCode(int);
void ChldHandler();
#endif
