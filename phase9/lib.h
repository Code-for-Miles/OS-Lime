/*******************************************
 *  Team Name: OSLime
 *  Members: Mulugeta Engdaw, Paul Makhnovskiy 
 *  Phase: 6
 *  lib.h, CPE159
 ********************************************/
#ifndef __LIB__
#define __LIB__

void Bzero(char *p, int bytes);
int QisEmpty(q_t *p);
int QisFull(q_t *p);
int DeQ(q_t *p);
int EnQ(int to_add, q_t *p);
int InQ(int ppid, q_t *p);
void DelQ(int id, q_t *p);
int StrCmp(char *s1, char *s2);
void MemCpy(char *dst, char *src, int size);
void ReclaimPages(int pid);
int Alloc(int pid, int how_many, int page_index[]);
#endif
