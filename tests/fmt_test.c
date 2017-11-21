#include "../format.h"
#include <stdio.h>

int main(void){
    printf(Q_FMT_APPLY(Q_COLOR_RED) "red test" Q_FMT_APPLY(Q_COLOR_RESET) "\n");
    return 0;
}