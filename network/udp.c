#include "../network.h"
#include "../utils.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

int qDatagramSocket_receive(qSocket sock,char* srcaddr,char* buffer,unsigned int size,int flags){
    if(sock.domain != AF_INET){
        return -1;
    }
    memset(buffer,0,size);
    struct sockaddr_in srcaddr_in;
    memset(&srcaddr_in,0,sizeof(srcaddr_in));
    socklen_t addrsz = sizeof(srcaddr_in);
    int actrlen = recvfrom(sock.desc,buffer,size,flags,(struct sockaddr*)&srcaddr_in,&addrsz);
    if(srcaddr != NULL){
        memset(srcaddr,0,26);
        strcpy(srcaddr,inet_ntoa(srcaddr_in.sin_addr));
        sprintf(srcaddr+strlen(inet_ntoa(srcaddr_in.sin_addr)),":%d",ntohs(srcaddr_in.sin_port));
    }
    return actrlen;
}

int qDatagramSocket_send(qSocket sock,const char* dest,const char* payload,unsigned int size,int flags){
    if(sock.domain != AF_INET){
        return -1;
    }
    int separator_pos = find_byte(dest,':',strlen(dest));
    struct sockaddr_in dest_addr;
    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.sin_addr = str_to_ipv4addr(dest);
    short tmpport;
    sscanf(dest+separator_pos+1,"%hd",&tmpport);
    dest_addr.sin_port = htons(tmpport);
    int sentlen = sendto(sock.desc,payload,size,flags,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr_in));
    return sentlen;
}
