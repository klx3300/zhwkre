#include "../utils.h"
#include "../unidef.h"
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

int string_comparator(void* stra,unsigned int sizea,void* strb,unsigned int sizeb){
    char *achar=stra,*bchar=strb;
    int alen=strlen(achar),blen=strlen(bchar);
    int cmplen=MIN(alen,blen);
    for(int i=0;i<cmplen;i++){
        if(achar[i]!=bchar[i]){
            return achar[i]-bchar[i];
        }
    }
    if(alen!=blen){
        return alen-blen;
    }
    return 0;
}

qBinarySafeString readline(FILE* fp){
    qBinarySafeString result = qbss_constructor();
    for(int ch=fgetc(fp);ch!=EOF && ch!='\n';ch=fgetc(fp)){
        char realch = (char)ch;
        q__bss_append(&result,&realch,1);
    }
    return result;
}

qBinarySafeString readall(FILE* fp){
    qBinarySafeString result = qbss_constructor();
    for(int ch=fgetc(fp);ch!=EOF;ch=fgetc(fp)){
        char realch = (char)ch;
        q__bss_append(&result,&realch,1);
    }
    return result;
}

