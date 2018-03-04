#include "../AVLTree.h"
#include <stdlib.h>
#include <stdio.h>
#include "../error.h"

#define TESTNUM 10000000

int int_comparator(void* a,unsigned int asize,void* b,unsigned int bsize){
    if(asize != 4 || bsize != 4){
        printf("[ERR] Allocation correctness failed: asize %u bsize %u inequal 4.\n",asize,bsize);
        exit(1);
    }
    return (*(int*)a)-(*(int*)b);
}

int main(void){
    qAVLTreeDescriptor desc = qAVLTree_constructor(int_comparator);
    printf("[DBG] Testing Insertion..\n");
    for(int i=0;i<TESTNUM;i++){
        //printf("[LOG] Attempt inserting %d\n",i);
        int stat = qAVLTree_insert(desc,i);
        fatalerr();
        if(stat != 0){
            printf("[ERR] While inserting %d insertion returned %d\n",i,stat);
            exit(1);
        }
    }
    if(desc.size != TESTNUM){
        printf("[ERR] Recorded size is %d, expected %d\n",desc.size,TESTNUM);
        exit(1);
    }
    printf("[DBG] Testing Correctness..\n");
    for(int i=0;i<TESTNUM;i++){
        //printf("[LOG] Attempt validating %d\n",i);
        qAVLTreeIterator testiter;
        int stat = qAVLTree_ptr_at(desc,testiter,i);
        fatalerr();
        if(stat != 0){
            printf("[ERR] While validating %d ptr_at returned stat %d\n",i,stat);
            exit(1);
        }
        if(!qAVLTreeIterator_isvalid(testiter)){
            printf("[ERR] While validating %d ptr_at returned invalid iterator.\n",i);
            exit(1);
        }
        if(*(int*)(qAVLTreeIterator_deref(testiter)) != i){
            printf("[ERR] Data correctness error: expected %d, get %d.\n",i,*(int*)(qAVLTreeIterator_deref(testiter)));
            exit(1);
        }
    }
    printf("[DBG] Testing Iteration/Next ..\n");
    int curr = 0;
    for(qAVLTreeIterator it=qAVLTree_begin(desc);qAVLTreeIterator_isvalid(it);it=qAVLTreeIterator_next(it)){
        if(*(int*)(qAVLTreeIterator_deref(it)) != curr){
            printf("[ERR] While iterating through, No.%d elem isn't %d.\n",*(int*)(qAVLTreeIterator_deref(it)),curr);
            exit(1);
        }
        curr++;
    }
    if(curr != TESTNUM){
        printf("[ERR] Only Iterated %d elems, expected %d\n",curr,TESTNUM);
        exit(1);
    }
    printf("[DBG] Testing Iteration/Prev ..\n");
    curr = TESTNUM-1;
    for(qAVLTreeIterator it=qAVLTree_end(desc);qAVLTreeIterator_isvalid(it);it=qAVLTreeIterator_prev(it)){
        if(*(int*)(qAVLTreeIterator_deref(it)) != curr){
            printf("[ERR] While iterating through, No.%d elem isn't %d.\n",*(int*)(qAVLTreeIterator_deref(it)),curr);
            exit(1);
        }
        curr--;
    }
    if(curr != -1){
        printf("[ERR] Only Iterated %d elems, expected %d\n",TESTNUM-1-curr,TESTNUM);
        exit(1);
    }
    printf("[DBG] Testing Erase..\n");
    for(int i=0;i<TESTNUM;i++){
        if(i%2){
            qAVLTreeIterator testiter;
            int stat = qAVLTree_ptr_at(desc,testiter,i);
            if(stat != 0){
                printf("[ERR] While erasing %d ptr_at returned stat %d\n",i,stat);
                exit(1);
            }
            //printf("[LOG] Attempting erase %d\n",i);
            stat = qAVLTree_erase(desc,testiter);
            if(stat != 0){
                printf("[ERR] While erasing %d erase returned stat %d\n",i,stat);
                exit(1);
            }
            /*int thewrongone = 13;
            int wrongstat = qBTree_ptr_at(desc,testiter,thewrongone);
            if(wrongstat == 0){
                printf("[DBG] Value %d still exists.\n",thewrongone);
            }else{
                printf("[DBG] Value %d vanished.\n",thewrongone);
                exit(-1);
            }*/
        }
    }
    if(desc.size != TESTNUM/2){
        printf("[ERR] Recorded size is %d, expected %d.\n",desc.size,TESTNUM/2);
        exit(1);
    }
    printf("[DBG] Testing Correctness..\n");
    for(int i=0;i<TESTNUM;i++){
        qAVLTreeIterator testiter;
        int stat = qAVLTree_ptr_at(desc,testiter,i);
        if(i%2){
            if(stat != -1){
                printf("[ERR] While validating %d ptr_at returned %d but it shouldn't.\n",i,stat);
                exit(1);
            }
        }else{
            if(stat != 0){
                printf("[ERR] While validating %d ptr_at returned %d but it shouldn't.\n",i,stat);
                exit(1);
            }
            if(!qAVLTreeIterator_isvalid(testiter)){
                printf("[ERR] While validating %d ptr_at returned invalid iterator.\n",i);
                exit(1);
            }
            if(*(int*)(qAVLTreeIterator_deref(testiter)) != i){
                printf("[ERR] Data correctness error: expected %d, get %d.\n",i,*(int*)(qAVLTreeIterator_deref(testiter)));
                exit(1);
            }
        }
    }
    printf("[YES] Test passed with %d elements.\n",TESTNUM);
    return 0;
}