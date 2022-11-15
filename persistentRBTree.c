#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const char RED = 'R';
const char BLACK = 'B';
const int BALANCE_STATE_PRBTREE = 1;
const int ROOT_STATE_PRBTREE = 2;
const int PUSH_STATE_PRBTREE = 3;
const int ROTATION_STATE_PRBTREE = 4;

const int DERECT_DELETE_PRBTREE = 1;
const int REBALANCE_PUSH_PRBTREE = 2;
const int BALANCE_SELF_PRBTREE = 3;

const int TREE_PARENT_VERSION_SIZE = 10;
const int TREE_ROOT_VERSION_SIZE = 1000;

const int PUSH_FINISH_STATE_ONE = 1;
const int PUSH_FINISH_STATE_TWO = 2;
const int CONTINUE_PUSH_STATE = 3;

//队列
typedef struct TreeNodePRB{
    void *data;
    char color;
    struct TreeParentPRBTree *parent;
    struct TreeNodePRB *left;
    struct TreeNodePRB *right;
}TreeNodePRB;

typedef struct TreeRootPRBTree{
    TreeNodePRB * root;
    char *versionInfo;
    int version;
}TreeRootPRBTree;

typedef struct TreeParentPRBTree{
    int *versions;
    TreeNodePRB ** parents;
    int size;
    int currentSize;
}TreeParentPRBTree;


typedef struct PersistentTreeRB{
    TreeRootPRBTree ** roots;
    int currentVersion;
    int versionSize;
}PersistentTreeRB;
//添加元素到红黑树中
void insertElementPRBTree(PersistentTreeRB *tree , void *data ,int (*compareFunc) (void *x , void *y) );
void showTreeRB(PersistentTreeRB *tree);
//创建具有版本数据额红黑树
PersistentTreeRB *createPersistentTreeRB();
//查找数据是否存在
void *findElementPRBTree(PersistentTreeRB *tree , void *data , int version , int (*compareFunc) (void *x , void *y) );

TreeNodePRB *create_tree_node_PRBTree(void *data,char color){
    TreeNodePRB *newNode = (TreeNodePRB *)malloc(sizeof(TreeNodePRB));
    newNode->data = data;
    newNode->color = color;
    newNode->left = newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

TreeRootPRBTree * create_tree_root_with_data_PRBTree(int version ,char * versionInfo , void * data){
    TreeRootPRBTree * treeRoot = (TreeRootPRBTree *)malloc(sizeof(TreeRootPRBTree));
    treeRoot->root = create_tree_node_PRBTree(data , BLACK);
    treeRoot ->versionInfo = versionInfo;
    treeRoot->version = version;
    return treeRoot;
}

TreeRootPRBTree * create_tree_root_with_node_PRBTree(int version ,char * versionInfo ,TreeNodePRB * node){
    TreeRootPRBTree * treeRoot = (TreeRootPRBTree *)malloc(sizeof(TreeRootPRBTree));
    treeRoot->root = node;
    treeRoot ->versionInfo = versionInfo;
    treeRoot->version = version;
    return treeRoot;
}

TreeNodePRB * get_root_node_by_vertion_PRBTree(PersistentTreeRB *tree, int version){
    if(tree->currentVersion < version){
        printf("version is not exist");
        return NULL;
    }
    return tree->roots[version - 1]->root;
}

TreeParentPRBTree *create_tree_parent_PRBTree(TreeNodePRB * parentNode , int version){
    TreeParentPRBTree * rsl = (TreeParentPRBTree*)malloc(sizeof(TreeParentPRBTree));
    rsl->versions = (int *)malloc(sizeof(int) * TREE_PARENT_VERSION_SIZE);
    rsl->size = TREE_PARENT_VERSION_SIZE;
    rsl->versions[0] = version;
    rsl->parents = (TreeNodePRB **)malloc(sizeof(TreeNodePRB *) * TREE_PARENT_VERSION_SIZE);
    rsl->parents[0] = parentNode;
    rsl->currentSize = 1;
    return rsl;
}

void add_tree_parent_version_PRBTree(TreeParentPRBTree * parentRBTree ,TreeNodePRB * newParentNode , int  version){
    if(version == parentRBTree->versions[parentRBTree->currentSize - 1]){
        parentRBTree->parents[parentRBTree->currentSize - 1] = newParentNode;
        return;
    }
    if(parentRBTree->size == parentRBTree->currentSize){
        parentRBTree->size = parentRBTree->size * 2;
        parentRBTree->versions = (int *)realloc(parentRBTree->versions , sizeof(int) * parentRBTree->size);
        parentRBTree->parents = (TreeNodePRB **)realloc(parentRBTree->parents , sizeof(TreeNodePRB *) * parentRBTree->size);
    }
    parentRBTree->versions[parentRBTree->currentSize] = version;
    parentRBTree->parents[parentRBTree->currentSize] = newParentNode;
    parentRBTree->currentSize ++;
}

TreeNodePRB* get_parent_node_PRBTree(int versionIndex , TreeParentPRBTree *pNode){
    if(pNode ==  NULL){
        return NULL;
    }
    int index = 0;
    while(index < pNode->currentSize && pNode->versions[index] <= versionIndex){
        index++;
    }
    return pNode->parents[index-1];
}

void show_tree_node_PRBTree(TreeNodePRB *node , int version , void (*showFunc) (void *data)){
    if(node == NULL){
        return;
    }
    printf("{");
    showFunc(node->data);
    printf("  %c)  :", node->color);
    if(node->left != NULL){
        printf("L : (");
        showFunc(node->left->data);
        printf(" %c) " , node->left->color);
    }
    if(node->right != NULL){
        printf("R : (");
        showFunc(node->right->data);
        printf(" %c) ;" , node->right->color);
    }
    if(node->parent != NULL){
        TreeNodePRB *pNode = get_parent_node_PRBTree(version , node->parent);
        printf("P : (");
        showFunc(pNode->data);
        printf(" %c) ;" , pNode->color);
    }
    printf("\n");
    show_tree_node_PRBTree(node->left , version , showFunc);
    show_tree_node_PRBTree(node->right , version , showFunc);
}

void showTreePRBTree(PersistentTreeRB *tree , int version, void (*showFunc) (void *data)){
    TreeNodePRB *root = get_root_node_by_vertion_PRBTree(tree, version);
    if(root == NULL){
        printf("empty tree");
    }
    show_tree_node_PRBTree(root , version , showFunc);
    printf("end\n");
}

PersistentTreeRB *createPersistentTreeRB(){
    PersistentTreeRB *tree = (PersistentTreeRB *)malloc(sizeof(PersistentTreeRB));
    tree->currentVersion = 0;
    tree->roots = (TreeRootPRBTree **)malloc(sizeof(TreeRootPRBTree *) * TREE_ROOT_VERSION_SIZE);
    tree->versionSize = TREE_ROOT_VERSION_SIZE;
    return tree;
}

int validRed_PRBTree(TreeNodePRB *node){
    return node->color == RED;
}

int get_inserted_node_state_PRBTree(TreeNodePRB *node , int version){
    if(node->parent == NULL){
        return ROOT_STATE_PRBTREE;
    }
    TreeNodePRB * pNode = get_parent_node_PRBTree(version , node->parent);
    if(!validRed_PRBTree(pNode)){
        return BALANCE_STATE_PRBTREE;
    }else{
        TreeNodePRB *gNode = get_parent_node_PRBTree(version , pNode->parent);
        TreeNodePRB *uNode = NULL;
        if(gNode->left == pNode){
            uNode = gNode->right;
        }else{
            uNode = gNode->left;
        }
        if(uNode != NULL && validRed_PRBTree(uNode)){
            return PUSH_STATE_PRBTREE;
        }else{
            return ROTATION_STATE_PRBTREE;
        }
    }
}

void gen_node_child_relation_PRBTree(TreeNodePRB *pNode , TreeNodePRB *childNode , int leftOrRight,int version){
    if(leftOrRight){
        pNode->left = childNode;
        if(childNode != NULL){
            if(childNode->parent == NULL){
                childNode->parent = create_tree_parent_PRBTree(pNode , version);
            }else{
                add_tree_parent_version_PRBTree(childNode->parent, pNode , version);
            }
        }
    }else{
        pNode->right = childNode;
        if(childNode != NULL){
            if(childNode->parent == NULL){
                childNode->parent = create_tree_parent_PRBTree(pNode , version);
            }else{
                add_tree_parent_version_PRBTree(childNode->parent, pNode , version);
            }
        }
    }
}

void generate_three_node_basic_branch_PRBTree(TreeNodePRB *nodeSmall , TreeNodePRB * nodeMiddle , TreeNodePRB * nodeBig , int version){
    gen_node_child_relation_PRBTree(nodeMiddle , nodeSmall , 1 , version);
    gen_node_child_relation_PRBTree(nodeMiddle , nodeBig , 0 , version);
    nodeBig->color = RED;
    nodeMiddle->color = BLACK;
    nodeSmall->color = RED;
}

void clone_node_parent_relation_PRBTree(TreeNodePRB *targetNode , TreeNodePRB *sourceNode ,int version){
    if(sourceNode->parent == NULL){
        targetNode->parent = NULL;
        return;
    }else{
        targetNode->parent = create_tree_parent_PRBTree(get_parent_node_PRBTree(version, sourceNode->parent) , version);
    }
}

TreeNodePRB * clone_tree_node_with_new_version_modify_two_child_PRBTree(TreeNodePRB * node ,TreeNodePRB * childNode1 ,TreeNodePRB * childNode2,int version, int (*compareFunc) (void *data1 , void *data2)){
    TreeNodePRB *newNode = create_tree_node_PRBTree(node->data , node->color);
    clone_node_parent_relation_PRBTree(newNode, node , version);
    if(compareFunc(childNode1->data , node->data) < 0){
        gen_node_child_relation_PRBTree(newNode, childNode1 , 1 , version);
        gen_node_child_relation_PRBTree(newNode, childNode2 , 0 , version);
    }else{
        gen_node_child_relation_PRBTree(newNode, childNode1 , 0 , version);
        gen_node_child_relation_PRBTree(newNode, childNode2 , 1 , version);
    }
    return newNode;
}

TreeNodePRB * clone_tree_node_with_new_version_modify_child_PRBTree(TreeNodePRB * node ,TreeNodePRB * childNode ,int version , int (*compareFunc) (void *data1 , void *data2)){
    TreeNodePRB *newNode = create_tree_node_PRBTree(node->data , node->color);
    clone_node_parent_relation_PRBTree(newNode, node , version);
    if(compareFunc(newNode->data , childNode->data) > 0){
        gen_node_child_relation_PRBTree(newNode, childNode , 1 , version);
        gen_node_child_relation_PRBTree(newNode, node->right, 0, version);
    }else{
        gen_node_child_relation_PRBTree(newNode, childNode , 0 , version);
        gen_node_child_relation_PRBTree(newNode, node->left, 1, version);
    }
    return newNode;
}

TreeNodePRB * clone_tree_node_with_new_version_PRBTree(TreeNodePRB * node , int version){
    TreeNodePRB *newNode = create_tree_node_PRBTree(node->data , node->color);
    clone_node_parent_relation_PRBTree(newNode, node , version);
    gen_node_child_relation_PRBTree(newNode, node->left , 1 , version);
    gen_node_child_relation_PRBTree(newNode, node->right, 0, version);
    return newNode;
}

TreeNodePRB *make_version_persistent_PRBTree(TreeNodePRB *node , int version , int (*compareFunc) (void *data1 , void *data2)){
    if(node ->parent == NULL){
        return node;
    }
    TreeNodePRB *pNode = get_parent_node_PRBTree(version , node->parent);
    TreeNodePRB *rsl = clone_tree_node_with_new_version_modify_child_PRBTree(pNode , node , version , compareFunc);
    return make_version_persistent_PRBTree(rsl , version , compareFunc);
}

TreeNodePRB *balance_keep_history_PRBTree(TreeNodePRB *childNode ,int version , int (* compareFun)(void * , void *)){
    int state = get_inserted_node_state_PRBTree(childNode , version);
    //printf(" state : %d " , state);
    if(state == BALANCE_STATE_PRBTREE){
        return make_version_persistent_PRBTree(childNode , version , compareFun);
    }else if(state == ROTATION_STATE_PRBTREE){
        TreeNodePRB * pNode = get_parent_node_PRBTree(version, childNode->parent);
        TreeNodePRB * gNode = get_parent_node_PRBTree(version, pNode->parent);
        TreeNodePRB * pNodeCopy = create_tree_node_PRBTree(pNode->data , pNode->color);
        TreeNodePRB * gNodeCopy = create_tree_node_PRBTree(gNode->data , gNode->color);
        TreeNodePRB * persistentTreeRB = NULL;
        if(compareFun(pNode->data,gNode->data) < 0){
            if(compareFun(childNode->data,pNode->data) < 0){
                clone_node_parent_relation_PRBTree(pNodeCopy,gNode,version);
                gen_node_child_relation_PRBTree(gNodeCopy,gNode->right,0,version);
                gen_node_child_relation_PRBTree(gNodeCopy, pNode->right,1,version);
                generate_three_node_basic_branch_PRBTree(childNode,pNodeCopy,gNodeCopy , version);
                persistentTreeRB = pNodeCopy;
            }else{
                clone_node_parent_relation_PRBTree(childNode,gNode,version);
                gen_node_child_relation_PRBTree(gNodeCopy,childNode->right,1,version);
                gen_node_child_relation_PRBTree(gNodeCopy,gNode->right,0,version);
                gen_node_child_relation_PRBTree(pNodeCopy,childNode->left,0,version);
                gen_node_child_relation_PRBTree(pNodeCopy,pNode->left,1,version);
                generate_three_node_basic_branch_PRBTree(pNodeCopy,childNode,gNodeCopy,version);
                
                persistentTreeRB = childNode;
            }
        }else{
            if(compareFun(childNode->data,pNode->data) < 0){
                clone_node_parent_relation_PRBTree(childNode,gNode,version);
                gen_node_child_relation_PRBTree(gNodeCopy,childNode->left,0,version);
                gen_node_child_relation_PRBTree(gNodeCopy,gNode->left,1,version);
                gen_node_child_relation_PRBTree(pNodeCopy,childNode->right,1,version);
                gen_node_child_relation_PRBTree(pNodeCopy,pNode->right,0,version);
                generate_three_node_basic_branch_PRBTree(gNodeCopy,childNode,pNodeCopy , version);
                persistentTreeRB = childNode;
            }else{
                clone_node_parent_relation_PRBTree(pNodeCopy,gNode,version);
                gen_node_child_relation_PRBTree(gNodeCopy,gNode->left,1,version);
                gen_node_child_relation_PRBTree(gNodeCopy,pNode->left,0,version);
                generate_three_node_basic_branch_PRBTree(gNodeCopy,pNodeCopy,childNode, version);
                persistentTreeRB = pNodeCopy;
            }
        }
        return make_version_persistent_PRBTree(persistentTreeRB , version ,compareFun);
    }else if(state == PUSH_STATE_PRBTREE){
        TreeNodePRB * pNode = get_parent_node_PRBTree(version, childNode->parent);
        TreeNodePRB * pNodeCopy = clone_tree_node_with_new_version_modify_child_PRBTree(pNode , childNode , version,compareFun);
        TreeNodePRB * gNode = get_parent_node_PRBTree(version, pNodeCopy->parent);
        TreeNodePRB * uPNode = pNode == gNode->left ? gNode->right :gNode->left;
        TreeNodePRB * uPNodeCopy = clone_tree_node_with_new_version_PRBTree(uPNode , version);
        TreeNodePRB * gNodeCopy = clone_tree_node_with_new_version_modify_two_child_PRBTree(gNode , pNodeCopy , uPNodeCopy , version,compareFun);
        gNodeCopy->left->color = gNodeCopy->right->color = BLACK;
        if(gNodeCopy->parent == NULL){
            return gNodeCopy;
        }else{
            gNodeCopy->color = RED;
            balance_keep_history_PRBTree(gNodeCopy,version,compareFun);
        } 
    }else if(state == ROOT_STATE_PRBTREE){
        return childNode;
    }
}

int insert_node_to_tree_PRBTree(TreeNodePRB *parent, TreeNodePRB *node , int (*compareFunc) (void *x , void *y) , int version){
    int compareValue;
    TreeNodePRB *tempNode = parent;
    while(tempNode != NULL){
        compareValue = (*compareFunc)(node->data,tempNode->data);
        if(compareValue < 0){
            if(tempNode->left == NULL){
                node->parent = create_tree_parent_PRBTree(tempNode,version);
                return 1;
            }else{
                tempNode = tempNode->left;
            }
        }else if(compareValue > 0){
            if(tempNode ->right == NULL){
                node->parent = create_tree_parent_PRBTree(tempNode,version);
                return 1;
            }else{
                tempNode = tempNode->right;
            }
        }else{
            printf("insert error , node value exist\n");
            return  0;
        }
    }
}

void add_new_vertion_root_with_data_PRBTree(PersistentTreeRB * tree , void * data , char * versionInfo){ 
    TreeRootPRBTree * rootTree = create_tree_root_with_data_PRBTree(tree->currentVersion + 1 , versionInfo , data);
    tree->roots[tree->currentVersion] = rootTree;
    tree->currentVersion ++;
}

void add_new_version_root_with_node_PRBTree(PersistentTreeRB * tree , TreeNodePRB * root , char * versioninfo){
    TreeRootPRBTree * rootTree = create_tree_root_with_node_PRBTree(tree->currentVersion + 1 , versioninfo , root);
    tree->roots[tree->currentVersion] = rootTree;
    tree->currentVersion ++;
}

void insertElementPRBTree(PersistentTreeRB *tree , void *data ,int (*compareFunc) (void *x , void *y) ){
    if(tree == NULL){
        printf("tree is NULL\n");
        return;
    }
    if(tree->currentVersion == 0){
        add_new_vertion_root_with_data_PRBTree(tree , data , "first version");
    }else{
        TreeNodePRB *node = get_root_node_by_vertion_PRBTree(tree,tree->currentVersion);
        if(node == NULL){
            add_new_vertion_root_with_data_PRBTree(tree , data , "add data");
        }
        TreeNodePRB *toBeInsertNode = create_tree_node_PRBTree(data , RED);
        int newVersion = tree->currentVersion + 1;
        int insertFlag = insert_node_to_tree_PRBTree(node , toBeInsertNode ,compareFunc , newVersion);
        if(insertFlag){
            TreeNodePRB *newRoot = balance_keep_history_PRBTree(toBeInsertNode , newVersion , compareFunc);
            add_new_version_root_with_node_PRBTree(tree , newRoot , "add data");
        }else{
            free(toBeInsertNode);
        }
    }    
}

TreeNodePRB * find_node_from_node_PRBTree(TreeNodePRB *node , void *data , int (*compareFun)(void * , void *)){
    if(node == NULL){
        printf("not find ,node is NULL\n");
        return NULL;
    }
    TreeNodePRB *cur = node;
    while(cur != NULL){
        int compareValue = compareFun(data , cur->data);
        //printf("comparenvalue : %d ,result %d \n",*(int*)cur->data , compareValue);
        if(compareValue == 0){
            return cur;
        }
        if(compareValue < 0){
            cur = cur->left;
        }else{
            cur = cur->right;
        }
    }
    return cur;
}

TreeNodePRB * find_node_in_tree_PRBTree(void *data , PersistentTreeRB * tree , int (*compareFun)(void * , void *) , int version){
    if(tree == NULL || tree->currentVersion == 0){
        printf("tree is NULL\n");
        return NULL;
    }
    TreeNodePRB * root = get_root_node_by_vertion_PRBTree(tree , version);
    if(root == NULL){
        return NULL;
    }
    return find_node_from_node_PRBTree(root,data , compareFun);
}

void * findElementPRBTree(PersistentTreeRB *tree , void *data , int version ,  int (*compareFunc) (void *x , void *y)){
    if(tree == NULL || tree->currentVersion == 0){
        printf("tree is NULL\n");
        return NULL;
    }
    TreeNodePRB *node = find_node_in_tree_PRBTree(data,tree,compareFunc ,version);
    if(node != NULL){
        return node->data;
    }
    return NULL;
}

TreeNodePRB * get_max_value_node_in_node_PRBTree(TreeNodePRB * node){
    if(node == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    TreeNodePRB * rsl = node;
    while(rsl->right != NULL){
        rsl = rsl->right;
    }
    return rsl;
}

TreeNodePRB * get_min_value_node_in_node_PRBTree(TreeNodePRB * node){
    if(node == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    TreeNodePRB * rsl = node;
    while(rsl->left != NULL){
        rsl = rsl->left;
    }
    return rsl;
}

int valid_min_node_PRBTree(TreeNodePRB * node){
    if(node == NULL){
        printf("node is NULL\n");
        return 0;
    }
    if(node->left == NULL || node->right == NULL ){
        return 1;
    }
    /*TreeNodeRB *left = node->left;
    TreeNodeRB *right = node->right;
    if(left->left == NULL || left->right == NULL){
        return 1;
    }
    if(right->left == NULL || right->right == NULL){
        return 1;
    }*/
    return 0;
}

int get_min_node_delate_state_PRBTree(TreeNodePRB * node){
    if(!validRed_PRBTree(node)){
        if(node->left == NULL && node->right == NULL){
            return REBALANCE_PUSH_PRBTREE;
        }else{
            return BALANCE_SELF_PRBTREE;
        }
    }
    if(validRed_PRBTree(node)){ 
        return DERECT_DELETE_PRBTREE;
    }
}

TreeNodePRB *direct_delete_min_node_PRBTree(TreeNodePRB *node , int version , int (*compareFunc)(void * , void *)){
    if(node == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    if(node->parent == NULL){
        printf("error root is red\n");
        return NULL;
    }
    TreeNodePRB *parent = get_parent_node_PRBTree(version ,node->parent);
    TreeNodePRB * parentCopy = create_tree_node_PRBTree(parent->data , parent->color);
    if(parent->left == node){
        gen_node_child_relation_PRBTree(parentCopy , parent->right , 0 , version);
    }else{
        gen_node_child_relation_PRBTree(parentCopy , parent->left , 1 , version);
    }
    clone_node_parent_relation_PRBTree(parentCopy , parent , version);
    return make_version_persistent_PRBTree(parentCopy , version , compareFunc);
}

TreeNodePRB *delete_min_node_self_balance_PRBTree(TreeNodePRB *node , int version , int (*compareFunc)(void * , void *)){
    if(node == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    TreeNodePRB * child = node->left == NULL ? node->right : node->left;
    TreeNodePRB * childCopy = create_tree_node_PRBTree(child->data , BLACK);
    if(node->parent == NULL){
        return childCopy;
    }
    TreeNodePRB * pNode = get_parent_node_PRBTree(version , node->parent);
    TreeNodePRB * pCopy = create_tree_node_PRBTree(pNode->data , pNode->color);
    clone_node_parent_relation_PRBTree(pCopy , pNode , version);
    if(pNode->left == node){
        gen_node_child_relation_PRBTree(pCopy , pNode->right , 0 , version);
        gen_node_child_relation_PRBTree(pCopy , childCopy , 1 , version);
    }else{
        gen_node_child_relation_PRBTree(pCopy , pNode->left , 1 , version);
        gen_node_child_relation_PRBTree(pCopy , childCopy , 0 , version);
    }
    return make_version_persistent_PRBTree(pCopy , version ,compareFunc);
}

TreeNodePRB *combind_node_PRBTree(TreeNodePRB *toBeCombindTree , TreeNodePRB *combindTree ,  int (* compareFunc)(void * , void *) , int version){
    if(toBeCombindTree == NULL || combindTree == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    TreeNodePRB *toBeCombindTreeCopy = create_tree_node_PRBTree(toBeCombindTree->data , toBeCombindTree->color);
    TreeNodePRB *rsl = NULL;
    if(compareFunc(toBeCombindTree->data , combindTree->data) < 0){
        if(toBeCombindTree->right == NULL ||  !validRed_PRBTree(toBeCombindTree->right)){
            clone_node_parent_relation_PRBTree(toBeCombindTreeCopy , combindTree , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , combindTree , 0 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , toBeCombindTree->left , 1 , version);
            gen_node_child_relation_PRBTree(combindTree , toBeCombindTree->right , 1 , version);
            rsl = toBeCombindTreeCopy;
        }else if(toBeCombindTree->left == NULL || !validRed_PRBTree(toBeCombindTree->left)){
            TreeNodePRB * rChildCopy = create_tree_node_PRBTree(toBeCombindTree->right->data , BLACK);
            clone_node_parent_relation_PRBTree(rChildCopy , combindTree , version);
            gen_node_child_relation_PRBTree(rChildCopy , combindTree , 0 , version);
            gen_node_child_relation_PRBTree(rChildCopy , toBeCombindTreeCopy , 1 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , toBeCombindTree->left , 1 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , toBeCombindTree->right->left , 0 , version);
            gen_node_child_relation_PRBTree(combindTree , toBeCombindTree->right->right , 1 , version);
            toBeCombindTreeCopy->color = RED;
            rsl = rChildCopy;
        }else{
            TreeNodePRB * lChildCopy = create_tree_node_PRBTree(toBeCombindTree->left->data , BLACK);
            TreeNodePRB * rChildCopy = create_tree_node_PRBTree(toBeCombindTree->right->data , BLACK);
            clone_node_parent_relation_PRBTree(toBeCombindTreeCopy , combindTree , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , rChildCopy , 0 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , lChildCopy , 1 , version);
            gen_node_child_relation_PRBTree(lChildCopy , toBeCombindTree->left->left , 1 , version);
            gen_node_child_relation_PRBTree(lChildCopy , toBeCombindTree->left->right , 0 , version);
            gen_node_child_relation_PRBTree(rChildCopy ,  combindTree, 0 , version);
            gen_node_child_relation_PRBTree(rChildCopy , toBeCombindTree->right->left , 1 , version);
            gen_node_child_relation_PRBTree(combindTree , toBeCombindTree->right->right , 1 , version);
            toBeCombindTreeCopy->color = RED;
            rsl = toBeCombindTreeCopy;
        }
    }else{
        if(toBeCombindTree->left == NULL ||  !validRed_PRBTree(toBeCombindTree->left)){
            clone_node_parent_relation_PRBTree(toBeCombindTreeCopy , combindTree , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , combindTree , 1 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , toBeCombindTree->right , 0 , version);
            gen_node_child_relation_PRBTree(combindTree , toBeCombindTree->left , 0 , version);
            rsl = toBeCombindTreeCopy;
        }else if(toBeCombindTree->right == NULL || !validRed_PRBTree(toBeCombindTree->right)){
            TreeNodePRB * lChildCopy = create_tree_node_PRBTree(toBeCombindTree->left->data , BLACK);
            clone_node_parent_relation_PRBTree(lChildCopy , combindTree , version);
            gen_node_child_relation_PRBTree(lChildCopy , combindTree , 1 , version);
            gen_node_child_relation_PRBTree(lChildCopy , toBeCombindTreeCopy , 0 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , toBeCombindTree->right , 0 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , toBeCombindTree->left->right , 1 , version);
            gen_node_child_relation_PRBTree(combindTree , toBeCombindTree->left->left , 0 , version);
            toBeCombindTreeCopy->color = RED;
            rsl = lChildCopy;
        }else{
            TreeNodePRB * lChildCopy = create_tree_node_PRBTree(toBeCombindTree->left->data , BLACK);
            TreeNodePRB * rChildCopy = create_tree_node_PRBTree(toBeCombindTree->right->data , BLACK);
            clone_node_parent_relation_PRBTree(toBeCombindTreeCopy , combindTree , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , rChildCopy , 0 , version);
            gen_node_child_relation_PRBTree(toBeCombindTreeCopy , lChildCopy , 1 , version);
            gen_node_child_relation_PRBTree(rChildCopy , toBeCombindTree->right->left , 1 , version);
            gen_node_child_relation_PRBTree(rChildCopy , toBeCombindTree->right->right , 0 , version);
            gen_node_child_relation_PRBTree(lChildCopy ,  combindTree, 1 , version);
            gen_node_child_relation_PRBTree(lChildCopy , toBeCombindTree->left->right , 0 , version);
            gen_node_child_relation_PRBTree(combindTree , toBeCombindTree->left->left , 0 , version);
            toBeCombindTreeCopy->color = RED;
            rsl = toBeCombindTreeCopy;
        }
    }
    return rsl;
}

int get_push_state_PRBTree(TreeNodePRB *node , TreeNodePRB *nodeChild){
    if(validRed_PRBTree(node)){
        return PUSH_FINISH_STATE_ONE;
    }else{
        if(validRed_PRBTree(nodeChild)){
            return PUSH_FINISH_STATE_TWO;
        }else{
            return CONTINUE_PUSH_STATE;
        }
    }
}

TreeNodePRB *delete_min_node_push_balance_PRBTree(TreeNodePRB *pNode , TreeNodePRB *brother ,int version , int (*compareFunc)(void * , void *)){;
    int pushState = get_push_state_PRBTree(pNode , brother);
    printf("pushState: %d\n",pushState);
    TreeNodePRB * returnNode = NULL;
    if(pushState == PUSH_FINISH_STATE_ONE){
        returnNode = combind_node_PRBTree(brother , pNode , compareFunc , version);
        return make_version_persistent_PRBTree(returnNode , version,compareFunc);
    }else if(pushState == PUSH_FINISH_STATE_TWO){
        pNode->color = RED;
        TreeNodePRB * brotherCopy = create_tree_node_PRBTree(brother->data , BLACK);
        clone_node_parent_relation_PRBTree(brotherCopy , pNode , version);
        if(compareFunc(brother->data , pNode->data) > 0){
            returnNode = combind_node_PRBTree(brother->right , pNode , compareFunc , version);
            gen_node_child_relation_PRBTree(brotherCopy , returnNode , 0 , version);
            gen_node_child_relation_PRBTree(brotherCopy , brother->left , 1 , version);
        }else{
            returnNode = combind_node_PRBTree(brother->left , pNode , compareFunc , version);
            gen_node_child_relation_PRBTree(brotherCopy , returnNode , 1 , version);
            gen_node_child_relation_PRBTree(brotherCopy , brother->right , 0 , version);
        }
        return make_version_persistent_PRBTree(brotherCopy , version , compareFunc);
    }else if(pushState == CONTINUE_PUSH_STATE){
        pNode->color = RED;
        returnNode = combind_node_PRBTree(brother , pNode , compareFunc , version);
        if(returnNode->parent == NULL  || validRed_PRBTree(returnNode)){
            returnNode->color = BLACK;
            return make_version_persistent_PRBTree(returnNode , version , compareFunc);
        }else{
            TreeNodePRB *gNode = get_parent_node_PRBTree(version ,returnNode->parent);
            TreeNodePRB *gNodeCopy = create_tree_node_PRBTree(gNode->data , gNode->color);
            clone_node_parent_relation_PRBTree(gNodeCopy , gNode , version);
            if(compareFunc(returnNode->data , gNode->data) < 0){
                gen_node_child_relation_PRBTree(gNodeCopy , returnNode , 1 , version);
                return delete_min_node_push_balance_PRBTree(gNodeCopy , gNode->right , version , compareFunc);
            }else{
                gen_node_child_relation_PRBTree(gNodeCopy , returnNode , 0 , version);
                return delete_min_node_push_balance_PRBTree(gNodeCopy , gNode->left , version , compareFunc);
            }
        }
    }
}

TreeNodePRB *balance_min_node_delete_keep_history_PRBTree(TreeNodePRB * node , int version , int (*compareFunc)(void * , void *)){
    if(node == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    int state = get_min_node_delate_state_PRBTree(node);
    printf("state is %d\n",state);
    if(state == DERECT_DELETE_PRBTREE){
        return direct_delete_min_node_PRBTree(node , version , compareFunc);
    }
    if(state == BALANCE_SELF_PRBTREE){
        return delete_min_node_self_balance_PRBTree(node,version , compareFunc);
    }
    if(state == REBALANCE_PUSH_PRBTREE){
        if(node->parent == NULL){
            return NULL;
        }
        TreeNodePRB *pNode = get_parent_node_PRBTree(version ,node->parent);
        TreeNodePRB *pNodeCopy = create_tree_node_PRBTree(pNode->data , pNode->color);
        clone_node_parent_relation_PRBTree(pNodeCopy , pNode , version);
        if(compareFunc(node->data , pNode->data) < 0){
            return delete_min_node_push_balance_PRBTree(pNodeCopy , pNode->right , version , compareFunc);
        }else{
            return delete_min_node_push_balance_PRBTree(pNodeCopy , pNode->left , version , compareFunc);
        }
    }
}

int deleteElementPRBTree(PersistentTreeRB *tree , void *data , int (*compareFunc)(void * , void *)){
    TreeNodePRB * node = find_node_in_tree_PRBTree(data , tree , compareFunc , tree->currentVersion);
    if(node == NULL){
        printf("not find ,delate failed\n");
        return 0;
    }
    TreeNodePRB *exChangeNode = get_max_value_node_in_node_PRBTree(node->left);  
    int newVersion = tree->currentVersion + 1;
    if(exChangeNode == NULL){
        TreeNodePRB* newNode = balance_min_node_delete_keep_history_PRBTree(node , newVersion , compareFunc);
        add_new_version_root_with_node_PRBTree(tree , newNode , "delete element"); 
        return 1;
    }else{
        TreeNodePRB *newRoot = balance_min_node_delete_keep_history_PRBTree(exChangeNode , newVersion , compareFunc);
        add_new_version_root_with_node_PRBTree(tree , newRoot , "delete element"); 
        TreeNodePRB * newNode = find_node_in_tree_PRBTree(data , tree , compareFunc , newVersion);
        newNode->data = exChangeNode->data;
        return 1; 
    }
}