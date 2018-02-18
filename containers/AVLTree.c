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
        int cmpresult = desc.comp(elem,elemsize,searchreach->data,searchreach->size);
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
qAVLTreeNode* qAVLTree__LeftRightRotate(qAVLTreeNode* root){
    if(root->lchild == NULL){
        SETERR(ZHWK_ERR_TREE_INVALID_OPERATION);
        return NULL;
    }
    qAVLTreeNode* tmpreroot = qAVLTree__SimpleLeftRotate(root->lchild);
    if(tmpreroot == NULL){
        // zhwkerr already set, return immediately is OJBK
        return NULL;
    }
    root->lchild = tmpreroot;
    return qAVLTree__SimpleRightRotate(root);
}

qAVLTreeNode* qAVLTree__RightLeftRotate(qAVLTreeNode* root){
    if(root->rchild == NULL){
        SETERR(ZHWK_ERR_TREE_INVALID_OPERATION);
        return NULL;
    }
    qAVLTreeNode* tmpreroot = qAVLTree__SimpleRightRotate(root->rchild);
    if(tmpreroot == NULL){
        return NULL;
    }
    root->rchild = tmpreroot;
    return qAVLTree__SimpleLeftRotate(root);
}

void qAVLTree__list_insert_left(qAVLTreeNode* parent,qAVLTreeNode* child){
    if(parent->prev != NULL){
        parent->prev->next = child;
    }
    child->prev = parent->prev;
    child->next = parent;
    parent->prev = child;
}

void qAVLTree__list_insert_right(qAVLTreeNode* parent,qAVLTreeNode* child){
    if(parent->next != NULL){
        parent->next->prev = child;
    }
    child->next = parent->next;
    child->prev = parent;
    parent->next = child;
}

// parent: where do i come from?
int qAVLTree__recursive_insert(qAVLTreeDescriptor desc,qAVLTreeNode* root,void* elem,ui size){
    int cmpresult = (desc.comp(elem,size,root->data,root->size));
    if(cmpresult == 0){
        // unfortunately, already exists.
        return -1;
    }else if(cmpresult < 0){
        if(root->lchild == NULL){
            // well, you there then!
            qAVLTreeNode* tmpnd = malloc(sizeof(qAVLTreeNode));
            if(tmpnd == NULL){
                SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
                return -1;
            }
            void* cpdata = malloc(size);
            if(cpdata == NULL){
                SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
                free(tmpnd);
                return -1;
            }
            // allocation success!
            ZEROINIT(*tmpnd);
            memcpy(cpdata,elem,size);
            tmpnd->size = size;
            tmpnd->data = cpdata;
            // complete insertion
            qAVLTree__list_insert_left(root,cpdata);
            root->lchild = cpdata;
            return 0;
        }
        int insresult = qAVLTree__recursive_insert(desc,root->lchild,elem,size);
        if(insresult == -1){
            return -1;
        }
    }else{

    }
}

int qAVLTree__insert(qAVLTreeDescriptor *desc,void* elem,ui size){

}