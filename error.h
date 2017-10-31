#ifndef Q_ZHWKRE_ERRNO_H
#define Q_ZHWKRE_ERRNO_H

extern int zhwk_error;

// macros
#define SETERR(x) (zhwk_error = (x))

// errors
#define ZHWK_ERR_SOCK_DOMAIN_INVAL 30
#define ZHWK_ERR_SOCK_ADDR_INVAL 31
#define ZHWK_ERR_SOCK_FD_OPEN_FAIL 32
#define ZHWK_ERR_SOCK_BIND_FAIL 33
#define ZHWK_ERR_SOCK_CLOSE_FAIL 34
#define ZHWK_ERR_SOCK_SETOPT_FAIL 35
#define ZHWK_ERR_TCP_CONNECT_FAIL 40
#define ZHWK_ERR_TCP_LISTEN_FAIL 41
#define ZHWK_ERR_TCP_ACCEPT_FAIL 42
#define ZHWK_ERR_TCP_READ_FAIL 43
#define ZHWK_ERR_TCP_RECV_FAIL 44
#define ZHWK_ERR_TCP_WRITE_FAIL 45
#define ZHWK_ERR_UDP_RECV_FAIL 50
#define ZHWK_ERR_UDP_SEND_FAIL 51

#endif