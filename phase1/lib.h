/*************************************************
 *  NAME: Mulugeta Engdaw
 *  Phase1
 *  lib.h
 **************************************************/
#ifndef __LIB__
#define __LIB__

void Bzero(char *p, int bytes);
int QisEmpty(q_t *p);
int QisFull(q_t *p);
int DeQ(q_t *p);
int EnQ(int to_add, q_t *p);

#endif
