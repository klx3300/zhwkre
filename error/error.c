#include "../error.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int zhwk_error;

int chkerr(void){
    if(zhwk_error){
        fprintf(stderr,"[D] zhwkerr %d, posix err %d\n",zhwk_error,errno);
        return -1;
    }
    zhwk_error = 0;
    return 0;
}

void fatalerr(void){
    if(zhwk_error){
        fprintf(stderr,"[E] zhwkerr %d, posix err %d\n",zhwk_error,errno);
        exit(-1);
    }
    zhwk_error = 0;
}

void clrerr(void){
    zhwk_error = 0;
}