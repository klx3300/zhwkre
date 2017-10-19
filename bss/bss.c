#include "../bss.h"
#include <stdlib.h>
#include <string.h>

binary_safe_string qbss_new(){
    binary_safe_string bss;
    bss.str = malloc(64);
    bss.size = 0;
    bss.capacity = 64;
    memset(bss.str,0,64);
    return bss;
}

void q__bss_append(binary_safe_string* bss,char* str,unsigned int len){
    // check capacity
    while(bss->capacity<2*(bss->size+len)){
        // realloc
        char* tmp = malloc(2*(bss->capacity));
        memset(tmp,0,2*(bss->capacity));
        memcpy(tmp,bss->str,bss->size);
        bss->capacity = bss->capacity * 2;
        free(bss->str);
        bss->str = tmp;
    }
    // copy to new dest
    memcpy(bss->str+(bss->size),str,len);
    bss->size += len;
}

void qbss_destructor(binary_safe_string bss){
    free(bss.str);
}