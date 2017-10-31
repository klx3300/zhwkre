#include "../concurrent.h"
#include "../error.h"
#include <stdio.h>
#include <stdlib.h>

qMutex qMutex_constructor(){
    qMutex mu;
    int errn;
    mu.mu = malloc(sizeof(pthread_mutex_t));
    if(mu.mu == NULL){
        SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
        return mu;
    }
    if((errn=pthread_mutex_init((mu.mu),NULL)) == -1){
        SETERR(ZHWK_ERR_MUTEX_INIT_FAIL);
    }
    mu.lock=q__Mutex_lock;
    mu.unlock=q__Mutex_unlock;
    return mu;
}

void q__Mutex_lock(qMutex mu){
    int errn;
    if((errn=pthread_mutex_lock(mu.mu))==-1){
        SETERR(ZHWK_ERR_MUTEX_LOCK_FAIL);
    }
}

void q__Mutex_unlock(qMutex mu){
    int errn;
    if((errn=pthread_mutex_unlock(mu.mu))==-1){
        SETERR(ZHWK_ERR_MUTEX_RELEASE_FAIL);
    }
}

void qMutex__destructor(qMutex* mu){
    int errn;
    if((errn=pthread_mutex_destroy(mu->mu))==-1){
        SETERR(ZHWK_ERR_MUTEX_DESTROY_FAIL);
    }
    free(mu->mu);
    mu->mu=NULL;
}