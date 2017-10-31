#ifndef Q_RE_BSS_H
#define Q_RE_BSS_H

struct q__binary_safe_string{
    char* str;
    unsigned int size;
    unsigned int capacity;
};

typedef struct q__binary_safe_string binary_safe_string;

binary_safe_string qbss_constructor(); // create a binary_safe_string

#define qbss_append(bss,str,len) q__bss_append(&bss,str,len)
void q__bss_append(binary_safe_string* bss,char* str,unsigned int len);
// any operation on the destructed bss is strictly prohibited.
// dont risk your time doing something silly
void qbss_destructor(binary_safe_string bss);

#endif