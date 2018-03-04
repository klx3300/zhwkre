#ifndef Q_ZHWKRE_UNIDEF_H
#define Q_ZHWKRE_UNIDEF_H


// includes some useful stuff..

typedef unsigned int ui;

#define PTR(x) (&(x))
#define VPTR(x) ((void*)(&(x)))
#define DEREF(x) (*(x))
#define ZEROINIT(x) memset(&(x),0,sizeof(x))

#define MAX(x,y) ({\
    typeof(x) xrep = (x);\
    typeof(y) yrep = (y);\
    xrep >= yrep ? xrep : yrep;\
})
#define MIN(x,y) ({\
    typeof(x) xrep = (x);\
    typeof(y) yrep = (y);\
    xrep <= yrep ? xrep : yrep;\
})

#define OFFSET(type,member) (&((type*)0)->member)

#endif