/*******************************************
 *  Team Name: OSLime
 *  Name: Mulugeta Engdaw
 *  Phase: 2
 *  isr.h, CPE159
 ********************************************/
#ifndef __ISR__
#define __ISR__

void NewProcISR(func_p_t p);
void TimerISR(void);
void GetPidISR(void);
void SleepISR(void);
void SetVideoISR(void);
void WriteISR(void);

#endif
