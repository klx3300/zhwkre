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
    // update blfactor accordingly
    rrchild->blfactor = rrchild->blfactor-1;
    root->blfactor = root->blfactor-1;
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
    rlchild->blfactor=rlchild->blfactor+1;
    root->blfactor=root->blfactor+1;
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
qAVLTreeNode* qAVLTree__recursive_insert(qAVLTreeDescriptor desc,qAVLTreeNode* root,void* elem,ui size){
    int cmpresult = (desc.comp(elem,size,root->data,root->size));
    if(cmpresult == 0){
        // unfortunately, already exists.
        return NULL;
    }else if(cmpresult < 0){
        if(root->lchild == NULL){
            // well, you there then!
            qAVLTreeNode* tmpnd = malloc(sizeof(qAVLTreeNode));
            if(tmpnd == NULL){
                SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
                return NULL;
            }
            void* cpdata = malloc(size);
            if(cpdata == NULL){
                SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
                free(tmpnd);
                return NULL;
            }
            // allocation success!
            ZEROINIT(*tmpnd);
            memcpy(cpdata,elem,size);
            tmpnd->size = size;
            tmpnd->data = cpdata;
            // complete insertion
            qAVLTree__list_insert_left(root,tmpnd);
            root->lchild = tmpnd;
            // update balance factor
            root->blfactor=root->blfactor-1;
            return root;
        }
        qAVLTreeNode* insresult = qAVLTree__recursive_insert(desc,root->lchild,elem,size);
        if(insresult == NULL){
            return NULL;
        }
        // then the insertion is succeeded.
        // update root
        root->lchild = insresult;
        // update blfactor
        root->blfactor=root->blfactor-1;
        if(root->blfactor <= -2){
            // trigger rotation
            if(root->lchild->rchild == NULL || root->lchild->rchild->blfactor<=0){
                // in this case ,simple right rotate is good enough.
                return qAVLTree__SimpleRightRotate(root);
            }else{
                return qAVLTree__LeftRightRotate(root);
            }
        }
        return root;
    }else{
        if(root->rchild == NULL){
            qAVLTreeNode* tmpnd = malloc(sizeof(qAVLTreeNode));
            if(tmpnd == NULL){
                SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
                return NULL;
            }
            void* cpdata = malloc(size);
            if(cpdata == NULL){
                SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
                free(tmpnd);
                return NULL;
            }
            ZEROINIT(*tmpnd);
            memcpy(cpdata,elem,size);
            tmpnd->size = size;
            tmpnd->data = cpdata;
            qAVLTree__list_insert_right(root,tmpnd);
            root->rchild = tmpnd;
            root->blfactor = root->blfactor+1;
            return root;
        }
        qAVLTreeNode* insresult = qAVLTree__recursive_insert(desc,root->rchild,elem,size);
        if(insresult == NULL){
            return NULL;
        }
        root->rchild = insresult;
        root->blfactor = root->blfactor+1;
        if(root->blfactor >= 2){
            if(root->rchild->lchild != NULL || root->rchild->lchild->blfactor >= 0){
                return qAVLTree__SimpleLeftRotate(root);
            }else{
                return qAVLTree__RightLeftRotate(root);
            }
        }
        return root;
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
    qAVLTreeNode* retv = qAVLTree__recursive_insert(*desc,desc->root,elem,size);
    if(retv == NULL) return -1;
    desc->size=desc->size+1;
    desc->root = retv;
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
    if(cmpresult == 0){
        // yes we get it!
        // is this a leaf or not?
        // the simple way: check blfactor & lchild null
        if(root->blfactor == 0){
            // balanced!
            // if lucky enough, it is a leaf.
            if(root->lchild == NULL){
                // a leaf
                // then simply boom myself!!
                // or not..
                if(opercode != DELETE_OPER_FIND){
                    // need swap
                    void* tmpdtswp = root->data;
                    root->data = elem.node->data;
                    elem.node->data = tmpdtswp;
                    unsigned int tmpszswp = root->size;
                    root->size = elem.node->size;
                    elem.node->size = tmpszswp;
                }
                free(root->data);
                // remove myself..
                qAVLTree__list_remove(root);
                *newrootptr = NULL;
                free(root);
                return 0;
            }
            // both child exists, obviously
            // randomly select one..
            // i choose.. leftmax!
            qAVLTreeNode* newroot=NULL;
            int status = qAVLTree__recursive_delete(desc,root->lchild,elem,&newroot,DELETE_OPER_LEFTMAX);
            if(status) return status;
            root->lchild = newroot;
            root->blfactor = root->blfactor+1;
            if(root->blfactor >= 2){
                // trigger rotation
                if(root->rchild->lchild != NULL || root->rchild->lchild->blfactor >= 0){
                    *newrootptr = qAVLTree__SimpleLeftRotate(root);
                }else{
                    *newrootptr = qAVLTree__RightLeftRotate(root);
                }
            }
            return 0;
        }else if(root->blfactor < 0){
            // left heavier => leftmax
            qAVLTreeNode* newroot=NULL;
            int status = qAVLTree__recursive_delete(desc,root->lchild,elem,&newroot,DELETE_OPER_LEFTMAX);
            if(status) return status;
            root->lchild = newroot;
            root->blfactor = root->blfactor+1;
            if(root->blfactor >= 2){
                // trigger rotation
                if(root->rchild->lchild != NULL || root->rchild->lchild->blfactor >= 0){
                    *newrootptr = qAVLTree__SimpleLeftRotate(root);
                }else{
                    *newrootptr = qAVLTree__RightLeftRotate(root);
                }
            }
            return 0;
        }else{
            // right heavier => rightmin
            qAVLTreeNode* newroot = NULL;
            int status = qAVLTree__recursive_delete(desc,root->rchild,elem,&newroot,DELETE_OPER_RIGHTMIN);
            if(status) return status;
            root->rchild = newroot;
            root->blfactor = root->blfactor-1;
            if(root->blfactor <= -2){
                // trigger rotation
                if(root->lchild->rchild == NULL || root->lchild->rchild->blfactor<=0){
                    *newrootptr = qAVLTree__SimpleRightRotate(root);
                }else{
                    *newrootptr = qAVLTree__LeftRightRotate(root);
                }
            }
            return 0;
        }
    }else if((cmpresult < 0 && opercode == DELETE_OPER_FIND) || opercode == DELETE_OPER_RIGHTMIN){
        // elem < root
        if(root->lchild == NULL){
            if(opercode != DELETE_OPER_RIGHTMIN) return 1;
            // a delete_right_min reached minimum right node!
            // hint: rchild can't be null. WHY?
            // swap!
            void* tmpdtswp = root->data;
            root->data = elem.node->data;
            elem.node->data = tmpdtswp;
            unsigned int tmpszswp = root->size;
            root->size = elem.node->size;
            elem.node->size = tmpszswp;
            // save the rchild -- it will be the new root!
            qAVLTreeNode* newroot = root->rchild;
            // boom myself
            free(root->data);
            qAVLTree__list_remove(root);
            free(root);
            *newrootptr = newroot;
            return 0;
        }
        qAVLTreeNode* newroot=NULL;
        int status = qAVLTree__recursive_delete(desc,root->lchild,elem,&newroot,DELETE_OPER_FIND);
        if(status) return status;
        root->lchild = newroot;
        // update blfactor
        root->blfactor = root->blfactor+1;
        if(root->blfactor >= 2){
            // trigger rotation
            if(root->rchild->lchild != NULL || root->rchild->lchild->blfactor >= 0){
                *newrootptr = qAVLTree__SimpleLeftRotate(root);
            }else{
                *newrootptr = qAVLTree__RightLeftRotate(root);
            }
        }
        return 0;
    }else if((cmpresult > 0 && opercode == DELETE_OPER_FIND) || opercode == DELETE_OPER_LEFTMAX){
        // elem > root
        if(root->rchild == NULL){
            if(opercode != DELETE_OPER_LEFTMAX) return 1;
            // a delete_left_max reached maximum left node!
            void* tmpdtswp = root->data;
            root->data = elem.node->data;
            elem.node->data = tmpdtswp;
            unsigned int tmpszswp = root->size;
            root->size = elem.node->size;
            elem.node->size = tmpszswp;
            // save the lchild -- it will be the new root!
            qAVLTreeNode* newroot = root->lchild;
            // boom myself
            free(root->data);
            qAVLTree__list_remove(root);
            free(root);
            *newrootptr = newroot;
            return 0;
        }
        qAVLTreeNode* newroot=NULL;
        int status = qAVLTree__recursive_delete(desc,root->rchild,elem,&newroot,DELETE_OPER_FIND);
        if(status) return status;
        root->rchild = newroot;
        root->blfactor = root->blfactor-1;
        if(root->blfactor <= -2){
            if(root->lchild->rchild == NULL || root->lchild->rchild->blfactor<=0){
                // in this case ,simple right rotate is good enough.
                *newrootptr = qAVLTree__SimpleRightRotate(root);
            }else{
                *newrootptr = qAVLTree__LeftRightRotate(root);
            }
        }
        return 0;
    }else{
        SETERR(ZHWK_ERR_TREE_UNEXPECTED_CASE);
        return -1;
    }
}

int qAVLTree__erase(qAVLTreeDescriptor *desc,qAVLTreeIterator elem){
    if(desc->size == 0) return -1;
    qAVLTreeNode* newroot = NULL;
    int status = qAVLTree__recursive_delete(*desc,desc->root,elem,&newroot,DELETE_OPER_FIND);
    if(status) return status;
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
