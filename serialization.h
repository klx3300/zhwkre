#ifndef Q_RE_SERIAL_H
#define Q_RE_SERIAL_H

#include "listd.h"
#include "bss.h"

#define YES_IT_IS_A_LIST 1
#define NO_IT_IS_NOT_A_LIST 0

// one-key serialize a list
struct q__ListDescriptor qSerialize(void* data,unsigned int len);

// so unserialize it
void* qUnserialize(struct q__ListDescriptor dataset,int isList);

#endif