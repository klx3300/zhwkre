#include "../serialization.h"

#include "../list.h"

#include "../unordered_map.h"

#include <string.h>

static struct q__ListDescriptor q__list_serialize(struct q__ListDescriptor desc,qBinarySafeString prefix){
    struct q__ListDescriptor rdesc = qList_constructor();
    // construct buffer bss
    qBinarySafeString buffer = qbss_constructor(); // initialize bss
    qList_push_back(rdesc,buffer); // only copies the pointer value
    // replacing with qList_push_back(rdesc,qbss_constructor()) is invalid
    // iterating through the whole list..
    unsigned int counter = 0; // for recording the item number
    qList_foreach(desc,iter){
        // first, append the prefix specified in parameter..
        q__bss_append(rdesc.head->data,prefix.str,prefix.size);
        // then the counter
        q__bss_append(rdesc.head->data,(char*)&counter,sizeof(counter));
        // then the stuff we wanted to serialize
        // to keep binary safe, a leading size spec is needed.
        q__bss_append(rdesc.head->data,(char*)&(iter->size),sizeof(iter->size));
        // thanks to the pointer cast that take much more time from me..
        q__bss_append(rdesc.head->data,iter->data,iter->size);
        // is the data refer to another qList?
        // let's find that out..
        if(!q__List_islist(iter->data,iter->size)){
            counter++;continue;
        }
        // so you are really a listdesc,right? okay..maybe i can help you
        struct q__ListDescriptor* idesc = iter->data;
        // construct the next prefix
        qBinarySafeString tmpiprefix = qbss_constructor();
        qbss_append(tmpiprefix,prefix.str,prefix.size);
        qbss_append(tmpiprefix,(char*)&counter,sizeof(counter));
        struct q__ListDescriptor irdesc = q__list_serialize(*idesc,tmpiprefix);
        // free prefix
        qbss_destructor(tmpiprefix);
        // append the recursive result to already-exist retdesc
        int retcnt = 1;
        qList_foreach(irdesc,appiter){
            qListIterator rdescit=rdesc.head;
            for(int nextcntr=0;nextcntr<retcnt;nextcntr++){
                rdescit = rdescit->next;
            }
            // if null,create(append)
            if(rdescit == NULL){
                qList_push_back(rdesc,*((qBinarySafeString*)(appiter->data)));
            }else{
                // if not null, bss-append
                q__bss_append(rdescit->data,((qBinarySafeString*)(appiter->data))->str,((qBinarySafeString*)(appiter->data))->size);
            }
            retcnt++;
        }
        counter++;
    }
    return rdesc;
}

struct q__ListDescriptor qSerialize(void* data,unsigned int len){
    if(q__List_islist(data,len)){
        return q__list_serialize(*((struct q__ListDescriptor*)data),qbss_constructor());
    }else{
        struct q__ListDescriptor rdesc = qList_constructor();
        qBinarySafeString tmpbss = qbss_constructor();
        qbss_append(tmpbss,data,len);
        qList_push_back(rdesc,tmpbss);
        return rdesc;
    }
}

// not proven very effective.
static unsigned int q__uhashf(void* str,unsigned int size){
    unsigned int tmpr = 0;
    unsigned char* cstr = str;
    for(unsigned int i=0;i<size;i++){
        tmpr+=cstr[i];
        tmpr*=131;
    }
    return tmpr%131;
}

static struct q__ListDescriptor q__list_unserialize(struct q__ListDescriptor dataset,qMap *previous_data){
    // first, read all elements storaged in the last dataset.
    qBinarySafeString* lbss = dataset.tail->data;
    qMap lists = qMap_constructor(Q_DEFAULT_MAXHASHV);
    qListDescriptor itemslist = qList_constructor();
    unsigned int prefix_num = dataset.size - 1;
    for(char* stringiter=lbss->str;
        stringiter != lbss->str+(lbss->size);/*do nothing*/){
        qBinarySafeString tmprefix = qbss_constructor();
        if(prefix_num >0){
            // append the prefix
            qbss_append(tmprefix,stringiter,prefix_num*sizeof(unsigned int));
            stringiter += prefix_num*sizeof(unsigned int);
        }
        // unserialize the data
        unsigned int tmpcnt = *(unsigned int*)stringiter; // take back the counter record --
        stringiter += sizeof(unsigned int); // skip the counter
        unsigned int tmpsize = *(unsigned int*)stringiter; // deref the size part
        stringiter += sizeof(unsigned int); // skip the size
        char *tmpdata = malloc(tmpsize);
        memset(tmpdata,0,tmpsize);
        memcpy(tmpdata,stringiter,tmpsize);
        stringiter += tmpsize; // next, next
        // which level i am?
        if(previous_data == NULL && prefix_num == 0){
            // i am the single. both bottom and top.
            q__List_push_back(&itemslist,tmpdata,tmpsize);
        }else{
            if(previous_data != NULL){
                // i have little sisters. take them back.
                qListDescriptor *tmpldrefr = (qListDescriptor*)tmpdata;
                qListDescriptor *tmpsister = NULL;
                if(prefix_num == 0){
                    qMapData *tmpmd = q__Map_ptr_at(previous_data,&tmpcnt,sizeof(unsigned int),q__uhashf);
                    if(tmpmd != NULL){
                        tmpsister = tmpmd->value;
                    }
                }else{
                    // construct temporary combination
                    qBinarySafeString tmpsearcher = qbss_constructor();
                    qbss_append(tmpsearcher,tmprefix.str,tmprefix.size);
                    qbss_append(tmpsearcher,(char*)&tmpcnt,sizeof(tmpcnt));
                    qMapData *tmpmd = q__Map_ptr_at(previous_data,tmpsearcher.str,tmpsearcher.size,q__uhashf);
                    if(tmpmd != NULL){
                        tmpsister = tmpmd->value;
                    }
                    qbss_destructor(tmpsearcher);
                }
                    // or perhaps i am an orphan. that's ... not so bad
                if(tmpsister != NULL){
                    tmpldrefr->head = tmpsister->head;
                    tmpldrefr->tail = tmpsister->tail;
                    tmpldrefr->size = tmpsister->size;
                    // a charming ending. dont you think so?
                }
            }
            if(prefix_num != 0){
                // my siblings -- they might have been in this world for a long time.
                // lets find that out their house's address.
                qMapData* tmpaddress = q__Map_ptr_at(&lists,tmprefix.str,tmprefix.size,q__uhashf);
                // is there living anyone?
                if(tmpaddress != NULL){
                    // another good endings again.
                    qListDescriptor *tmpsiblings = tmpaddress->value;
                    // join them
                    q__List_push_back(tmpsiblings,tmpdata,tmpsize);
                }else{
                    // i have to prepare for the coming brothers and sisters!
                    qListDescriptor tmphouse = qList_constructor();
                    q__List_push_back(&tmphouse,tmpdata,tmpsize);
                    // register in local address book.
                    q__Map_insert(&lists,tmprefix.str,&tmphouse,tmprefix.size,sizeof(qListDescriptor),q__uhashf);
                }
            }
            if(prefix_num == 0) q__List_push_back(&itemslist,tmpdata,tmpsize);
        }
        qbss_destructor(tmprefix);
    }
    // works for this level is finished.
    // check have reached the top
    if(prefix_num == 0){q__Map_clear(&lists);return itemslist;}
    // generate temporary stuff
    qListDescriptor tmpng = qList_copy(dataset);
    qList_pop_back(tmpng);
    qListDescriptor tmpretld = q__list_unserialize(tmpng,&lists);
    q__Map_clear(&lists);
    qList_destructor(tmpng);
    return tmpretld;
}

void* qUnserialize(struct q__ListDescriptor dataset,int isList){
    if(isList){
        qListDescriptor *ld = malloc(sizeof(qListDescriptor));
        qListDescriptor buffer = q__list_unserialize(dataset,NULL);
        *ld = buffer;
        return ld;
    }else{
        qBinarySafeString bss=qbss_constructor();
        qbss_append(bss,((qBinarySafeString*)(dataset.head->data))->str,
                    ((qBinarySafeString*)(dataset.head->data))->size);
        return bss.str;
    }
}