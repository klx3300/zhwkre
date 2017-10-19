#include "../list.h"
#include <stdlib.h>
#include <string.h>
typedef void (*tmpfncptr)();
qListIterator q__List_new(){
    qListIterator tmp = (qListIterator)malloc(sizeof(struct q__List));
    tmp->data = tmp->prev = tmp->next = NULL;
    tmp->size = 0;
    return tmp;
}

void q__List_initdesc(void* desc){
    struct q__ListDescriptor *ld = (qListDescriptor*)desc;
    memcpy(ld->ldmagic,Q__LD_MAGIC,10);
    ld->head = NULL;
    ld->tail = NULL;
    ld->size = 0;
}

void q__List_push_back(void* descriptor,void* target,unsigned int size){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->tail==NULL){
        // empty list.give one.
        desc->head=q__List_new();
        desc->tail=desc->head;
        desc->tail->data=malloc(size);
        memcpy(desc->tail->data,target,size);
        desc->tail->size = size;
        desc->size++;
    }else{
        desc->tail->next=q__List_new();
        desc->tail->next->prev=desc->tail;
        desc->tail->next->data=malloc(size);
        memcpy(desc->tail->next->data,target,size);
        desc->tail->next->size = size;
        desc->tail=desc->tail->next;
        desc->size++;
    }
}

void q__List_push_front(void* descriptor,void* target,unsigned int size){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->tail==NULL){
        // empty list.give one.
        desc->head=q__List_new();
        desc->tail=desc->head;
        desc->tail->data=malloc(size);
        memcpy(desc->tail->data,target,size);
        desc->tail->size = size;
        desc->size++;
    }else{
        desc->head->prev=q__List_new();
        desc->head->prev->next=desc->head;
        desc->head->prev->data=malloc(size);
        memcpy(desc->head->prev->data,target,size);
        desc->head->prev->size = size;
        desc->head=desc->head->prev;
        desc->size++;
    }
}

void q__List_pop_back(void* descriptor){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->tail == NULL) return;
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
}

void q__List_pop_front(void* descriptor){
    struct q__ListDescriptor* desc = (struct q__ListDescriptor*)descriptor;
    if(desc->head==NULL) return;
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
}

void q__List_swap_elem(void* descriptor,qListIterator a,qListIterator b){
    void* tmp=a->data;
    unsigned int tmpsize = a->size;
    a->data=b->data;
    a->size=b->size;
    b->data=tmp;
    b->size=tmpsize;
}

void q__List_erase_elem(void* descriptor,qListIterator a){
    qListDescriptor* desc=(qListDescriptor*)descriptor;
    if(a->prev!=NULL) a->prev->next = a->next;
    if(a->next!=NULL) a->next->prev = a->prev;
    if(desc->head == a) desc->head=a->next;
    if(desc->tail == a) desc->tail=a->prev;
    desc->size--;
    free(a->data);
}

void q__List_destructor(void* descriptor){
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
    qListDescriptor rdesc;
    qList_initdesc(rdesc);
    qList_foreach(*desc,iter){
        q__List_push_back(&rdesc,iter->data,iter->size);
    }
    return rdesc;
}