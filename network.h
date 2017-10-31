#ifndef Q_RE_NETWORK_H
#define Q_RE_NETWORK_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <time.h>

#define qStreamSocket SOCK_STREAM
#define  qDatagramSocket SOCK_DGRAM
#define qDefaultProto 0
#define qDatagramSocketDefaultFlag 0
#define qUnixDomain AF_UNIX
#define qIPv4 AF_INET
#define qIPv6 AF_INET6

struct q__Socket{
    int desc;
    int domain;
    int type;
    int protocol;
    int quickack;
};

typedef struct q__Socket qSocket;

// some useful util funcs
struct in_addr str_to_ipv4addr(const char* addrxport);

// generic for all socks
qSocket qSocket_constructor(int domain,int type,int protocol);
#define qSocket_open(sock) qSocket__open(&sock)
int qSocket__open(qSocket* sock);
int qSocket_bind(qSocket sock,const char* addr);
#define qSocket_close(sock) qSocket__close(&sock)
int qSocket__close(qSocket* sock);
#define qSocket_destructor(sock) qSocket__destructor(&sock)
int qSocket__destructor(qSocket* scok);

// stream sockets (tcp)
int qStreamSocket_connect(qSocket sock,const char* addrxport);
int qStreamSocket_listen(qSocket sock);
qSocket qStreamSocket_accept(qSocket sock,char* srcaddr);
//void qStreamSocket__sendbeat(qSocket sock);
//void qStreamSocket__acceptbeat(qSocket sock);
#define qStreamSocket_setQuickAck(sock,qack) qStreamSocket__setQuickAck(&sock,qack)
int qStreamSocket__setQuickAck(qSocket *sock,int quickack);
int qStreamSocket_write(qSocket sock,const char* content,unsigned int length);
int qStreamSocket_read(qSocket sock,char* buffer,unsigned int limitation);
int qStreamSocket_readchar(qSocket sock,char* c);
int qStreamSocket_nonblockRead(qSocket sock,char* buffer,unsigned int limitation);
int qStreamSocket_nonblockReadChar(qSocket sock,char *c);

// datagram sockets (udp)
int qDatagramSocket_receive(qSocket sock,char* srcaddr,char* buffer,unsigned int limitation,int flags);
int qDatagramSocket_send(qSocket sock,const char* dest,const char* content,unsigned int limitation,int flags);

#endif