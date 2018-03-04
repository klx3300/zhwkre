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
    // update height accordingly
    // TODO
    return rrchild;
}

qAVLTreeNode* qAVLTree__SimpleRightRotate(qAVLTreeNode* root){
    if(root->lchild == NULL){
        SETERR(ZHWK_ERR_TREE_INVALID_OPERATION);
        return NULL;
    }
    qAVLTreeNode *rlchild = (root->lchild);
    // look SimpleLeftRotate for details..
    root->lchild = rlchild->rchild;
    rlchild->rchild = root;
    // need update height
    // TODO
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

void qAVLTree__list_remove(qAVLTreeNode* elem){
    if(elem->prev!=NULL) elem->prev->next = elem->next;
    if(elem->next!=NULL) elem->next->prev = elem->prev;
}

// parent: where do i come from?
int qAVLTree__recursive_insert(qAVLTreeDescriptor desc,qAVLTreeNode* root,void* elem,ui size,qAVLTreeNode** newrootptr){
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
            qAVLTree__list_insert_left(root,tmpnd);
            root->lchild = tmpnd;
            // update height
            // TODO
            *newrootptr = root;
            return 0;
        }
        qAVLTreeNode* newroot = NULL;
        int insresult = qAVLTree__recursive_insert(desc,root->lchild,elem,size,&newroot);
        root->lchild = newroot;
        if(insresult != 0){
            *newrootptr = root;
            return insresult;
        }
        // TODO: update height and trigger rotation
        if(??){
            // trigger rotation
            if(root->lchild->rchild != NULL && ??){
                *newrootptr = qAVLTree__LeftRightRotate(root);
            }else{
                *newrootptr = qAVLTree__SimpleRightRotate(root);
            }
            return 0;
        }
        *newrootptr = root;
        return 0;
    }else{
        if(root->rchild == NULL){
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
            ZEROINIT(*tmpnd);
            memcpy(cpdata,elem,size);
            tmpnd->size = size;
            tmpnd->data = cpdata;
            qAVLTree__list_insert_right(root,tmpnd);
            root->rchild = tmpnd;
            // TODO: update height
            *newrootptr = root;
            return 0;
        }
        qAVLTreeNode* newroot = NULL;
        int insresult = qAVLTree__recursive_insert(desc,root->rchild,elem,size,&newroot);
        root->rchild = newroot;
        if(insresult != 0){
            *newrootptr = root;
            return insresult;
        }
        // TODO: update height & trigger rotation
        if(??){
            if(root->rchild->lchild != NULL && ??){
                *newrootptr = qAVLTree__RightLeftRotate(root);
            }else{
                *newrootptr = qAVLTree__SimpleLeftRotate(root);
            }
            return 0;
        }
        *newrootptr = root;
        return -1;
    }
}

int qAVLTree__insert(qAVLTreeDescriptor *desc,void* elem,ui size){
    if(desc->root == NULL){
        // the very first elem
        desc->root = malloc(sizeof(qAVLTreeNode));
        if(desc->root == NULL){
            SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
            return -1;
        }
        void* cpdata = malloc(size);
        if(cpdata == NULL){
            SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
            free(desc->root);
            desc->root = NULL;
            return -1;
        }
        ZEROINIT(*(desc->root));
        memcpy(cpdata,elem,size);
        desc->root->data = cpdata;
        desc->root->size = size;
        desc->size = 1;
        return 0;
    }
    // recursive version kick in
    qAVLTreeNode* newroot = NULL;
    int retv = qAVLTree__recursive_insert(*desc,desc->root,elem,size,&newroot);
    if(retv == INSERT_OPER_FAILED) return -1;
    desc->size+=1;
    desc->root = newroot;
    return 0;
}

// this enum is used to embed the whole deletion possibilites into
// one recursive delete function.
enum{
    DELETE_OPER_FIND = 0, // still did not found the elem
    DELETE_OPER_LEFTMAX, // to_replace is left-max
    DELETE_OPER_RIGHTMIN // to_replace is right-min
};

int qAVLTree__recursive_delete(qAVLTreeDescriptor desc,qAVLTreeNode* root,qAVLTreeIterator elem,qAVLTreeNode** newrootptr,int opercode){
    int cmpresult = desc.comp(elem.node->data,elem.node->size,root->data,root->size);
    // TODO: completes this
    if(cmpresult == 0){
    }else if((cmpresult < 0 && opercode == DELETE_OPER_FIND) || opercode == DELETE_OPER_RIGHTMIN){
    }else if((cmpresult > 0 && opercode == DELETE_OPER_FIND) || opercode == DELETE_OPER_LEFTMAX){
    }else{
        SETERR(ZHWK_ERR_TREE_UNEXPECTED_CASE);
        return -1;
    }
}

int qAVLTree__erase(qAVLTreeDescriptor *desc,qAVLTreeIterator elem){
    if(desc->size == 0) return -1;
    qAVLTreeNode* newroot = NULL;
    int status = qAVLTree__recursive_delete(*desc,desc->root,elem,&newroot,DELETE_OPER_FIND);
    if(status == -1) return status;
    desc->size--;
    desc->root = newroot;
    return 0;
}

int qAVLTree__destructor(qAVLTreeDescriptor *desc){
    // destructive free!
    qAVLTreeNode* min_node = desc->root;
    while(min_node->lchild != NULL) min_node = min_node->lchild;
    for(;min_node!=NULL;){
        qAVLTreeNode* nextnode = min_node->next;
        free(min_node->data);
        free(min_node);
        min_node = nextnode;
    }
    memset(desc,0,sizeof(qAVLTreeDescriptor));
    return 0;
}

qAVLTreeIterator qAVLTree_begin(qAVLTreeDescriptor desc){
    qAVLTreeNode* min_node = desc.root;
    while(min_node->lchild != NULL) min_node = min_node->lchild;
    qAVLTreeIterator it;
    it.node = min_node;
    return it;
}

qAVLTreeIterator qAVLTree_end(qAVLTreeDescriptor desc){
    qAVLTreeNode* max_node = desc.root;
    while(max_node->rchild != NULL) max_node = max_node->rchild;
    qAVLTreeIterator it;
    it.node = max_node;
    return it;
}

int qAVLTreeIterator_isvalid(qAVLTreeIterator iter){
    return iter.node != NULL;
}

void* qAVLTreeIterator_deref(qAVLTreeIterator iter){
    if(qAVLTreeIterator_isvalid(iter)) return iter.node->data;
    return NULL;
}

qAVLTreeIterator qAVLTreeIterator_prev(qAVLTreeIterator iter){
    qAVLTreeIterator priter;
    priter.node = iter.node->prev;
    return priter;
}

qAVLTreeIterator qAVLTreeIterator_next(qAVLTreeIterator iter){
    qAVLTreeIterator priter;
    priter.node = iter.node->next;
    return priter;
}
