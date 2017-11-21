#include "../format.h"
#define Q_LOG_LOGLEVEL 0
#include "../log.h"
#include <stdio.h>

int main(void){
    qLogDebug("A sample debug message!");
    qLogInfo("A sample info message!");
    qLogSucc("A sample succ message!");
    qLogWarn("A sample warn message!");
    qLogFail("A sample fail message!");
    return 0;
}