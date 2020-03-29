/**********************************************
  NAME: Mulugeta Engdaw
  Phase 0, Prep 4, Timer Device
  entry.S
***********************************************/
#include <spede/machine/asmacros.h>

ENTRY(TimerEntry)
  pusha
  cld
  call CNAME(TimerCode)
  popa
  iret

