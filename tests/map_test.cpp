#include <unordered_map>
#include "../unordered_map.h"
#include <iostream>

unsigned int hashf(void* key,unsigned int size){
    return *((unsigned int*)key)%131;
}

int main(void){
    qMap qmap=qMap_constructor(Q_DEFAULT_MAXHASHV);
    for(int i=0;i<100;i++){
        q__Map_insert(&qmap,&i,&i,sizeof(int),sizeof(int),hashf);
    }
    std::cout << "The size is " << qMap_size(qmap) << std::endl;
    for(int i=0;i<100;i++){
        if(q__Map_ptr_at(&qmap,&i,sizeof(int),hashf)==NULL){
            std::cout << "Existence check on "<<i<<"th item failed." << std::endl;
            continue;
        }
        if(*(int*)(q__Map_ptr_at(&qmap,&i,sizeof(int),hashf)->value)!=i){
            std::cout << "Correctiveness check on "<<i<<"th item failed. Wrong result is "<< *((int*)q__Map_ptr_at(&qmap,&i,sizeof(int),hashf)->value) << std::endl;
        }
    }
    // elmination check
    for(int i=0;i<100;i++){
        if(i%2==0){
            q__Map_erase(&qmap,&i,sizeof(int),hashf);
        }
    }
    for(int i=0;i<100;i++){
        if(i%2==0){
            q__Map_erase(&qmap,&i,sizeof(int),hashf);
        }
    }
    std::cout << "The size is " << qMap_size(qmap) << std::endl;
    for(int i=0;i<100;i++){
        if(i%2==0){
            if(q__Map_ptr_at(&qmap,&i,sizeof(int),hashf)!=NULL){
                std::cout << "Existence check on "<<i<<"th item failed. Wrong result is " << *((int*)q__Map_ptr_at(&qmap,&i,sizeof(int),hashf)->value) << std::endl;
            }
        }else{
            if(*(int*)(q__Map_ptr_at(&qmap,&i,sizeof(int),hashf)->value)!=i){
                std::cout << "Correctiveness check on "<<i<<"th item failed. Wrong result is "<< *((int*)q__Map_ptr_at(&qmap,&i,sizeof(int),hashf)->value) << std::endl;
            }
        }
    }
    return 0;
}