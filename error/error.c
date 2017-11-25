#include "../error.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../log.h"

int zhwk_error;

int chkerr(void){
    if(zhwk_error){
        qLogWarnfmt("zhwkerr %d, posix err %s",zhwk_error,strerror(errno));
        return -1;
    }
    zhwk_error = 0;
    return 0;
}

void fatalerr(void){
    if(zhwk_error){
        qLogFailfmt("zhwkerr %d, posix err %s\n",zhwk_error,strerror(errno));
        exit(-1);
    }
    zhwk_error = 0;
}

void clrerr(void){
    zhwk_error = 0;
}