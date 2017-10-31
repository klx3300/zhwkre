#include "../network.h"
#include "../utils.h"
#include "../error.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// convert "0.0.0.0:0" like c-str to in_addr_t
struct in_addr str_to_ipv4addr(const char* addrxport){
    int divid = find_byte(addrxport,':',strlen(addrxport));
    char tmpv[50];
    if(divid != -1){
        strncpy(tmpv,addrxport,divid);
    }else{
        strcpy(tmpv,addrxport);
    }
    struct sockaddr_in tmpsck;
    memset(&tmpsck,0,sizeof(tmpsck));
    inet_pton(AF_INET,tmpv,&tmpsck);
    return tmpsck.sin_addr;
}

// not opened yet.
qSocket qSocket_constructor(int domain,int type,int protocol){
    qSocket tmpsock;
    tmpsock.domain=domain;
    tmpsock.type=type;
    tmpsock.protocol=protocol;
    tmpsock.desc = -1;
    tmpsock.quickack = 0;
    return tmpsock;
}

int qSocket__open(qSocket* sock){
    sock->desc = socket(sock->domain,sock->type,sock->protocol);
    if(sock->desc == -1){
        SETERR(ZHWK_ERR_SOCK_FD_OPEN_FAIL);
        return -1;
    }
    return 0;
}


// bind to address like ":80" means wildcard address with 80 port.
int qSocket_bind(qSocket sock,const char* addrxport){
    if(sock.domain != qIPv4){
        SETERR(ZHWK_ERR_SOCK_DOMAIN_INVAL);
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = qIPv4;
    int separator_pos = find_byte(addrxport,':',strlen(addrxport));
    if(separator_pos == -1){
        SETERR(ZHWK_ERR_SOCK_ADDR_INVAL);
        return -1;
    }
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    char portstr[10];
    memset(portstr,0,sizeof(portstr));
    strncpy(portstr,addrxport+separator_pos+1,strlen(addrxport)-separator_pos-1);
    int hostport=0;
    sscanf(portstr,"%d",&hostport);
    addr.sin_port = htons(hostport);
    if(separator_pos != 0){
        addr.sin_addr = str_to_ipv4addr(addrxport);
    }
    int bindstat = bind(sock.desc,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
    if(bindstat == -1){
        SETERR(ZHWK_ERR_SOCK_BIND_FAIL);
        return -1;
    }
    return 0;
}

int qSocket__close(qSocket* sock){
    int retv = (close(sock->desc)!=-1);
    sock->desc = -1;
    if(retv == -1){
        SETERR(ZHWK_ERR_SOCK_CLOSE_FAIL);
        return -1;
    }
    return 0;
}

int qSocket__destructor(qSocket* sock){
    return qSocket__close(sock);
}