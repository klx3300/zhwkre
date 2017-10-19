#include "../concurrent.h"
#include <stdio.h>
#include <stdlib.h>

qMutex qMutex_constructor(){
    qMutex mu;
    int errn;
    mu.mu = malloc(sizeof(pthread_mutex_t));
    if((errn=pthread_mutex_init((mu.mu),NULL))){
        fprintf(stderr,"MutexConstruct Error:POSIX call pthread_mutex_init failed with errno %d\n",errn);
    }
    mu.lock=q__Mutex_lock;
    mu.unlock=q__Mutex_unlock;
    return mu;
}

void q__Mutex_lock(qMutex mu){
    int errn;
    if((errn=pthread_mutex_lock(mu.mu))){
        fprintf(stderr,"MutexAcquire Error:POSIX call pthread_mutex_lock failed with errno %d.\n",errn);
    }
}

void q__Mutex_unlock(qMutex mu){
    int errn;
    if((errn=pthread_mutex_unlock(mu.mu))){
        fprintf(stderr,"MutexRelease Error:POSIX call pthread_mutex_unlock failed with errno %d.\n",errn);
    }
}

void qMutex__destructor(qMutex* mu){
    int errn;
    if((errn=pthread_mutex_destroy(mu->mu))){
        fprintf(stderr,"MutexDestroy Error:POSIX call pthread_mutex_destroy failed with errno %d.\n",errn);
    }
    free(mu->mu);
    mu->mu=NULL;
}