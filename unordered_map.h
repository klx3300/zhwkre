#include "list.h"
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

#define Q_DEFAULT_MAXHASHV 131

typedef struct q__MapData{
    void* key;
    void* value;
    int keylen;
    int valuelen;
}qMapData;

struct q__Map{
    qListDescriptor* listArray;
    unsigned int maxlength;
    unsigned int counts;
};

typedef unsigned int (*qHashFuncProto)(void* key,unsigned int keysize);

typedef struct q__Map qMap;
// functions 
// note the absence of reference
qMap qMap_constructor(unsigned int maxhashv);
qMapData q__MapData_constructor(void* key,void* value,unsigned int keysize,unsigned int valuesize);
unsigned int qMap_size(qMap st);

#define qMap_clear(map) q__Map_clear(&(map))
int q__Map_clear(qMap* st);
#define qMap_insert(map,key,value,hashf) q__Map_insert(&(map),&(key),&(value),sizeof(key),sizeof(value),hashf)
int q__Map_insert(qMap* st, void* key, void* value,unsigned int keysize,unsigned int valuesize,qHashFuncProto hashf);
#define qMap_erase(map,key,hashf) q__Map_erase(&(map),&(key),sizeof(key),hashf)
int q__Map_erase(qMap* st, void* key, unsigned int keysize,qHashFuncProto hashf);

// return NULL if not found
// otherwise return pointer to value
#define qMap_ptr_at(map,key,hashf) q__Map_ptr_at(&(map),&(key),sizeof(key),hashf)
qMapData* q__Map_ptr_at(qMap* st, void* key, unsigned int keysize,qHashFuncProto hashf);