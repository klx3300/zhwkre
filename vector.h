#ifndef Q_ZHWKRE_VECTOR_H
#define Q_ZHWKRE_VECTOR_H

struct q__vector_desc_st{
    void* content;
    int* size;
    int elemstep;
    int allocated;
};

struct q__vector_iter_st{
    void* base;
    int* limit;
    int count;
    int elemstep;
};

typedef struct q__vector_desc_st qVectorDescriptor;
typedef struct q__vector_iter_st qVectorIterator;

qVectorDescriptor qVector_constructor(int elemsize);

#define qVector_push_back(desc,elem) qVector__push_back(&(desc),&(elem))
int qVector__push_back(qVectorDescriptor* desc,void* elem);

#define qVector_pop_back(desc) qVector__pop_back(&(desc))
int qVector__pop_back(qVectorDescriptor* desc);
#define qVector_erase(desc,iter) qVector__erase(&(desc),iter)
int qVector__erase(qVectorDescriptor* desc,qVectorIterator positional);

// dont use that again! all memory released!
int qVector_destructor(qVectorDescriptor desc);

// ---- iterator operations ----

qVectorIterator qVector_begin(qVectorDescriptor desc);
qVectorIterator qVector_end(qVectorDescriptor desc);

int qVectorIterator_isvalid(qVectorIterator iter);

void* qVectorIterator_deref(qVectorIterator iter);

qVectorIterator qVectorIterator_move(qVectorIterator iter,int delta);

int qVectorIterator_diff(qVectorIterator a,qVectorIterator b);

#endif