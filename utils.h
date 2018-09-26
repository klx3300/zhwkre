#ifndef Q_RE_UTILS_H
#define Q_RE_UTILS_H
#include "bss.h"
#include <stdio.h>

int fullstrcmp(const char *a,const char* b);
void printSomeSpaces(unsigned int spcs);
int find_byte(const char* str,const char target,int size);
int partstrcmp(const char *a,const char* b);

// this comparator can be effectively use for comps.
int string_comparator(void* stra,unsigned int sizea,void* strb,unsigned int sizeb);

// read from files!
qBinarySafeString readline(FILE* fp);
qBinarySafeString readall(FILE* fp);

#endif
