#include "../network.h"

int qSocket_select(int maxfdid, qSocketSet* rd, qSocketSet* wr, qSocketSet* ex){
    return select(maxfdid, rd, wr, ex, NULL);
}

int qSocketSet_exhaust(qSocketSet* sset, qSocket* chkarr, size_t chklen, qSocket* okarr){
    size_t accu = 0;
    for(size_t i = 0; i < chklen; i++){
        qSocket sock = chkarr[i];
        if(qSocketSet_exist(sset, sock)){
            okarr[accu] = sock;
            accu ++;
        }
    }
    return accu;
}
