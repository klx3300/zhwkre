#include "../trie.h"
#include "../error.h"
#include <stdlib.h>
#include <string.h>

#define TYPEALLOC(x) malloc(sizeof(x))
#define ZEROINIT(x) memset(&(x),0,sizeof(x))

static qTrieNode* qTrieNode__constructor(){
    qTrieNode* tmp = TYPEALLOC(qTrieNode);
    ZEROINIT(*tmp);
    tmp->value = qbss_constructor();
    tmp->payload = qbss_constructor();
    return tmp;
}

// will destruct any attached nodes.
// if you want to delete this only
// ZEROINIT children array first
static void qTrieNode__destructor(qTrieNode* node){
    qbss_destructor(node->value);
    qbss_destructor(node->payload);
    for(int i=0;i<255;i++){
        if(node->children[i] != NULL){
            qTrieNode__destructor(node->children[i]);
        }
    }
    free(node);
}

qTrieDescriptor qTrie_constructor(){
    qTrieDescriptor desc;
    ZEROINIT(desc);
    return desc;
}

int qTrie__ptr_at(qTrieDescriptor desc,qTrieIterator *iter,qBinarySafeString prefix){
    qTrieNode* it = desc.root;
    if(it == NULL){
        SETERR(ZHWK_ERR_TREE_ELEM_NOTEXIST);
        return -1;
    }
    for(int i=0;i<prefix.size;i++){
        if(it->children[prefix.str[i]] == NULL){
            SETERR(ZHWK_ERR_TREE_ELEM_NOTEXIST);
            return -1;
        }
        it = it->children[prefix.str[i]];
    }
    qTrieIterator tmpiter;
    tmpiter.limit = desc.root;
    tmpiter.realiter = it;
    *iter = tmpiter;
    return 0;
}

int qTrie__insert(qTrieDescriptor* desc,qBinarySafeString key,void* payload,unsigned int size){
    qTrieNode *it = desc->root;
    if(it == NULL){
        // haven't inited yet!!
        desc->root = qTrieNode__constructor();
        it = desc->root;
    }
    for(int i=0;i<key.size;i++){
        if(it->children[key.str[i]] == NULL){
            // need insertion here
            it->children[key.str[i]] = qTrieNode__constructor();
            qTrieNode* tmpnd = it->children[key.str[i]];
            q__bss_append(&(tmpnd->value),key.str,i);
        }
        it = it->children[key.str[i]];
    }
    // Reached END
    qbss_destructor(it->payload);
    it->payload = qbss_constructor();
    q__bss_append(&(it->payload),payload,size);
    return 0;
}