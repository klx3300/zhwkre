#ifndef Q_ZHWKRE_BTREE_H
#define Q_ZHWKRE_BTREE_H

#include "pair.h"

struct q__btree_node_st{
    qPair kv[2];
    struct q__btree_node_st* childs[3];
    qPair extrakv; // for delayed split
    struct q__btree_node_st* extrachild; // for delayed split again
};

struct q__btree_desc_st{
    //char magic[10];
    // list have magic because school assigned projects.
    // that's really a bad idea.
    unsigned int size;
    struct q__btree_node_st* root;
    int (*comp)(void* a,unsigned int asize,void* b,unsigned int bsize);
};

// comparator behavior:
// a>b ==> positive
// a=b ==> zero
// a<b ==> negative

// use to implement O(1) prev/next
struct q__btree_track_st{
    struct q__btree_node_st *parent;
    int childno;
};

struct q__btree_iter_st{
    struct q__btree_node_st* node;
    int which;
    // 2^100 elem? are you serious?
    // i'm not going to write a constant for it.
    // found that's annoying.
    struct q__btree_track_st track[100];
    int top;
};

typedef struct q__btree_node_st qBTreeNode;
typedef struct q__btree_iter_st qBTreeIterator;
typedef struct q__btree_desc_st qBTreeDescriptor;

qBTreeDescriptor qBTree_constructor(int (*comp)(void*,unsigned int,void*,unsigned int));

#define qBTree_ptr_at(desc,iter,k) qBTree__ptr_at(desc,&(iter),&(k),sizeof(k))
int qBTree__ptr_at(qBTreeDescriptor desc,qBTreeIterator* iter,void* key,unsigned int keysize);

#define qBTree_insert(desc,k,v) qBTree__insert(&(desc),&(k),sizeof(k),&(v),sizeof(v))
int qBTree__insert(qBTreeDescriptor* desc,void* key,unsigned int keysize,void* value,unsigned int valuesize);

// erase operation may cause existing iterators invalidated.
// keep that in mind.
#define qBTree_erase(desc,iter) qBTree__erase(&(desc),iter)
int qBTree__erase(qBTreeDescriptor* desc,qBTreeIterator elem);

#define qBTree_destructor(desc) qBTree__destructor(&(desc))
int qBTree__destructor(qBTreeDescriptor* desc);

qBTreeIterator qBTree_begin(qBTreeDescriptor desc);
qBTreeIterator qBTree_end(qBTreeDescriptor desc);

int qBTreeIterator_isvalid(qBTreeIterator i);
qPair qBTreeIterator_deref(qBTreeIterator i);
qBTreeIterator qBTreeIterator_prev(qBTreeIterator i);
qBTreeIterator qBTreeIterator_next(qBTreeIterator i);

#endif