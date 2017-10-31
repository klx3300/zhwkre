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

#endif