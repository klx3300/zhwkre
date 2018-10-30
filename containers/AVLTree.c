#include "../AVLTree.h"
#include "../unidef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned int ui;
typedef int (*Cmp)(void*,ui,void*,ui);

#define qAVLTreeNode__updateHeight(rootnd) (MAX(qAVLTreeNode__getHeight((rootnd)->lchild),qAVLTreeNode__getHeight((rootnd)->rchild))+1)
#define qAVLTreeNode__get_blfactor(rootnd) (qAVLTreeNode__getHeight((rootnd)->rchild)-qAVLTreeNode__getHeight((rootnd)->lchild))

int qAVLTreeNode__getHeight(qAVLTreeNode* tn){
    if(tn == NULL) return 0;
    return tn->height;
}

qAVLTreeNode* qAVLTreeNode__constructor(){
    qAVLTreeNode* it=malloc(sizeof(qAVLTreeNode));
    if(it == NULL){
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
            if(iter!=NULL) iter->node = searchreach;
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
        return NULL;
    }
    // perform rotation
    qAVLTreeNode *rrchild = (root->rchild);
    qAVLTreeNode *rrlchild = root->rchild->lchild;
    qAVLTreeNode *rrrchild = root->rchild->rchild;
    if(rrrchild == NULL){
        // merge
        qAVLTreeNode *rrllchild = rrlchild->lchild;
        qAVLTreeNode *rrlrchild = rrlchild->rchild;
        root->rchild = rrllchild;
        rrchild->lchild = rrlrchild;
        rrlchild->lchild = root;
        rrlchild->rchild = rrchild;
        root->height = qAVLTreeNode__updateHeight(root);
        rrchild->height = qAVLTreeNode__updateHeight(rrchild);
        rrlchild->height = qAVLTreeNode__updateHeight(rrlchild);
        return rrlchild;
    }else{
        root->rchild = rrchild->lchild;
        rrchild->lchild = root;
        // update height accordingly
        root->height = qAVLTreeNode__updateHeight(root);
        rrchild -> height = qAVLTreeNode__updateHeight(rrchild);
        return rrchild;
    }
}

qAVLTreeNode* qAVLTree__SimpleRightRotate(qAVLTreeNode* root){
    if(root->lchild == NULL){
        return NULL;
    }
    qAVLTreeNode *rlchild = (root->lchild);
    qAVLTreeNode *rllchild = root->lchild->lchild;
    qAVLTreeNode* rlrchild = root->lchild->rchild;
    // look SimpleLeftRotate for details..
    if(rllchild == NULL){
        // merge
        qAVLTreeNode* rlrlchild = rlrchild->lchild;
        qAVLTreeNode* rlrrchild = rlrchild->rchild;
        rlchild->rchild = rlrlchild;
        root->lchild = rlrrchild;
        rlrchild->lchild = rlchild;
        rlrchild->rchild = root;
        root->height = qAVLTreeNode__updateHeight(root);
        rlchild->height = qAVLTreeNode__updateHeight(rlchild);
        rlrchild->height = qAVLTreeNode__updateHeight(rlrchild);
        return rlrchild;
    }else{
        root->lchild = rlchild->rchild;
        rlchild->rchild = root;
        // need update height
        root->height = qAVLTreeNode__updateHeight(root);
        rlchild -> height = qAVLTreeNode__updateHeight(rlchild);
        return rlchild;
    }
}

// any double rotations are combinations of simple rotations
qAVLTreeNode* qAVLTree__LeftRightRotate(qAVLTreeNode* root){
    if(root->lchild == NULL){
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

qAVLTreeNode* qAVLTree__left_balance(qAVLTreeDescriptor desc,qAVLTreeNode* root){
    if(qAVLTreeNode__get_blfactor(root)<=-2){
        // trigger rotation
        if(root->lchild != NULL && root->lchild->rchild != NULL && qAVLTreeNode__get_blfactor(root->lchild->rchild) > 0){
            return qAVLTree__LeftRightRotate(root);
        }else{
            return qAVLTree__SimpleRightRotate(root);
        }
    }
    return root;
}

qAVLTreeNode* qAVLTree__right_balance(qAVLTreeDescriptor desc,qAVLTreeNode* root){
    if(qAVLTreeNode__get_blfactor(root) >= 2){
        if(root->rchild != NULL && root->rchild->lchild != NULL && qAVLTreeNode__get_blfactor(root->rchild->lchild) < 0){
            return qAVLTree__RightLeftRotate(root);
        }else{
            return qAVLTree__SimpleLeftRotate(root);
        }
    }
    return root;
}

// parent: where do i come from?
int qAVLTree__recursive_insert(qAVLTreeDescriptor desc,qAVLTreeNode* root,void* elem,ui size,qAVLTreeNode** newrootptr){
    int cmpresult = (desc.comp(elem,size,root->data,root->size));
    if(cmpresult == 0){
        // unfortunately, already exists.
        *newrootptr = root;
        return -1;
    }else if(cmpresult < 0){
        if(root->lchild == NULL){
            // well, you there then!
            qAVLTreeNode* tmpnd = qAVLTreeNode__constructor();
            if(tmpnd == NULL){
                return -1;
            }
            void* cpdata = malloc(size);
            if(cpdata == NULL){
                free(tmpnd);
                return -1;
            }
            // allocation success!
            ZEROINIT(*tmpnd);
            memcpy(cpdata,elem,size);
            tmpnd->size = size;
            tmpnd->data = cpdata;
            tmpnd->height = 1;
            // complete insertion
            qAVLTree__list_insert_left(root,tmpnd);
            root->lchild = tmpnd;
            root->height = qAVLTreeNode__updateHeight(root);
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
        root->height = qAVLTreeNode__updateHeight(root);
        *newrootptr = qAVLTree__left_balance(desc,root);
        return 0;
    }else{
        if(root->rchild == NULL){
            qAVLTreeNode* tmpnd = qAVLTreeNode__constructor();
            if(tmpnd == NULL){
                return -1;
            }
            void* cpdata = malloc(size);
            if(cpdata == NULL){
                free(tmpnd);
                return -1;
            }
            ZEROINIT(*tmpnd);
            memcpy(cpdata,elem,size);
            tmpnd->size = size;
            tmpnd->data = cpdata;
            tmpnd->height = 1;
            qAVLTree__list_insert_right(root,tmpnd);
            root->rchild = tmpnd;
            // TODO: update height
            root->height = qAVLTreeNode__updateHeight(root);
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
        root->height = qAVLTreeNode__updateHeight(root);
        *newrootptr = qAVLTree__right_balance(desc,root);
        return 0;
    }
}

int qAVLTree__insert(qAVLTreeDescriptor *desc,void* elem,ui size){
    if(desc->root == NULL){
        // the very first elem
        desc->root = malloc(sizeof(qAVLTreeNode));
        if(desc->root == NULL){
            return -1;
        }
        void* cpdata = malloc(size);
        if(cpdata == NULL){
            free(desc->root);
            desc->root = NULL;
            return -1;
        }
        ZEROINIT(*(desc->root));
        memcpy(cpdata,elem,size);
        desc->root->data = cpdata;
        desc->root->size = size;
        desc->root->height = 1;
        desc->size = 1;
        return 0;
    }
    // recursive version kick in
    qAVLTreeNode* newroot = NULL;
    int retv = qAVLTree__recursive_insert(*desc,desc->root,elem,size,&newroot);
    if(retv) return -1;
    desc->size+=1;
    desc->root = newroot;
    return 0;
}

#define qAVLTreeNode__swap(pnda,pndb) {\
    qAVLTreeNode *nda=(pnda),*ndb=(pndb);\
    void* tmpdtswp = nda->data;\
    nda->data = ndb->data;\
    ndb->data = tmpdtswp;\
    ui tmpszswp = nda->size;\
    nda->size = ndb->size;\
    ndb->size = tmpszswp;\
    }

qAVLTreeNode* qAVLTree__delete_leftmax(qAVLTreeDescriptor desc,qAVLTreeNode* root,qAVLTreeIterator elem){
    if(root->rchild != NULL){
        qAVLTreeNode* newroot = qAVLTree__delete_leftmax(desc,root->rchild,elem);
        root->rchild = newroot;
        root->height = qAVLTreeNode__updateHeight(root);
        return qAVLTree__right_balance(desc,root);
    }
    // the rightmost node
    qAVLTreeNode__swap(root,elem.node);
    qAVLTreeNode* remain = root->lchild;
    free(root->data);
    qAVLTree__list_remove(root);
    free(root);
    return remain;
}
qAVLTreeNode* qAVLTree__delete_rightmin(qAVLTreeDescriptor desc,qAVLTreeNode* root,qAVLTreeIterator elem){
    if(root->lchild != NULL){
        qAVLTreeNode* newroot = qAVLTree__delete_rightmin(desc,root->lchild,elem);
        root->lchild = newroot;
        root->height = qAVLTreeNode__updateHeight(root);
        return qAVLTree__left_balance(desc,root);
    }
    // the leftmost node
    qAVLTreeNode__swap(root,elem.node);
    qAVLTreeNode* remain = root->rchild;
    free(root->data);
    qAVLTree__list_remove(root);
    free(root);
    return remain;
}

int qAVLTree__recursive_delete(qAVLTreeDescriptor desc,qAVLTreeNode* root,qAVLTreeIterator elem,qAVLTreeNode** newrootptr){
    int cmpresult = desc.comp(elem.node->data,elem.node->size,root->data,root->size);
    // TODO: completes this
    if(cmpresult == 0){
        // is this a leaf?
        if(qAVLTreeNode__getHeight(root) == 1){
            // clear myself
            *newrootptr = NULL;
            free(root->data);
            qAVLTree__list_remove(root);
            free(root);
            return 0;
        }else{
            // select!
            if(qAVLTreeNode__get_blfactor(root)>0){
                qAVLTreeNode* newroot = qAVLTree__delete_rightmin(desc,root->rchild,elem);
                root->rchild = newroot;
                root->height = qAVLTreeNode__updateHeight(root);
                *newrootptr = qAVLTree__right_balance(desc,root);
                return 0;
            }else{
                qAVLTreeNode* newroot = qAVLTree__delete_leftmax(desc,root->lchild,elem);
                root->lchild = newroot;
                root->height = qAVLTreeNode__updateHeight(root);
                *newrootptr = qAVLTree__left_balance(desc,root);
                return 0;
            }
        }
    }else if(cmpresult < 0){
        if(root->lchild == NULL){
            *newrootptr = root;
            return -1;
        }
        qAVLTreeNode* newroot = NULL;
        int status = qAVLTree__recursive_delete(desc,root->lchild,elem,&newroot);
        *newrootptr = root;
        if(status) return status;
        root->lchild = newroot;
        root->height = qAVLTreeNode__updateHeight(root);
        *newrootptr = qAVLTree__right_balance(desc,root);
        return 0;
    }else{
        if(root->rchild == NULL){
            *newrootptr = root;
            return -1;
        }
        qAVLTreeNode *newroot = NULL;
        int status = qAVLTree__recursive_delete(desc,root->rchild,elem,&newroot);
        *newrootptr = root;
        if(status) return status;
        root->rchild = newroot;
        root->height = qAVLTreeNode__updateHeight(root);
        *newrootptr = qAVLTree__left_balance(desc,root);
        return 0;
    }
}

int qAVLTree__erase(qAVLTreeDescriptor *desc,qAVLTreeIterator elem){
    if(desc->size == 0) return -1;
    qAVLTreeNode* newroot = NULL;
    int status = qAVLTree__recursive_delete(*desc,desc->root,elem,&newroot);
    if(status) return status;
    desc->size--;
    desc->root = newroot;
    return 0;
}

int qAVLTree__destructor(qAVLTreeDescriptor *desc){
    // destructive free!
    qAVLTreeNode* min_node = desc->root;
    while(min_node != NULL && min_node->lchild != NULL) min_node = min_node->lchild;
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
    while(min_node != NULL && min_node->lchild != NULL) min_node = min_node->lchild;
    qAVLTreeIterator it;
    it.node = min_node;
    return it;
}

qAVLTreeIterator qAVLTree_end(qAVLTreeDescriptor desc){
    qAVLTreeNode* max_node = desc.root;
    while(max_node != NULL && max_node->rchild != NULL) max_node = max_node->rchild;
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
