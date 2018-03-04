#ifndef Q_ZHWKRE_AVLTREE_H
#define Q_ZHWKRE_AVLTREE_H

struct q__avltree_node_st{
    void* data;
    unsigned int size;
    struct q__avltree_node_st *lchild;
    struct q__avltree_node_st *rchild;
    struct q__avltree_node_st *prev;
    struct q__avltree_node_st *next;
    int height;
};

struct q__avltree_desc_st{
    unsigned int size;
    struct q__avltree_node_st *root;
    int (*comp)(void* a,unsigned int asize,void* b,unsigned int bsize);
};

struct q__avltree_iter_st{
    struct q__avltree_node_st *node;
};

typedef struct q__avltree_iter_st qAVLTreeIterator;
typedef struct q__avltree_node_st qAVLTreeNode;
typedef struct q__avltree_desc_st qAVLTreeDescriptor;

qAVLTreeDescriptor qAVLTree_constructor(int (*comp)(void*,unsigned int,void*,unsigned int));

#define qAVLTree_ptr_at(desc,iter,k) qAVLTree__ptr_at(desc,&(iter),&(k),sizeof(k))
int qAVLTree__ptr_at(qAVLTreeDescriptor desc,qAVLTreeIterator *iter,void* elem,unsigned int elemlen);

#define qAVLTree_insert(desc,elem) qAVLTree__insert(&(desc),&(elem),sizeof(elem))
int qAVLTree__insert(qAVLTreeDescriptor *desc,void* elem,unsigned int elemsize);

#define qAVLTree_erase(desc,iter) qAVLTree__erase(&(desc),iter)
int qAVLTree__erase(qAVLTreeDescriptor *desc,qAVLTreeIterator elem);

#define qAVLTree_destructor(desc) qAVLTree__destructor(&(desc))
int qAVLTree__destructor(qAVLTreeDescriptor *desc);

qAVLTreeIterator qAVLTree_begin(qAVLTreeDescriptor desc);
qAVLTreeIterator qAVLTree_end(qAVLTreeDescriptor desc);

int qAVLTreeIterator_isvalid(qAVLTreeIterator iter);
void* qAVLTreeIterator_deref(qAVLTreeIterator iter);

qAVLTreeIterator qAVLTreeIterator_prev(qAVLTreeIterator iter);
qAVLTreeIterator qAVLTreeIterator_next(qAVLTreeIterator iter);

#endif