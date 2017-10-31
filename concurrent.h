#ifndef Q_RE_CONCURRENT_H
#define Q_RE_CONCURRENT_H

#include <pthread.h>

typedef struct q__Thread{pthread_t threadno;} qThread;
typedef struct q__Mutex{pthread_mutex_t *mu;void (*lock)(struct q__Mutex qmu);void (*unlock)(struct q__Mutex qmu);} qMutex;

// create a thread and detach immediately
int qRun(void* (*func)(void* args),void* args);
// create a thread and remain joinable
qThread qStart(void* (*func)(void* args),void* args);
// detach a thread
int qDetach(qThread thr);
// wait a thread to complete(cant wait detached threads)
// the return value of this function is the void* func return.
void* qWait(qThread thr);

// Mutex support
qMutex qMutex_constructor();
void q__Mutex_lock(qMutex mu);
void q__Mutex_unlock(qMutex mu);
#define qMutex_destructor(mu) qMutex__destructor(&mu)
void qMutex__destructor(qMutex* mu);

#endif