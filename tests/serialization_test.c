#include "../list.h"
#include "../unordered_map.h"
#include "../serialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils.h"

typedef unsigned int ui;

ui list_compare(qListDescriptor a,qListDescriptor b){
    // first compare the length
    if(a.size != b.size){printf("List size incorrect.\n");return 0;};
    ui counter=0;
    for(qListIterator ai=a.head,bi=b.head;ai!=NULL && bi!=NULL;(ai=ai->next),(bi=bi->next),(counter++)){
        if(ai->size!=bi->size){printf("Elem %u size incorrect. %u != %u.\n",counter,ai->size,bi->size);return 0;};
        if(memcmp(ai->data,bi->data,ai->size)){printf("Elem %u content incorrect.\n",counter);return 0;};
    }
    return 1;
}

typedef struct _tsta_t{qListDescriptor ld;char data[4];} tsta;

int main(void){
    // normal linked list serialization
    printf(" -- test normal serialization START.\n");
    qListDescriptor normal_ld;
    qList_initdesc(normal_ld);
    for(ui i=0;i<10000;i++){
        qList_push_back(normal_ld,i);
    }
    // serialize
    printf("   -- Serialization on normal_ld start.\n");
    qListDescriptor normal_ser_data = qSerialize(&normal_ld,sizeof(qListDescriptor));
    // unserialize
    printf("   -- Unserialization on normal_ld start.\n");
    qListDescriptor* unser_normal_ld = qUnserialize(normal_ser_data,YES_IT_IS_A_LIST);
    if(!list_compare(normal_ld,*unser_normal_ld)){
        printf(" -- test normal serialization FAILED.\n");
    }else{
        printf(" -- test normal serialization PASSED.\n");
    }

    printf(" -- test recursive I serialization START.\n");
    qListDescriptor ri_ld;
    qList_initdesc(ri_ld);
    for(ui i=0;i<100;i++){
        tsta* tmp=malloc(sizeof(tsta));
        qList_initdesc(tmp->ld);
        for(ui i=0;i<100;i++){
            qList_push_back(tmp->ld,i);
        }
        *(ui*)tmp->data = i;
        q__List_push_back(&ri_ld,tmp,sizeof(tsta));
        free(tmp);
    }
    printf("   -- Serialization on ri_ld start.\n");
    qListDescriptor ri_ser_data = qSerialize(&ri_ld,sizeof(qListDescriptor));
    printf("   -- Unserialization on ri_ld start.\n");
    qListDescriptor* unser_ri_ld = qUnserialize(ri_ser_data,YES_IT_IS_A_LIST);
    ui ric=0;
    ui FLAG_RI_PASSED=1;
    if(unser_ri_ld->size != 100){
        printf("Unserialization size incorrect. result is %u.\n",unser_ri_ld->size);
        FLAG_RI_PASSED = 0;
    }
    qList_foreach(*unser_ri_ld,rii){
        tsta* tmprefr = rii->data;
        if(tmprefr->ld.size != 100){
            printf("List %u size incorrect. result is %u.\n",ric,tmprefr->ld.size);
            FLAG_RI_PASSED = 0;
            continue;
        }
        ui riic = 0;
        qList_foreach(tmprefr->ld,riii){
            if(*(ui*)riii->data != riic){
                printf("Listelem %u-%u incorrect. result is %u.\n",ric,riic,*(ui*)riii->data);
                FLAG_RI_PASSED = 0;
                continue;
            }
            riic++;
        }
        ric++;
    }
    if(FLAG_RI_PASSED){
        printf(" -- test recursive I serialization PASSED.\n");
    }else{
        printf(" -- test recursive I serialization FAILED.\n");
    }
    printf(" -- test recursive II serialization START.\n");
    qListDescriptor rj_ld;
    qList_initdesc(rj_ld);
    for(ui i=0;i<100;i++){
        tsta *tmpi = malloc(sizeof(tsta));
        *(ui*)(tmpi->data) = i;
        qList_initdesc(tmpi->ld);
        for(ui j=0;j<10;j++){
            tsta *tmpj = malloc(sizeof(tsta));
            *(ui*)(tmpj->data)=j;
            qList_initdesc(tmpj->ld);
            for(ui k=0;k<10;k++){
                qList_push_back(tmpj->ld,k);
            }
            qList_push_back(tmpi->ld,*tmpj);
            free(tmpj);
        }
        qList_push_back(rj_ld,*tmpi);
        free(tmpi);
    }
    printf("   -- Serialization on rj_ld start.\n");
    qListDescriptor rj_ser_data = qSerialize(&rj_ld,sizeof(qListDescriptor));
    if(rj_ser_data.size != 3){
        printf("Serialization size error. result is %u.\n",rj_ser_data.size);
    }
    printf("   -- Unserialization on rj_ld start.\n");
    qListDescriptor* unser_rj_ld = qUnserialize(rj_ser_data,YES_IT_IS_A_LIST);
    ui rjc = 0;
    ui FLAG_RJ_PASSED = 1;
    if(unser_rj_ld->size != 100){
        printf("Unserialization size error. result is %u\n",unser_rj_ld->size);
    }
    qList_foreach(*unser_rj_ld,rji){
        tsta* tmprefr = rji->data;
        if(tmprefr->ld.size != 10){
            printf("List %u size incorrect. result is %u.\n",rjc,tmprefr->ld.size);
            FLAG_RJ_PASSED = 0;
        }
        if(*(ui*)tmprefr->data != rjc){
            printf("List %u data incorrect. result is %u.\n",rjc,*(ui*)tmprefr->data);
            FLAG_RJ_PASSED = 0;
        }
        ui rjjc = 0;
        qList_foreach(tmprefr->ld,rjji){
            tsta* tmprefrr = rjji->data;
            if(tmprefrr->ld.size != 10){
                printf("List %u-%u size incorrect. result is %u.\n",rjc,rjjc,tmprefr->ld.size);
                FLAG_RJ_PASSED = 0;
            }
            if(*(ui*)tmprefrr->data != rjjc){
                printf("List %u-%u data incorrect. result is %u.\n",rjc,rjjc,*(ui*)tmprefrr->data);
                FLAG_RJ_PASSED = 0;
            }
            ui rjjjc = 0;
            qList_foreach(tmprefrr->ld,rjjji){
                if(*(ui*)rjjji->data != rjjjc){
                    printf("Listelem %u-%u-%u incorrect. result is %u.\n",rjc,rjjc,rjjjc,*(ui*)rjjji->data);
                    FLAG_RJ_PASSED = 0;
                }
                rjjjc++;
            }
            rjjc++;
        }
        rjc++;
    }
    if(FLAG_RJ_PASSED){
        printf(" -- test recursive II serialization PASSED.\n");
    }else{
        printf(" -- test recursive II serialization FAILED.\n");
    }
    return 0;
}