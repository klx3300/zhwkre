#ifndef Q_ZHWKRE_SET_H
#define Q_ZHWKRE_SET_H
#include "AVLTree.h"

typedef qAVLTreeDescriptor qSetDescriptor;
typedef qAVLTreeIterator qSetIterator;
typedef int (*qSetComparator)(void*,unsigned int,void*,unsigned int);

qSetDescriptor qSet_constructor(qSetComparator cmp);

#define qSet_ptr_at(desc,iter,k) qSet__ptr_at(desc,&(iter),&(k),sizeof(k))
int qSet__ptr_at(qSetDescriptor desc,qSetIterator *iter,void* elem,unsigned int elemlen);

#define qSet_insert(desc,elem) qSet__insert(&(desc),&(elem),sizeof(elem))
int qSet__insert(qSetDescriptor *desc,void* elem,unsigned int elemsize);

#define qSet_erase(desc,iter) qSet__erase(&(desc),iter)
int qSet__erase(qSetDescriptor *desc,qSetIterator elem);

#define qSet_destructor(desc) qSet__destructor(&(desc))
int qSet__destructor(qSetDescriptor *desc);

qSetIterator qSet_begin(qSetDescriptor desc);
qSetIterator qSet_end(qSetDescriptor desc);

int qSetIterator_isvalid(qSetIterator iter);
void* qSetIterator_deref(qSetIterator iter);

qSetIterator qSetIterator_prev(qSetIterator iter);
qSetIterator qSetIterator_next(qSetIterator iter);

// extra set operations
qSetDescriptor qSet_union(qSetDescriptor seta,qSetDescriptor setb);
qSetDescriptor qSet_intersect(qSetDescriptor seta,qSetDescriptor setb);
qSetDescriptor qSet_difference(qSetDescriptor seta,qSetDescriptor setb);
int qSet_isequal(qSetDescriptor seta, qSetDescriptor setb);
// is a the subset of b?
int qSet_issubset(qSetDescriptor seta,qSetDescriptor setb);

#endif