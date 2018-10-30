#include "../unordered_map.h"
#include <string.h>
#include <stdio.h>

qMap qMap_constructor(unsigned int maxhashv){
    qMap thas;
    thas.maxlength=maxhashv;
    thas.counts=0;
    thas.listArray=(qListDescriptor*)malloc(sizeof(qListDescriptor)*maxhashv);
    if(thas.listArray == NULL){
        return thas;
    }
    for(unsigned int iter=0;iter<maxhashv;iter++){
        thas.listArray[iter] = qList_constructor();
    }
    return thas;
}

qMapData q__MapData_constructor(void* key,void* value,unsigned int keysize,
    unsigned int valuesize){
    qMapData ptr;
    ptr.keylen=keysize;
    ptr.valuelen=valuesize;
    ptr.key=malloc(keysize);
    if(ptr.key == NULL){
        return ptr;
    }
    memcpy(ptr.key,key,keysize);
    ptr.value=malloc(valuesize);
    if(ptr.value == NULL){
        return ptr;
    }
    memcpy(ptr.value,value,valuesize);
    return ptr;
}

unsigned int qMap_size(qMap thas){
    return thas.counts;
}

int q__Map_clear(qMap* thas){
    for(unsigned int iter=0;iter<thas->maxlength;iter++){
        if(!(((thas->listArray)[iter]).size))
            continue;
        // using foreach to clear the whole allocated data
        qList_foreach((thas->listArray)[iter],liter){
            qMapData *tmprefr = (qMapData*)liter->data;
            free(tmprefr->key);
            free(tmprefr->value);
        }
        q__List_destructor((thas->listArray)+iter);
    }
    thas->counts = 0;
    return 0;
}

int q__Map_insert(qMap* thas,void* key,void* value,unsigned int keysize,
                   unsigned int valuesize,qHashFuncProto hashf){
    unsigned int hashv=hashf(key,keysize);
    qListDescriptor referred_list=(thas->listArray)[hashv];
    qList_foreach(referred_list,iter){
            if(!memcmp(((qMapData*)(iter->data))->key,key,keysize)){
                memcpy(((qMapData*)(iter->data))->value,value,valuesize);
                return 0;
            }
    }
    qMapData tmpmpd = q__MapData_constructor(key,value,keysize,valuesize);
    if(tmpmpd.key == NULL || tmpmpd.value == NULL) return -1;
    q__List_push_back(thas->listArray+hashv,&tmpmpd,sizeof(tmpmpd));
    thas->counts++;
    return 0;
}

int q__Map_erase(qMap* thas,void* key,unsigned int keysize,qHashFuncProto hashf){
    unsigned int hashv=hashf(key,keysize);
    qListDescriptor referred_list=(thas->listArray)[hashv];
    qList_foreach(referred_list,iter){
        if(!memcmp(((qMapData*)(iter->data))->key,key,keysize)){
            // correct.
            qMapData *tmprefr=(qMapData*)iter->data;
            free(tmprefr->key);
            free(tmprefr->value);
            q__List_erase_elem(thas->listArray+hashv,iter);
            thas->counts--;
            return 0;
        }
    }
    return -1;
}

qMapData* q__Map_ptr_at(qMap* thas,void* key,unsigned int keysize,qHashFuncProto hashf){
    unsigned int hashv=hashf(key,keysize);
    qListDescriptor referred_list=(thas->listArray)[hashv];
    qList_foreach(referred_list,iter){
        if(!memcmp(((qMapData*)(iter->data))->key,key,keysize)){
            return (qMapData*)iter->data;
        }
    }
    return NULL;
}
