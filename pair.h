#ifndef Q_ZHWKRE_PAIR_H
#define Q_ZHWKRE_PAIR_H

struct q__pair_st{
    void* key;
    unsigned int keysize;
    void* value;
    unsigned int valuesize;
};

typedef struct q__pair_st qPair;

#endif