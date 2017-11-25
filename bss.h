#ifndef Q_RE_BSS_H
#define Q_RE_BSS_H

struct q__binary_safe_string{
    char* str;
    unsigned int size;
    unsigned int capacity;
};

typedef struct q__binary_safe_string qBinarySafeString;

qBinarySafeString qbss_constructor(); // create a binary_safe_string

#define qbss_from_array(arr) qbss__from_array(arr,sizeof(arr))
qBinarySafeString qbss__from_array(void* arr,unsigned int size);

#define qbss_append(bss,str,len) q__bss_append(&(bss),str,len)
void q__bss_append(qBinarySafeString* bss,char* str,unsigned int len);
// any operation on the destructed bss is strictly prohibited.
// dont risk your time doing something silly
void qbss_destructor(qBinarySafeString bss);

#endif