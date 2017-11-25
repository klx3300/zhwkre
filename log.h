#ifndef Q_ZHWKRE_LOG_H
#define Q_ZHWKRE_LOG_H

#include <stdio.h>
#include "format.h"
#include <time.h>
// this file totally written in macros.
// usage example:
// #define Q_LOG_LOGLEVEL 4
// #include "zhwkre/log.h"

#ifndef Q_LOG_LOGLEVEL
#define Q_LOG_LOGLEVEL 0
#endif

// these are pre-defined loglevels

#define Q_LOG_FAIL 40
#define Q_LOG_WARN 30
#define Q_LOG_SUCC 20
#define Q_LOG_INFO 10
#define Q_LOG_DEBUG 0

#ifndef qLog_TIME_FMT_STR
#define qLog_TIME_FMT_STR "(%.2d/%.2d/%.4d %.2d:%.2d:%.2d) "
#endif

#define qLogCurrTime() do{time_t rawtv;struct tm* timerep;\
time(&rawtv);timerep = localtime(&rawtv);\
fprintf(stderr,qLog_TIME_FMT_STR,timerep->tm_mon,timerep->tm_mday,\
timerep->tm_year+1900,timerep->tm_hour,timerep->tm_min,timerep->tm_sec);}while(0)

#define qLog(lvl,tag,color,str) do{if(Q_LOG_LOGLEVEL <= lvl){\
qfmtColorizerF(stderr,"[",tag,"]",color);qLogCurrTime();fprintf(stderr,str);fprintf(stderr,"\n");}}while(0)
#define qLogfmt(lvl,tag,color,fmtstr,...) do{if(Q_LOG_LOGLEVEL <= lvl){\
qfmtColorizerF(stderr,"[",tag,"]",color);qLogCurrTime();fprintf(stderr,fmtstr,__VA_ARGS__);fprintf(stderr,"\n");}}while(0)

#define qLogDebug(str) qLog(Q_LOG_DEBUG,"DEBG",Q_COLOR_WHITE,str)
#define qLogDebugfmt(fmtstr,...) qLogfmt(Q_LOG_DEBUG,"DEBG",Q_COLOR_WHITE,fmtstr,__VA_ARGS__)
#define qLogInfo(str) qLog(Q_LOG_INFO,"INFO",Q_COLOR_RESET,str)
#define qLogInfofmt(fmtstr,...) qLogfmt(Q_LOG_INFO,"INFO",Q_COLOR_RESET,fmtstr,__VA_ARGS__)
#define qLogSucc(str) qLog(Q_LOG_SUCC,"SUCC",Q_COLOR_GREEN,str)
#define qLogSuccfmt(fmtstr,...) qLogfmt(Q_LOG_SUCC,"SUCC",Q_COLOR_GREEN,fmtstr,__VA_ARGS__)
#define qLogWarn(str) qLog(Q_LOG_WARN,"WARN",Q_COLOR_YELLOW,str)
#define qLogWarnfmt(fmtstr,...) qLogfmt(Q_LOG_WARN,"WARN",Q_COLOR_YELLOW,fmtstr,__VA_ARGS__)
#define qLogFail(str) qLog(Q_LOG_FAIL,"FAIL",Q_COLOR_RED,str)
#define qLogFailfmt(fmtstr,...) qLogfmt(Q_LOG_FAIL,"FAIL",Q_COLOR_RED,fmtstr,__VA_ARGS__)

#endif