#include "../utils.h"
#include <string.h>
#include <stdio.h>

int fullstrcmp(const char* a,const char* b){
    int lena=strlen(a),lenb=strlen(b);
    if (lena!=lenb){
        return 0;
    }
    for(int i=0;i<lena;i++){
        if(a[i]!=b[i]) return 0;
    }
    return 1;
}

void printSomeSpaces(unsigned int spcs){
    for(int i=0;i<spcs;i++){
        printf(" ");
    }
}

int find_byte(const char* str,const char target,int size){
    for(int i=0;i<size;i++){
        if(str[i]==target){
            return i;
        }
    }
    return -1;
}

int partstrcmp(const char* a,const char* b){
    int lena=strlen(a),lenb=strlen(b);
    int rlen=(lena>lenb?lenb:lena);
    for(int i=0;i<rlen;i++){
        if(a[i]!=b[i]){
            return 0;
        }
    }
    return 1;
}