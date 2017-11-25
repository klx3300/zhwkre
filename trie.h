#ifndef Q_ZHWKRE_TRIE_H
#define Q_ZHWKRE_TRIE_H

#include "bss.h"

struct q__trie_node_st{
    qBinarySafeString value;
    struct q__trie_node_st *children[256];
};

struct q__trie_desc_st{
    int size;
    struct q__trie_node_st* root;
};

typedef struct q__trie_node_st qTrieNode;
typedef struct q__trie_desc_st qTrieDescriptor;

qTrieDescriptor qTrie_constructor();

#endif