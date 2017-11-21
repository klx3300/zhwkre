#ifndef Q_ZHWKRE_FORMAT_H
#define Q_ZHWKRE_FORMAT_H

#include <stdio.h>

#define Q_ESCAPE_CHAR "\x1b"
#define Q_FMT_APPLY(x) Q_ESCAPE_CHAR x

// colors
#define Q_COLOR_RESET "[0m"
#define Q_COLOR_RED "[31m"
#define Q_COLOR_GREEN "[32m"
#define Q_COLOR_YELLOW "[33m"
#define Q_COLOR_BLUE "[34m"
#define Q_COLOR_MAGENTA "[35m" // purple-like
#define Q_COLOR_CYAN "[36m"
#define Q_COLOR_WHITE "[37m"

// functions for better usage?
void qfmtColor(const char* color_const);
void qfmtClearColor();
// qfmtColorizer("[",FAIL,"]",Q_COLOR_RED) ==> try this out!
#define qfmtColorizer(prefix,colored,suffix,color) qfmtColorizerF(stdout,prefix,colored,suffix,color)
void qfmtColorizerF(FILE* dstfile,const char* prefix,const char* colored,const char* suffix,const char* color);

// const moves



#endif