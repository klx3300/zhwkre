#ifndef Q_ZHWKRE_TRIE_H
#define Q_ZHWKRE_TRIE_H

#include "bss.h"

struct q__trie_node_st{
    qBinarySafeString value;
    qBinarySafeString payload;
    struct q__trie_node_st *parent;
    struct q__trie_node_st *children[256];
};

struct q__trie_desc_st{
    int size;
    struct q__trie_node_st* root;
};

struct q__trie_iter_st{
    struct q__trie_node_st *realiter;
    struct q__trie_node_st *limit;
};

typedef struct q__trie_node_st qTrieNode;
typedef struct q__trie_desc_st qTrieDescriptor;
typedef struct q__trie_iter_st qTrieIterator;

qTrieDescriptor qTrie_constructor();

#define qTrie_ptr_at(desc,dstit,bss) qTrie__ptr_at(desc,&(dstit),bss)
int qTrie__ptr_at(qTrieDescriptor desc,qTrieIterator *iter,qBinarySafeString prefix);

#define qTrie_insert(desc,bss,payload) qTrie__insert(&(desc),bss,&(payload),sizeof(payload))
int qTrie__insert(qTrieDescriptor* desc,qBinarySafeString bss,void* payload,unsigned int payloadsize);

#define qTrie_erase(desc,iter) qTrie__erase(&(desc),iter)
int qTrie__erase(qTrieDescriptor *desc,qTrieIterator iter);

#define qTrie_destructor(desc) qTrie__destructor(&(desc))
int qTrie__destructor(qTrieDescriptor* desc);

#define qTrie_begin(desc) qTrie__begin((desc).root)
qTrieIterator qTrie__begin(qTrieNode* root);

#define qTrie_end(desc) qTrie__end((desc).root)
qTrieIterator qTrie__end(qTrieNode* root);

int qTrieIterator_isvalid(qTrieIterator i);
void* qTrieIterator_deref(qTrieIterator i);
qTrieIterator qTrieIterator_prev(qTrieIterator i);
qTrieIterator qTrieIterator_next(qTrieIterator i);

qTrieIterator qTrieIterator_ranged(qTrieIterator i);

#endif