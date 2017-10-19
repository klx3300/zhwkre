#include "../alg.h"
#include "../list.h"
#include <stdio.h>

void bubble_sort(qListDescriptor *desc,Comparator cmpr){
    int FLAG_EXED=0;
    while(!FLAG_EXED){
        FLAG_EXED = 1;
        for(qListIterator it=desc->head;it->next!=NULL;it=it->next){
            if(cmpr(it,it->next)){
                q__List_swap_elem(desc,it,it->next);
                FLAG_EXED = 0;
                break;
            }
        }
    }
}