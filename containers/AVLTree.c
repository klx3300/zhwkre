#include "../AVLTree.h"
#include "../error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PTR(x) (&(x))
#define VPTR(x) ((void*)(&(x)))
#define DEREF(x) (*(x))
#define ZEROINIT(x) memset(&(x),0,sizeof(x))

typedef unsigned int ui;
typedef int (*Cmp)(void*,ui,void*,ui);

qAVLTreeNode* qAVLTreeNode__constructor(){
    qAVLTreeNode* it=malloc(sizeof(qAVLTreeNode));
    if(it == NULL){
        SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
        return NULL;
    }
    memset(it,0,sizeof(qAVLTreeNode));
    return it;
}

qAVLTreeDescriptor qAVLTree_constructor(Cmp cmper){
    qAVLTreeDescriptor desc;
    desc.size = 0;
    desc.root = NULL;
    desc.comp = cmper;
    return desc;
}

int qAVLTree__ptr_at(qAVLTreeDescriptor desc,qAVLTreeIterator *iter,void* elem,ui elemsize){
    if(desc.root == NULL) return -1;
    qAVLTreeNode* searchreach = desc.root;
    while(searchreach != NULL){
        int cmpresult = desc.comp(searchreach->data,searchreach->size,elem,elemsize);
        if(cmpresult == 0){
            iter->node = searchreach;
            return 0;
        }
        if(cmpresult < 0){
            // less
            searchreach = searchreach->lchild;
        }else{
            // larger
            searchreach = searchreach->rchild;
        }
    }
    return -1;// not found
}

// all rotation functions returns the new root pointers
// if there are needs to manage memory, already handled inside.
qAVLTreeNode* qAVLTree__SimpleLeftRotate(qAVLTreeNode* root){
    if(root->rchild == NULL){
        // no you have to be kidding me.
        SETERR(ZHWK_ERR_TREE_INVALID_OPERATION);
        return NULL;
    }
    // perform rotation
    qAVLTreeNode *rrchild = (root->rchild);
    root->rchild = rrchild->lchild;
    rrchild->lchild = root;
    return rrchild;
}

qAVLTreeNode* qAVLTree__SimpleRightRotate(qAVLTreeNode* root){
    if(root->lchild == NULL){
        SETERR(ZHWK_ERR_TREE_INVALID_OPERATION);
        return NULL;
    }
    qAVLTreeNode *rlchild = (root->lchild);
    root->lchild = rlchild->rchild;
    rlchild->rchild = root;
    return rlchild;
}

// any double rotations are combinations of simple rotations
// keep that in mind to fully understand that.
qAVLTreeNode* qAVLTree__LeftRightRotate(qAVLTreeNode* root){

}

