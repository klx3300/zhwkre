#include "../vector.h"
#include "../error.h"
#include "../log.h"
#include <stdlib.h>

#define TESTNUM 10000000

int main(void){
    qVectorDescriptor desc = qVector_constructor(sizeof(int));
    qLogDebug("Testing Insertion..");
    for(int i=0;i<TESTNUM;i++){
        int tmp = i+1;
        qVector_push_back(desc,tmp);
        fatalerr();
    }
    qLogSucc("Correct.");
    qLogDebug("Testing Insertion Integrity..");
    for(int i=0;i<TESTNUM;i++){
        if(*(int*)qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),i)) != i+1){
            qLogFailfmt("Integrity Failed: expected %d but %d.",i+1,*(int*)qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),i)));
            exit(-1);
        }
    }
    qLogSucc("Correct.");
    qLogDebug("Testing Pop..");
    for(int i=0;i<TESTNUM/2;i++){
        qVector_pop_back(desc);
        fatalerr();
    }
    qLogSucc("Correct.");
    qLogDebug("Testing Pop Integrity..");
    for(int i=0;i<TESTNUM-(TESTNUM/2);i++){
        if(*(int*)qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),i)) != i+1){
            qLogFailfmt("Integrity Failed: expected %d but %d.",i+1,*(int*)qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),i)));
            exit(-1);
        }
    }
    qLogSucc("Correct.");
    qVector_destructor(desc);
    desc = qVector_constructor(sizeof(int));
    qLogInfo("Resetting content..");
    for(int i=0;i<TESTNUM;i++){
        int tmp = i+1;
        qVector_push_back(desc,tmp);
        fatalerr();
    }
    qLogDebug("Testing erase..");
    qVector_erase(desc,qVectorIterator_move(qVector_begin(desc),1993));
    fatalerr();
    qLogSucc("Correct.");
    qLogDebug("Testing erase integrity..");
    if(*(desc.size) != TESTNUM-1){
        qLogFailfmt("Size Failed: expected %d but %d\n",TESTNUM-1,*(desc.size));
        exit(-1);
    }
    for(int i=0;i<TESTNUM-1;i++){
        if(*(int*)qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),i)) != i+1){
            if(i<1993){
                qLogFailfmt("Integrity Failed: expected %d but %d.",i+1,*(int*)qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),i)));
                exit(-1);
            }
        }else{
            if(i>=1993){
                qLogFailfmt("Integrity Failed: expected %d but %d.",i,*(int*)qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),i)));
                exit(-1);
            }
        }
    }
    qLogSucc("Correct.");
}