#include "../format.h"
#include <stdio.h>

void qfmtColor(const char* c){
    printf("\x1b%s",c);
}

void qfmtClearColor(){
    qfmtColor(Q_COLOR_RESET);
}

void qfmtColorizerF(FILE* f,const char *prefix,const char* colored,const char* suffix,const char* c){
    fprintf(f,"%s\x1b%s%s\x1b%s%s",prefix,c,colored,Q_COLOR_RESET,suffix);
}