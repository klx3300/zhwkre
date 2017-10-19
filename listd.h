#ifndef Q_RE_LIST_DEF_H
#define Q_RE_LIST_DEF_H

#define Q__LD_MAGIC "__listdesc"

struct q__List{
    void* data;
    unsigned int size;
    struct q__List *prev;
    struct q__List *next;
};
struct q__ListDescriptor{
    char ldmagic[10];
    struct q__List* head;
    struct q__List* tail;
    unsigned int size;
};

typedef struct q__List *qListIterator;
typedef struct q__ListDescriptor qListDescriptor;


#endif