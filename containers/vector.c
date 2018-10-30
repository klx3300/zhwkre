#include "../vector.h"
#include <stdlib.h>
#include <string.h>

#define ZEROINIT(x) memset(&(x),0,sizeof(x))

qVectorDescriptor qVector_constructor(int elemsize){
    qVectorDescriptor desc;
    ZEROINIT(desc);
    desc.size = malloc(sizeof(int));
    if(desc.size == NULL){

    }else{
        *(desc.size) = 0;
        desc.elemstep = elemsize;
    }
    return desc;
}

int qVector__push_back(qVectorDescriptor* desc,void* elem){
    if(desc->allocated < (*(desc->size)+1)*desc->elemstep){
        // REALLOC
        void* tmprealloc = malloc(2*(desc->allocated+desc->elemstep));
        if(tmprealloc == NULL){
            
            return -1;
        }
        if(desc->content != NULL){
            memcpy(tmprealloc,desc->content,desc->allocated);
        }
        desc->allocated = 2*(desc->allocated+desc->elemstep);
        free(desc->content);
        desc->content = tmprealloc;
    }
    memcpy(desc->content+(*(desc->size))*desc->elemstep,elem,desc->elemstep);
    (*(desc->size))++;
    return 0;
}

int qVector__pop_back(qVectorDescriptor* desc){
    (*(desc->size))--;
    // shrink check
    if(desc->allocated > 2*(*(desc->size)*desc->elemstep) && desc->allocated/2 != 0){
        void* tmprealloc = malloc(desc->allocated/2);
        if(tmprealloc == NULL){
            
            return -1;
        }
        memcpy(tmprealloc,desc->content,(*(desc->size))*desc->elemstep);
        free(desc->content);
        desc->content = tmprealloc;
        desc->allocated=desc->allocated/2;
    }
    return 0;
}

int qVector_destructor(qVectorDescriptor desc){
    free(desc.content);
    free(desc.size);
    return 0;
}

qVectorIterator qVector_begin(qVectorDescriptor desc){
    qVectorIterator tmpiter;
    tmpiter.base = desc.content;
    tmpiter.count = 0;
    tmpiter.elemstep = desc.elemstep;
    tmpiter.limit = desc.size;
    return tmpiter;
}

qVectorIterator qVector_end(qVectorDescriptor desc){
    qVectorIterator tmpiter = qVector_begin(desc);
    tmpiter.count = (*tmpiter.limit)-1;
    return tmpiter;
}

int qVectorIterator_isvalid(qVectorIterator iter){
    return iter.count < *iter.limit && iter.count >= 0;
}

void* qVectorIterator_deref(qVectorIterator iter){
    return iter.base + (iter.count*iter.elemstep);
}

qVectorIterator qVectorIterator_move(qVectorIterator iter,int delta){
    qVectorIterator tmpiter = iter;
    tmpiter.count += delta;
    return tmpiter;
}

int qVector__erase(qVectorDescriptor* desc,qVectorIterator positional){
    void *buffer = malloc(desc->elemstep);
    if(buffer == NULL){
        
        return -1; // zhwkerr not here!
    }
    memcpy(buffer,qVectorIterator_deref(positional),desc->elemstep);
    qVectorIterator i;
    for(i = positional;
        qVectorIterator_isvalid(qVectorIterator_move(i,1));
        i = qVectorIterator_move(i,1)){
            memcpy(qVectorIterator_deref(i),
                   qVectorIterator_deref(qVectorIterator_move(i,1)),
                   desc->elemstep);
    }
    memcpy(qVectorIterator_deref(i),buffer,desc->elemstep);
    return qVector__pop_back(desc);
}

int qVectorIterator_diff(qVectorIterator a,qVectorIterator b){
    return a.count - b.count;
}
