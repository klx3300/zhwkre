#include "../concurrent.h"
#include <stdio.h>

qThread qStart(void* (*func)(void* args),void* args){
    qThread tmp;
    int errn;
    if((errn=pthread_create(&(tmp.threadno),NULL,func,args))==-1){
    }
    return tmp;
}

int qDetach(qThread thr){
    int errn;
    if((errn=pthread_detach(thr.threadno))==-1){
    }
    return errn;
}

int qRun(void* (*func)(void* args),void* args){
    zhwk_error = 0;
    int errn;
    qThread tmpthr = qStart(func,args);
    if(zhwk_error){
        return -1;
    }
    errn = qDetach(tmpthr);
    return errn;
}

void* qWait(qThread thr){
    int errn;
    void* res=NULL;
    if((errn=pthread_join(thr.threadno,&res))==-1){
    }
    return res;
}
