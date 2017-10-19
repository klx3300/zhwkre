#ifndef Q_RE_LIST_H
#define Q_RE_LIST_H

#include "listd.h"
#include <stdlib.h>

#define qList(typename,varname) struct{char ldmagic[10];\
                                       struct q__List* head;\
                                       struct q__List* tail;\
                                       unsigned int size;\
                                       } varname;\
                                       q__List_initdesc(&varname);

#define qBareList(typename) struct{char ldmagic[10];\
                                   struct q__List* head;\
                                   struct q__List* tail;\
                                   unsigned int size;}

// the correct way to init a listdesc ..
// for barelist condition
#define qList_initdesc(desc) q__List_initdesc(&(desc))
void q__List_initdesc(void* descriptor);

#define qList_push_back(desc,target) q__List_push_back(&(desc),&(target),sizeof(target))
void q__List_push_back(void* descriptor,void* target,unsigned int target_size);
#define qList_push_front(desc,target) q__List_push_front(&(desc),&(target),sizeof(target))
void q__List_push_front(void* descriptor,void* target,unsigned int target_size);
#define qList_pop_back(desc) q__List_pop_back(&(desc))
void q__List_pop_back(void* descriptor);
#define qList_pop_front(desc) q__List_pop_front(&(desc))
void q__List_pop_front(void* descriptor);
#define qList_erase_elem(desc,iter) q__List_erase_elem(&(desc),iter)
void q__List_erase_elem(void* descriptor,qListIterator iter);
#define qList_swap_elem(desc,itera,iterb) q__List_swap_elem(&(desc),itera,iterb)
void q__List_swap_elem(void* descriptor,qListIterator itera,qListIterator iterb);
// notice: unlike cpp behaivor, the destructor here plays the role of
// clear the whole list and set the necessary parameters to its starting status.
#define qList_destructor(desc) q__List_destructor(&desc)
void q__List_destructor(void* descriptor);

#define qList_foreach(desc,itername) for(qListIterator itername=(desc).head;itername != NULL;itername = itername->next)

#define qList_islist(desc) q__List_islist(&(desc),sizeof(desc))
int q__List_islist(void* descriptor,unsigned int size);

#define qList_copy(desc) q__List_copy(&(desc))
qListDescriptor q__List_copy(struct q__ListDescriptor *desc);

#endif