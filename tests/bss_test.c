#include "../bss.h"
#include "../error.h"
#include "../log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ELEMCOUNT 10000000

int main(void){
    qBinarySafeString bss = qbss_constructor();
    qLogInfo("Testing Insertion..");
    for(int i=0;i<ELEMCOUNT;i++){
        q__bss_append(&bss,(char*)&i,sizeof(i));
    }
    qLogInfo("Testing Correctness..");
    int *fakearr = (int*)bss.str;
    for(int i=0;i<ELEMCOUNT;i++){
        if(fakearr[i]!=i){
            qLogFailfmt("Incorrect occurred at: %d(%d expected).",fakearr[i],i);
            exit(-1);
        }
    }
    qLogSuccfmt("Test Completed with %d Elements.",ELEMCOUNT);
    return 0;
}