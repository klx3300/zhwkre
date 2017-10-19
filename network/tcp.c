#include "../network.h"
#include "../utils.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

int qStreamSocket_connect(qSocket sock,const char* addrxport){
    if(sock.domain != qIPv4){
        return 0;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = qIPv4;
    int separator_pos = find_byte(addrxport,':',strlen(addrxport));
    if(separator_pos == -1){
        return 0;
    }
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    char portstr[10];
    memset(portstr,0,sizeof(portstr));
    strncpy(portstr,addrxport+separator_pos+1,strlen(addrxport)-separator_pos-1);
    short hostport=0;
    sscanf(portstr,"%hd",&hostport);
    addr.sin_port = htons(hostport);
    if(separator_pos != 0){
        addr.sin_addr = str_to_ipv4addr(addrxport);
    }
    int connstat = connect(sock.desc,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
    return (connstat != -1);
}

int qStreamSocket_listen(qSocket sock){
    return (listen(sock.desc,128)!=-1); // 128:limitation of undecided connections.
}


// source ipaddr will be conv into addrxport format
qSocket qStreamSocket_accept(qSocket sock,char* srcaddr){
    qSocket srcinfo;
    srcinfo.domain = sock.domain;
    srcinfo.protocol = sock.protocol;
    srcinfo.type = sock.type;
    srcinfo.quickack = sock.quickack;
    struct sockaddr_in srcaddr_in;
    socklen_t srcaddr_len = sizeof(srcaddr_in);
    srcinfo.desc = accept(sock.desc,(struct sockaddr*)&srcaddr_in,&srcaddr_len);
    memset(srcaddr,0,26);
    strcpy(srcaddr,inet_ntoa(srcaddr_in.sin_addr));
    sprintf(srcaddr+strlen(inet_ntoa(srcaddr_in.sin_addr)),":%d",ntohs(srcaddr_in.sin_port));
    return srcinfo;
}

void qStreamSocket__setQuickAck(qSocket *sock,int qack){
    sock->quickack = 1;
    setsockopt(sock->desc,IPPROTO_TCP,TCP_QUICKACK,&(sock->quickack),sizeof(int));
}

int qStreamSocket_write(qSocket sock,const char* content,unsigned int size){
    int written=write(sock.desc,content,size);
    return written;
}
int qStreamSocket_read(qSocket sock,char* buffer,unsigned int buffer_size){
    memset(buffer,0,buffer_size);
    int actrlen=read(sock.desc,buffer,buffer_size);
    return actrlen;
}
int qStreamSocket_readchar(qSocket sock,char* c){
    int err = read(sock.desc,c,1);
    return err;
}

int qStreamSocket_nonblock_read(qSocket sock,char *buffer,unsigned int size){
    memset(buffer,0,size);
    int actrlen = recv(sock.desc,buffer,size,MSG_DONTWAIT);
    return actrlen;
}

int qStreamSocket_nonblock_readChar(qSocket sock,char* c){
    int err = recv(sock.desc,c,1,MSG_DONTWAIT);
    return err;
}

