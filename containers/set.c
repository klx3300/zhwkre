#include "../set.h"
#include "../error.h"
#include "../unidef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


qSetDescriptor qSet_constructor(qSetComparator cmp){
    return qAVLTree_constructor(cmp);
}

int qSet__ptr_at(qSetDescriptor desc,qSetIterator *iter,void* elem,unsigned int elemlen){
    return qAVLTree__ptr_at(desc,iter,elem,elemlen);
}

int qSet__insert(qSetDescriptor *desc,void* elem,unsigned int elemsize){
    return qAVLTree__insert(desc,elem,elemsize);
}

int qSet__erase(qSetDescriptor *desc,qSetIterator elem){
    return qAVLTree__erase(desc,elem);
}

int qSet__destructor(qSetDescriptor *desc){
    return qAVLTree__destructor(desc);
}

qSetIterator qSet_begin(qSetDescriptor desc){
    return qAVLTree_begin(desc);
}
qSetIterator qSet_end(qSetDescriptor desc){
    return qAVLTree_end(desc);
}

int qSetIterator_isvalid(qSetIterator iter){
    return qAVLTreeIterator_isvalid(iter);
}
void* qSetIterator_deref(qSetIterator iter){
    return qAVLTreeIterator_deref(iter);
}

qSetIterator qSetIterator_prev(qSetIterator iter){
    return qAVLTreeIterator_prev(iter);
}
qSetIterator qSetIterator_next(qSetIterator iter){
    return qAVLTreeIterator_next(iter);
}

qSetDescriptor qSet_union(qSetDescriptor seta,qSetDescriptor setb){
    qSetDescriptor desc=qSet_constructor(seta.comp);
    for(qSetIterator iter = qSet_begin(seta);qSetIterator_isvalid(iter);iter=qSetIterator_next(iter)){
        qSet__insert(&desc,qSetIterator_deref(iter),iter.node->size);
    }
    for(qSetIterator iter = qSet_begin(setb);qSetIterator_isvalid(iter);iter=qSetIterator_next(iter)){
        qSet__insert(&desc,qSetIterator_deref(iter),iter.node->size);
    }
    return desc;
}
qSetDescriptor qSet_intersect(qSetDescriptor seta,qSetDescriptor setb){
    qSetDescriptor desc = qSet_constructor(seta.comp);
    for(qSetIterator iter = qSet_begin(seta);qSetIterator_isvalid(iter);iter=qSetIterator_next(iter)){
        if(!qSet__ptr_at(setb,NULL,qSetIterator_deref(iter),iter.node->size)){
            qSet__insert(&desc,qSetIterator_deref(iter),iter.node->size);
        }
    }
    return desc;
}
qSetDescriptor qSet_difference(qSetDescriptor seta,qSetDescriptor setb){
    qSetDescriptor desc = qSet_constructor(seta.comp);
    for(qSetIterator iter = qSet_begin(seta);qSetIterator_isvalid(iter);iter=qSetIterator_next(iter)){
        if(qSet__ptr_at(setb,NULL,qSetIterator_deref(iter),iter.node->size)){
            qSet__insert(&desc,qSetIterator_deref(iter),iter.node->size);
        }
    }
    return desc;
}
int qSet_isequal(qSetDescriptor seta, qSetDescriptor setb){
    if(seta.size != setb.size) return 0;
    for(qSetIterator iter = qSet_begin(seta);qSetIterator_isvalid(iter);iter=qSetIterator_next(iter)){
        if(qSet__ptr_at(setb,NULL,qSetIterator_deref(iter),iter.node->size)){
            return 0;
        }
    }
    return 1;
}
int qSet_issubset(qSetDescriptor seta,qSetDescriptor setb){
    for(qSetIterator iter = qSet_begin(seta);qSetIterator_isvalid(iter);iter=qSetIterator_next(iter)){
        if(qSet__ptr_at(setb,NULL,qSetIterator_deref(iter),iter.node->size)){
            return 0;
        }
    }
    return 1;
}