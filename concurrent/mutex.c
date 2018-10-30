#include "../concurrent.h"
#include <stdio.h>
#include <stdlib.h>

qMutex qMutex_constructor(){
    qMutex mu;
    int errn;
    mu.mu = malloc(sizeof(pthread_mutex_t));
    if(mu.mu == NULL){
        return mu;
    }
    if((errn=pthread_mutex_init((mu.mu),NULL)) == -1){
    }
    mu.lock=q__Mutex_lock;
    mu.unlock=q__Mutex_unlock;
    return mu;
}

void q__Mutex_lock(qMutex mu){
    int errn;
    if((errn=pthread_mutex_lock(mu.mu))==-1){
    }
}

void q__Mutex_unlock(qMutex mu){
    int errn;
    if((errn=pthread_mutex_unlock(mu.mu))==-1){
    }
}

void qMutex__destructor(qMutex* mu){
    int errn;
    if((errn=pthread_mutex_destroy(mu->mu))==-1){
    }
    free(mu->mu);
    mu->mu=NULL;
}
