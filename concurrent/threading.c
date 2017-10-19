#include "../concurrent.h"
#include <stdio.h>

qThread qStart(void* (*func)(void* args),void* args){
    qThread tmp;
    int errn;
    if((errn=pthread_create(&(tmp.threadno),NULL,func,args))){
        fprintf(stderr,"ThreadCreation Error:POSIX pthread_create() call failed with errno %d.\n",errn);
    }
    return tmp;
}

void qDetach(qThread thr){
    if(pthread_detach(thr.threadno)){
        fprintf(stderr,"ThreadDetach Error: POSIX pthread_detach() call failed.\n");
    }
}

void qRun(void* (*func)(void* args),void* args){
    int errn;
    if((errn=pthread_detach(qStart(func,args).threadno))){
        fprintf(stderr,"ThreadDetach Error:POSIX pthread_detach() call failed with errno %d.\n",errn);
    }
}

void* qWait(qThread thr){
    int errn;
    void* res=NULL;
    if((errn=pthread_join(thr.threadno,&res))){
        fprintf(stderr,"ThreadJoin Error:POSIX pthread_join() cal failed with errno %d.\n",errn);
    }
    return res;
}