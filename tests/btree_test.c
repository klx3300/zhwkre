#include "../btree.h"
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
    qBTreeDescriptor desc = qBTree_constructor(int_comparator);
    printf("[DBG] Testing Insertion..\n");
    for(int i=0;i<TESTNUM;i++){
        //printf("[LOG] Attempt inserting %d\n",i);
        int tmpvaild = i+1;
        int stat = qBTree_insert(desc,i,tmpvaild);
        fatalerr();
        if(stat != 0){
            printf("[ERR] While inserting %d:%d insertion returned %d\n",i,tmpvaild,stat);
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
        qBTreeIterator testiter;
        int stat = qBTree_ptr_at(desc,testiter,i);
        fatalerr();
        if(stat != 0){
            printf("[ERR] While validating %d ptr_at returned stat %d\n",i,stat);
            exit(1);
        }
        if(!qBTreeIterator_isvalid(testiter)){
            printf("[ERR] While validating %d ptr_at returned invalid iterator.\n",i);
            exit(1);
        }
        if(*(int*)(qBTreeIterator_deref(testiter).value) != i+1){
            printf("[ERR] Data correctness error: expected %d at %d, get %d.\n",i+1,i,*(int*)(qBTreeIterator_deref(testiter).value));
            exit(1);
        }
    }
    printf("[DBG] Testing Iteration/Next ..\n");
    int curr = 0;
    for(qBTreeIterator it=qBTree_begin(desc);qBTreeIterator_isvalid(it);it=qBTreeIterator_next(it)){
        if(*(int*)(qBTreeIterator_deref(it).key) != curr){
            printf("[ERR] While iterating through, No.%d key isn't %d.\n",*(int*)(qBTreeIterator_deref(it).key),curr);
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
    for(qBTreeIterator it=qBTree_end(desc);qBTreeIterator_isvalid(it);it=qBTreeIterator_prev(it)){
        if(*(int*)(qBTreeIterator_deref(it).key) != curr){
            printf("[ERR] While iterating through, No.%d key isn't %d.\n",*(int*)(qBTreeIterator_deref(it).key),curr);
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
            //printf("[LOG] Attempt erasing %d\n",i);
            qBTreeIterator testiter;
            int stat = qBTree_ptr_at(desc,testiter,i);
            if(stat != 0){
                printf("[ERR] While erasing %d ptr_at returned stat %d\n",i,stat);
                exit(1);
            }
            stat = qBTree_erase(desc,testiter);
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
        qBTreeIterator testiter;
        int stat = qBTree_ptr_at(desc,testiter,i);
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
            if(!qBTreeIterator_isvalid(testiter)){
                printf("[ERR] While validating %d ptr_at returned invalid iterator.\n",i);
                exit(1);
            }
            if(*(int*)(qBTreeIterator_deref(testiter).value) != i+1){
                printf("[ERR] Data correctness error: expected %d at %d, get %d.\n",i+1,i,*(int*)(qBTreeIterator_deref(testiter).value));
                exit(1);
            }
        }
    }
    printf("[YES] Test passed with %d elements.\n",TESTNUM);
    return 0;
}