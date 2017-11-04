#include "../btree.h"
#include "../error.h"
#include <stdlib.h>
#include <string.h>

#define PTR(x) (&(x))
#define VPTR(x) ((void*)(&(x)))
#define deref(x) (*(x))
#define ZEROINIT(x) memset(&(x),0,sizeof(x))
#define PCHILD(x,n) (deref(x)->childs[n])

typedef unsigned int ui;
typedef int (*comper)(void*,ui,void*,ui);

qBTreeNode* qBTreeNode__constructor(){
    qBTreeNode* it = malloc(sizeof(qBTreeNode));
    if(it == NULL){
        SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
        return NULL;
    }
    memset(it,0,sizeof(qBTreeNode));
    return it;
}

// always use left child to be promote child
qPair qBTreeNode__split_up(qBTreeDescriptor desc,qBTreeNode* nodesplit,qBTreeNode* toprochild){
    int firstcmp = desc.comp(nodesplit->extrakv.key,nodesplit->extrakv.keysize,nodesplit->kv[0].key,nodesplit->kv[0].keysize);
    if(firstcmp < 0){
        qPair topropair = nodesplit->kv[0];
        ZEROINIT(*toprochild);
        toprochild->kv[0]=nodesplit->extrakv;
        toprochild->childs[0]=nodesplit->extrachild;
        toprochild->childs[1]=nodesplit->childs[0];
        nodesplit->kv[0]=nodesplit->kv[1];
        ZEROINIT(nodesplit->kv[1]);
        nodesplit->childs[0]=nodesplit->childs[1];
        nodesplit->childs[1]=nodesplit->childs[2];
        nodesplit->childs[2]=NULL;
        return topropair;
    }
    int secondcmp = desc.comp(nodesplit->extrakv.key,nodesplit->extrakv.keysize,nodesplit->kv[1].key,nodesplit->kv[1].keysize);
    if(secondcmp < 0){
        qPair topropair = nodesplit->extrakv;
        ZEROINIT(*toprochild);
        toprochild->kv[0]=nodesplit->kv[0];
        nodesplit->kv[0]=nodesplit->kv[1];
        ZEROINIT(nodesplit->kv[1]);
        toprochild->childs[0]=nodesplit->childs[0];
        toprochild->childs[1]=nodesplit->extrachild;
        nodesplit->childs[0]=nodesplit->childs[1];
        nodesplit->childs[1]=nodesplit->childs[2];
        nodesplit->childs[2]=NULL;
        return topropair;
    }else{
        qPair topropair = nodesplit->kv[1];
        ZEROINIT(*toprochild);
        toprochild->kv[0]=nodesplit->kv[0];
        nodesplit->kv[0]=nodesplit->extrakv;
        ZEROINIT(nodesplit->kv[1]);
        toprochild->childs[0]=nodesplit->childs[0];
        toprochild->childs[1]=nodesplit->childs[1];
        nodesplit->childs[0]=nodesplit->extrachild;
        nodesplit->childs[1]=nodesplit->childs[2];
        nodesplit->childs[2]=NULL;
        return topropair;
    }
}

qBTreeDescriptor qBTree_constructor(comper comp){
    qBTreeDescriptor desc;
    desc.size = 0;
    desc.root = NULL;
    desc.comp = comp;
    return desc;
}

int qBTree__recursive_search(qBTreeDescriptor desc,qBTreeNode* root,qBTreeIterator* lastit,void* key,unsigned int keysize){
    // increase
    // compare first
    if(root == NULL){
        // not found
        ZEROINIT(*lastit);
        return -1;
    }
    int first_cmp = desc.comp(key,keysize,root->kv[0].key,root->kv[0].keysize);
    if(first_cmp == 0){
        // successive found
        lastit->node = root;
        lastit->which = 0;
        return 0;
    }
    if(first_cmp < 0){
        lastit->track[lastit->top].parent = root;
        lastit->track[lastit->top].childno = 0;
        lastit->top++;
        return qBTree__recursive_search(desc,root->childs[0],lastit,key,keysize);
    }
    if(root->kv[1].key == NULL){
        ZEROINIT(*lastit);
        return -1; // not found
    }
    int second_cmp = desc.comp(key,keysize,root->kv[1].key,root->kv[1].keysize);
    if(second_cmp == 0){
        // successive found
        lastit->node = root;
        lastit->which = 1;
        return 0;
    }
    if(first_cmp < 0){
        lastit->track[lastit->top].parent = root;
        lastit->track[lastit->top].childno = 1;
        lastit->top++;
        return qBTree__recursive_search(desc,root->childs[1],lastit,key,keysize);
    }
    lastit->track[lastit->top].parent =  root;
    lastit->track[lastit->top].childno = 2;
    lastit->top++;
    return qBTree__recursive_search(desc,root->childs[2],lastit,key,keysize);
}

int qBTree_ptr_at(qBTreeDescriptor desc,qBTreeIterator* iter,void* key,unsigned int keysize){
    ZEROINIT(*iter);
    return qBTree__recursive_search(desc,desc.root,iter,key,keysize);
}

// -1:EEXIST 0:SUCC 1:SPLIT
int qBTree__recursive_insert(qBTreeDescriptor* desc,qBTreeNode** root,qPair pair){
    if(*root == NULL){
        *root = qBTreeNode__constructor();
        if(*root == NULL) return -1;
        (*root)->kv[0]=pair;
        return 0;
    }
    if(!(deref(root)->childs[0] || deref(root)->childs[1] || deref(root)->childs[2])){
        // a leaf.
        // check elem numbers first
        if(deref(root)->kv[1].key != NULL){
            // a full leaf.
            // split required
            deref(root)->extrakv = pair;
            return 1; // wait split
        }
        // not full yet
        int firstcmp = desc->comp(pair.key,pair.keysize,deref(root)->kv[0].key,deref(root)->kv[0].keysize);
        if(firstcmp == 0){
            SETERR(ZHWK_ERR_TREE_ELEM_EXIST);
            return -1;
        }
        if(firstcmp > 0){
            deref(root)->kv[1] = pair;
            return 0;// succ
        }
        qPair tmp = deref(root)->kv[0];
        deref(root)->kv[0] = pair;
        deref(root)->kv[1] = tmp;
        return 0;// succ 2
    }
    // a branch
    int issp = 0;
    int firstcmp = desc->comp(pair.key,pair.keysize,deref(root)->kv[0].key,deref(root)->kv[0].keysize);
    if(firstcmp == 0){
        SETERR(ZHWK_ERR_TREE_ELEM_EXIST);
        return -1;
    }
    if(firstcmp < 0){
        issp = qBTree__recursive_insert(desc,&(deref(root)->childs[0]),pair);
        if(issp == 0 || issp == -1){
            return issp;
        }
        qBTreeNode* topronode = qBTreeNode__constructor();
        qPair topropair = qBTreeNode__split_up(*desc,deref(root)->childs[0],topronode);
        if(deref(root)->childs[1] != NULL){
            deref(root)->extrakv = topropair;
            deref(root)->extrachild = topronode;
            return 1;
        }else{
            deref(root)->kv[1]=topropair;
            deref(root)->childs[2] = deref(root)->childs[1];
            deref(root)->childs[1] = topronode;
            return 0;
        }
    }else{
        int secondcmp = 0;
        if(deref(root)->kv[1].key != NULL){
            secondcmp = desc->comp(pair.key,pair.keysize,deref(root)->kv[1].key,deref(root)->kv[1].keysize);
        }
        if(deref(root)->kv[1].key == NULL || secondcmp < 0){
            issp = qBTree__recursive_insert(desc,&(deref(root)->childs[1]),pair);
            if(issp == 0 || issp == -1){
                return issp;
            }
            qBTreeNode* topronode = qBTreeNode__constructor();
            qPair topropair = qBTreeNode__split_up(*desc,deref(root)->childs[1],topronode);
            if(deref(root)->childs[1] != NULL){
                deref(root)->extrakv = topropair;
                deref(root)->extrachild = topronode;
                return 1;
            }else{
                deref(root)->kv[1]=topropair;
                deref(root)->childs[2] = deref(root)->childs[1];
                deref(root)->childs[1] = topronode;
                return 0;
            }
        }
        if(secondcmp == 0){
            SETERR(ZHWK_ERR_TREE_ELEM_EXIST);
            return -1;
        }else{
            issp = qBTree__recursive_insert(desc,&(deref(root)->childs[2]),pair);
            if(issp == 0 || issp == -1){
                return issp;
            }
            qBTreeNode* topronode = qBTreeNode__constructor();
            qPair topropair = qBTreeNode__split_up(*desc,deref(root)->childs[2],topronode);
            deref(root)->extrakv = topropair;
            deref(root)->extrachild = topronode;
            return 1;
        }
    }
}

int qBTree__insert(qBTreeDescriptor* desc,void* key,unsigned int keysize,void* value,unsigned int valuesize){
    qPair topair;
    topair.key = malloc(keysize);
    if(topair.key == NULL){
        SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
        return -1;
    }
    memcpy(topair.key,key,keysize);
    topair.keysize = keysize;
    topair.value = malloc(valuesize);
    if(topair.value == NULL){
        free(topair.key);
        SETERR(ZHWK_ERR_MM_ALLOC_FAIL);
        return -1;
    }
    memcpy(topair.value,value,valuesize);
    topair.valuesize = valuesize;
    int stat = qBTree__recursive_insert(desc,&(desc->root),topair);
    if(stat == 0 || stat == -1){
        desc->size+=(stat+1);
        return stat;
    }
    qBTreeNode* newrootchild = qBTreeNode__constructor();
    if(newrootchild == NULL){
        return -1;
    }
    qPair newrootpair = qBTreeNode__split_up(*desc,desc->root,newrootchild);
    qBTreeNode* newroot = qBTreeNode__constructor();
    if(newroot == NULL){
        return -1;
    }
    newroot->kv[0]=newrootpair;
    newroot->childs[0]=newrootchild;
    newroot->childs[1]=desc->root;
    desc->root = newroot;
    desc->size += 1;
    return 0;
}

// which: the index of child need to be balanced
int qBTree__rotate_left(qBTreeDescriptor* desc, qBTreeNode* root,int which){
    if(which == 2){
        return -1;
    }
    if(root->childs[which+1] == NULL || root->childs[which+1]->kv[1].key == NULL){
        // right sibling not exist or don't have spare pair
        return -1;
    }
    root->childs[which]->kv[0] = root->childs[which+1]->kv[1];
    ZEROINIT(root->childs[which+1]->kv[1]);
    return 0;
}
int qBTree__rotate_right(qBTreeDescriptor* desc, qBTreeNode* root,int which){
    if(which == 0){
        return -1;
    }
    if(root->childs[which-1] == NULL || root->childs[which-1]->kv[1].key == NULL){
        // right sibling not exist or don't have spare pair
        return -1;
    }
    root->childs[which]->kv[0] = root->childs[which-1]->kv[1];
    ZEROINIT(root->childs[which-1]->kv[1]);
    return 0;
}
// child zero is remain child needed to be merge to upper layer
int qBTree__merge(qBTreeDescriptor* desc,qBTreeNode* root,int which){
    switch(which){
        case 0:{
            root->childs[0]->kv[0]=root->kv[0];
            root->childs[0]->kv[1]=root->childs[1]->kv[0];
            root->childs[0]->childs[1]=root->childs[1]->childs[0];
            root->childs[0]->childs[2]=root->childs[1]->childs[1];
            free(root->childs[1]);
            root->kv[0]=root->kv[1];
            root->childs[1]=root->childs[2];
            ZEROINIT(root->kv[1]);
            root->childs[2]=NULL;
            if(root->kv[0].key == NULL) return 1; // need balance
            return 0; 
        }break;
        case 1:{
            root->childs[0]->kv[1] = root->kv[0];
            root->kv[0]=root->kv[1];
            ZEROINIT(root->kv[1]);
            if(root->childs[0]->childs[1] == NULL){
                root->childs[0]->childs[1] = root->childs[1]->childs[0];
            }else{
                root->childs[0]->childs[2] = root->childs[1]->childs[0];
            }
            free(root->childs[1]);
            root->childs[1] = NULL;
            if(root->kv[0].key == NULL){
                return 1;
            }
            return 0;
        }break;
        case 2:{
            root->childs[1]->kv[1] = root->kv[1];
            ZEROINIT(root->kv[1]);
            if(root->childs[1]->childs[1] == NULL){
                root->childs[1]->childs[1] = root->childs[2]->childs[0];
            }else{
                root->childs[1]->childs[2] = root->childs[2]->childs[0];
            }
            free(root->childs[2]);
            root->childs[2]=NULL;
            return 0;
        }break;
        default:return -1;// it can't be!
    }
}

// FLAG_FREE: will this operation free the value deleted?
int qBTree__recursive_erase_min(qBTreeDescriptor* desc,qBTreeNode* root,int FLAG_FREE){
    if(root == NULL){
        return -1;
    }
    if(root->childs[0] != NULL){
        int status = qBTree__recursive_erase_min(desc,root->childs[0],FLAG_FREE);
        // post work
        if(status == 0 || status == -1){
            return status;
        }
        if(qBTree__rotate_left(desc,root,0) == 0) return 0;
        return qBTree__merge(desc,root,0);
    }
    // this is the minimum
    if(root->kv[1].key != NULL){
        if(FLAG_FREE){
            free(root->kv[0].key);
            free(root->kv[0].value);
        }
        root->kv[0] = root->kv[1];
        ZEROINIT(root->kv[1]);
        return 0;
    }
    // need balance
    if(FLAG_FREE){
        free(root->kv[0].key);
        free(root->kv[0].value);
    }
    root->kv[0].key = NULL;
    root->kv[0].value = NULL;
    return 1;
}

int qBTree__get_min(qBTreeDescriptor* desc,qBTreeNode* root,qPair* mvpair,int FLAG_FREE_ORIG){
    if(root == NULL) return -1;
    if(root->childs[0] != NULL) return qBTree__get_min(desc,root,mvpair,FLAG_FREE_ORIG);
    if(FLAG_FREE_ORIG){
        free(mvpair->key);
        free(mvpair->value);
    }
    *mvpair = root->kv[0];
    return 0;
}

int qBTree__recursive_erase(qBTreeDescriptor* desc,qBTreeNode* root,qPair target){
    if(root == NULL){
        SETERR(ZHWK_ERR_TREE_ELEM_NOTEXIST);
        return -1;
    }
    int firstcmp = desc->comp(target.key,target.keysize,root->kv[0].key,root->kv[0].keysize);
    if(firstcmp == 0){
        // trigger erase
        int gmstat = qBTree__get_min(desc,root->childs[1],PTR(root->kv[0]),1);
        if(gmstat == -1){
            // here it is...
            return qBTree__recursive_erase_min(desc,root,1);
        }
        int erasestat = qBTree__recursive_erase_min(desc,root->childs[1],0);
        if(erasestat == 0 || erasestat == -1){
            return erasestat;
        }
        if(qBTree__rotate_left(desc,root,1) == 0 || qBTree__rotate_right(desc,root,1) == 0){
            return 0;
        }
        return qBTree__merge(desc,root,1);
    }
    if(firstcmp < 0){
        int status = qBTree__recursive_erase(desc,root->childs[0],target);
        // post work I
        if(status == 0 || status == -1){
            return status;
        }
        if(!qBTree__rotate_left(desc,root,0)) return 0;
        return qBTree__merge(desc,root,0);
    }
    if(root->childs[2] == NULL){
        int status = qBTree__recursive_erase(desc,root->childs[1],target);
        // post work II
        if(status == 0 || status == -1) return status;
        if(qBTree__rotate_left(desc,root,1) == 0 || qBTree__rotate_right(desc,root,1) == 0){
            return 0;
        }
        return qBTree__merge(desc,root,1);
    }
    int secondcmp = desc->comp(target.key,target.keysize,root->kv[1].key,root->kv[1].keysize);
    if(secondcmp == 0){
        // trigger erase
        int gmstat = qBTree__get_min(desc,root->childs[2],PTR(root->kv[1]),1);
        if(gmstat == -1){
            free(root->kv[1].key);
            free(root->kv[1].value);
            ZEROINIT(root->kv[1]);
            return 0;
        }
        int erasestat = qBTree__recursive_erase_min(desc,root->childs[2],0);
        if(erasestat == 0 || erasestat == 1){
            return erasestat;
        }
        if(!qBTree__rotate_right(desc,root,2)) return 0;
        return qBTree__merge(desc,root,2);
    }
    if(secondcmp < 0){
        int status = qBTree__recursive_erase(desc,root->childs[1],target);
        // post work III
        if(status == 0 || status == -1) return status;
        if(qBTree__rotate_left(desc,root,1) == 0 || qBTree__rotate_right(desc,root,1) == 0){
            return 0;
        }
        return qBTree__merge(desc,root,1);
    }
    int status = qBTree__recursive_erase(desc,root->childs[2],target);
    // post work IV
    if(status == 0 || status == 1) return status;
    if(!qBTree__rotate_right(desc,root,2)) return 0;
    return qBTree__merge(desc,root,2);
}

int qBTree__erase(qBTreeDescriptor* desc,qBTreeIterator elem){
    qPair tmpair=elem.node->kv[elem.which];
    tmpair.key = malloc(tmpair.keysize);
    memcpy(tmpair.key,elem.node->kv[elem.which].key,tmpair.keysize);
    tmpair.value = malloc(tmpair.valuesize);
    memcpy(tmpair.value,elem.node->kv[elem.which].value,tmpair.valuesize);
    int status = qBTree__recursive_erase(desc,desc->root,tmpair);
    if(status == -1) return -1;
    if(status == 1){
        qBTreeNode* tmproot = desc->root->childs[0];
        free(desc->root);
        desc->root = tmproot;
    }
    desc->size--;
    free(tmpair.key);free(tmpair.value);
    return 0;
}

