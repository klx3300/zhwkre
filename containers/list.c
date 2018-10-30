#include "../list.h"
#include <stdlib.h>
#include <string.h>
typedef void (*tmpfncptr)();
static qListIterator q__List_new(){
    qListIterator tmp = (qListIterator)malloc(sizeof(struct q__List));
    if(tmp==NULL){
        return NULL;
    }
    tmp->data = tmp->prev = tmp->next = NULL;
    tmp->size = 0;
    return tmp;
}

static void q__List_initdesc(void* desc){
    struct q__ListDescriptor *ld = (qListDescriptor*)desc;
    memcpy(ld->ldmagic,Q__LD_MAGIC,10);
    ld->head = NULL;
    ld->tail = NULL;
    ld->size = 0;
}

qListDescriptor qList_constructor(){
    qListDescriptor desc;
    q__List_initdesc(&desc);
    return desc;
}

int q__List_push_back(void* descriptor,void* target,unsigned int size){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->tail==NULL){
        // empty list.give one.
        desc->head=q__List_new();
        if(desc->head == NULL){
            return -1;
        }
        desc->tail=desc->head;
        desc->tail->data=malloc(size);
        if(desc->tail->data == NULL){
            return -1;
        }
        memcpy(desc->tail->data,target,size);
        desc->tail->size = size;
        desc->size++;
    }else{
        desc->tail->next=q__List_new();
        if(desc->tail->next == NULL){
            return -1;
        }
        desc->tail->next->prev=desc->tail;
        desc->tail->next->data=malloc(size);
        if(desc->tail->next->data == NULL){
            return -1;
        }
        memcpy(desc->tail->next->data,target,size);
        desc->tail->next->size = size;
        desc->tail=desc->tail->next;
        desc->size++;
    }
    return 0;
}

int q__List_push_front(void* descriptor,void* target,unsigned int size){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->tail==NULL){
        // empty list.give one.
        desc->head=q__List_new();
        if(desc->head == NULL){
            return -1;
        }
        desc->tail=desc->head;
        desc->tail->data=malloc(size);
        if(desc->tail->data == NULL){
            return -1;
        }
        memcpy(desc->tail->data,target,size);
        desc->tail->size = size;
        desc->size++;
    }else{
        desc->head->prev=q__List_new();
        if(desc->head == NULL){
            return -1;
        }
        desc->head->prev->next=desc->head;
        desc->head->prev->data=malloc(size);
        if(desc->head->prev->data == NULL){
            return -1;
        }
        memcpy(desc->head->prev->data,target,size);
        desc->head->prev->size = size;
        desc->head=desc->head->prev;
        desc->size++;
    }
    return 0;
}

int q__List_pop_back(void* descriptor){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->tail == NULL){
        return -1;
    };
    if(desc->tail==desc->head){
        // he is the only.
        free(desc->tail->data);
        free(desc->tail);
        desc->tail=NULL;
        desc->head=NULL;
        desc->size--;
    }else{
        // good news encountered.
        desc->tail=desc->tail->prev;
        free(desc->tail->next->data);
        free(desc->tail->next);
        desc->tail->next=NULL;
        desc->size--;
    }
    return 0;
}

int q__List_pop_front(void* descriptor){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->head==NULL){
        return -1;
    };
    if(desc->tail==desc->head){
        // he is the only.
        free(desc->tail->data);
        free(desc->tail);
        desc->tail=NULL;
        desc->head=NULL;
        desc->size--;
    }else{
        // good news encountered.
        desc->head=desc->head->next;
        free(desc->head->prev->data);
        free(desc->head->prev);
        desc->head->prev=NULL;
        desc->size--;
    }
    return 0;
}

int q__List_swap_elem(void* descriptor,qListIterator a,qListIterator b){
    void* tmp=a->data;
    unsigned int tmpsize = a->size;
    a->data=b->data;
    a->size=b->size;
    b->data=tmp;
    b->size=tmpsize;
    return 0;
}

int q__List_erase_elem(void* descriptor,qListIterator a){
    qListDescriptor* desc=(qListDescriptor*)descriptor;
    if(a->prev!=NULL) a->prev->next = a->next;
    if(a->next!=NULL) a->next->prev = a->prev;
    if(desc->head == a) desc->head=a->next;
    if(desc->tail == a) desc->tail=a->prev;
    desc->size--;
    free(a->data);
    return 0;
}

int q__List_destructor(void* descriptor){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    qListIterator iterator=desc->head;
    while(iterator!=NULL){
        qListIterator tmp=iterator->next;
        free(iterator->data);
        free(iterator);
        iterator=tmp;
    }
    desc->head = desc->tail = NULL;
    desc->size=0;
    return 0;
}

int q__List_islist(void* descriptor,unsigned int size){
    if(size < sizeof(struct q__ListDescriptor)){
        return 0;
    }
    if(strncmp(((struct q__ListDescriptor*)(descriptor))->ldmagic,Q__LD_MAGIC,strlen(Q__LD_MAGIC))){
        return 0;
    }
    return 1;
}

qListDescriptor q__List_copy(struct q__ListDescriptor *desc){
    qListDescriptor rdesc=qList_constructor();
    qList_foreach(*desc,iter){
        q__List_push_back(&rdesc,iter->data,iter->size);
    }
    return rdesc;
}

qListIterator qList_begin(qListDescriptor desc){
    return desc.head;
}
qListIterator qList_end(qListDescriptor desc){
    return desc.tail;
}

qListIterator qListIterator_prev(qListIterator iter){
    return iter->prev;
}

qListIterator qListIterator_next(qListIterator iter){
    return iter->next;
}

int qListIterator_isvalid(qListIterator iter){
    return iter!=NULL;
}

void* qListIterator_deref(qListIterator iter){
    return iter->data;
}
