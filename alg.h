#ifndef Q_RE_ALG_H
#define Q_RE_ALG_H

#include "listd.h"

// return >0 only if a>b ==> sort by increment.
typedef int (*Comparator)(qListIterator a,qListIterator b);

void bubble_sort(qListDescriptor *desc,Comparator cmpr);

#endif