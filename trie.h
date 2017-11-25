#ifndef Q_ZHWKRE_TRIE_H
#define Q_ZHWKRE_TRIE_H

#include "bss.h"

struct q__trie_node_st{
    qBinarySafeString value;
    struct q__trie_node_st *parent;
    struct q__trie_node_st *children[256];
};

struct q__trie_desc_st{
    int size;
    struct q__trie_node_st* root;
};

typedef struct q__trie_node_st qTrieNode;
typedef struct q__trie_desc_st qTrieDescriptor;
typedef struct q__trie_node_st *qTrieIterator;

qTrieDescriptor qTrie_constructor();

#define qTrie_ptr_at(desc,dstit,bss) qTrie__ptr_at(desc,&(dstit),bss)
int qTrie__ptr_at(qTrieDescriptor desc,qTrieIterator *iter,qBinarySafeString prefix);



#endif