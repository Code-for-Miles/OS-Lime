/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 
 *  entry.h, CPE159
 ********************************************/
// prototypes what're in entry.S

#ifndef __ENTRY__
#define __ENTRY__

#ifndef ASSEMBLER  // skip below if ASSEMBLER defined (from an assembly code)
                   // since below is not in assembler syntax
__BEGIN_DECLS

#include "types.h"                // TF_t

void TimerEntry(void);            // coded in entry.S, assembler won't like this syntax
void Loader(TF_t *);              // coded in entry.S
void SyscallEntry(void);          // coded in entry.S
void Term0Entry(void);
void Term1Entry(void);
__END_DECLS

#endif // ifndef ASSEMBLER

#endif // ifndef _ENTRY_H_

