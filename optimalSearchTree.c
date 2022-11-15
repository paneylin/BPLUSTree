#include<stdio.h>
#include<stdlib.h>
#include "./sort.c"
//二叉树
typedef struct TreeNodeOST
{
    void * data;
    struct TreeNodeOST *parent;
    struct TreeNodeOST *left;
    struct TreeNodeOST *right;
    int (*compareFunc)(void *data1,void *data2);
}TreeNodeOST;

typedef struct OSTTree
{
    TreeNodeOST ** nodeArray;
    int nodeSize;
    int currentNodeSize;
    int ** minPriorArray;
    int ** rootArray;
    TreeNodeOST *root;
    int (*getPriorFunc)(void *data);
}OSTTree;

int OSTTREE_INITSIZE = 100;

TreeNodeOST * create_tree_node_OST(void * data , int (*compareFunc)(void *data1,void *data2)){
    TreeNodeOST * node = (TreeNodeOST *)malloc(sizeof(TreeNodeOST));
    node->data = data;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->compareFunc = compareFunc;
    return node;
}

OSTTree *create_OSTTree_OST(void ** data , int size , int (*getPriorFunc)(void *data) , int (*compareFunc)(void *data1,void *data2)){
    OSTTree * rsl = (OSTTree*)malloc(sizeof(OSTTree));
    rsl->nodeArray = (TreeNodeOST**)malloc(sizeof(TreeNodeOST*) * OSTTREE_INITSIZE);
    rsl->nodeSize = OSTTREE_INITSIZE;
    rsl->currentNodeSize = size;
    rsl->minPriorArray = (int**)malloc(sizeof(int*) * OSTTREE_INITSIZE);
    rsl->getPriorFunc = getPriorFunc;
    for(int i = 0 ; i < OSTTREE_INITSIZE ; i ++){
        rsl->minPriorArray[i] = (int *)malloc(sizeof(int) * OSTTREE_INITSIZE);
    }
    rsl->rootArray = (int**)malloc(sizeof(int*) * OSTTREE_INITSIZE * OSTTREE_INITSIZE);
    for(int i = 0 ; i < OSTTREE_INITSIZE ; i ++){
        rsl->rootArray[i] = (int *)malloc(sizeof(int) * OSTTREE_INITSIZE);
    }
    for(int i = 0 ; i < size ; i++){
        rsl->nodeArray[i] = create_tree_node_OST(data[i] , compareFunc);
        rsl->minPriorArray[i][i] = getPriorFunc(data[i]);
        rsl->rootArray[i][i] = i;
    }
    for(int i = 0 ; i < size ; i ++){
        for(int j = i + 1 ; j < size ; j ++){
                 rsl->minPriorArray[i][j] = -1;
                 rsl->minPriorArray[j][i] = 0;
                 rsl->rootArray[i][j] = -1;
        }
    }
    rsl->root = NULL;
}

void setPrior_OST(OSTTree *tree , int i , int j , int prior){
    tree->minPriorArray[i][j] = prior;
}

void setRoot_OST(OSTTree *tree , int i , int j , int root){
    tree->rootArray[i][j] = root;
}

int getPrior_OST(OSTTree *tree , int i , int j){
    return tree->minPriorArray[i][j];
}

int compare_prior_OST(int prior1 , int prior2){
    if(prior1 == -1){
        return 1;
    }
    return prior1 > prior2;
}

int getTotalPrior_OST(OSTTree *tree , int i , int j){
    int rsl = 0;
    for(int k = i ; k <= j ; k ++){
        rsl += getPrior_OST(tree , k , k);
    }
    return rsl;
}

int calPrior_OST(OSTTree *tree , int i , int j){
    if(i > j){
        return 0;
    }
    if(getPrior_OST(tree , i , j) != -1){
        return getPrior_OST(tree , i , j);
    }
    for(int k = i ; k <= j ; k ++){
        int prior = calPrior_OST(tree , i , k - 1) + calPrior_OST(tree , k + 1 , j)  + getTotalPrior_OST(tree , i , j);
        if(compare_prior_OST(getPrior_OST(tree , i , j) , prior)){
            setPrior_OST(tree , i , j , prior);
            setRoot_OST(tree , i , j , k);
        }
    }
    return getPrior_OST(tree , i , j);
}

TreeNodeOST *get_tree_node_from_array_OST(OSTTree * tree , int startIndex , int endIndex , TreeNodeOST *parent){
    if(startIndex > endIndex){
        return NULL;
    }
    TreeNodeOST * rsl = tree->nodeArray[tree->rootArray[startIndex][endIndex]];
    rsl->parent = parent;
    rsl->left = get_tree_node_from_array_OST(tree , startIndex , tree->rootArray[startIndex][endIndex] - 1 , rsl);
    rsl->right = get_tree_node_from_array_OST(tree , tree->rootArray[startIndex][endIndex] + 1 , endIndex , rsl);
    return rsl;
}

void build_OSTTree_OST(OSTTree * tree){
    tree->root = tree->nodeArray[tree->rootArray[0][tree->currentNodeSize - 1]];
    tree->root->parent = NULL;
    tree->root->left = get_tree_node_from_array_OST(tree , 0 , tree->rootArray[0][tree->currentNodeSize - 1] - 1 , tree->root);
    tree->root->right = get_tree_node_from_array_OST(tree , tree->rootArray[0][tree->currentNodeSize - 1] + 1 , tree->currentNodeSize - 1 , tree->root);
}

OSTTree *createOSTTree(void **data , int dataSize ,int (*getPriorFunc)(void*) , int (*conpareFunc)(void * , void *)){
    quickSort(data , dataSize , 1 , conpareFunc);
    OSTTree * tree = create_OSTTree_OST(data , dataSize , getPriorFunc ,conpareFunc);
    for(int i = 0 ; i < dataSize ; i++){
        calPrior_OST(tree , 0 , i);
    }
    build_OSTTree_OST(tree);
    return tree;
}

int compareNode_OST(TreeNodeOST *node1 , TreeNodeOST *node2){
    return node1->compareFunc(node1->data , node2->data);
}

int add_data_to_tree_OST(OSTTree * tree , void *data , int (*compareFunc)(void * , void *) , int (*getPriorFunc)(void *)){
    if(tree->currentNodeSize == 0){
        tree->nodeArray[0] = create_tree_node_OST(data , compareFunc);
        tree->minPriorArray[0][0] = getPriorFunc(data);
        tree->rootArray[0][0] = 0;
        tree->root = tree->nodeArray[0];
        tree->root->parent = NULL;
        tree->root->left = NULL;
        tree->root->right = NULL;
        tree->currentNodeSize ++;
        return 0;
    }
    if(tree->currentNodeSize == tree->nodeSize){
        tree->nodeSize *= 2;
        tree->nodeArray = (TreeNodeOST**)realloc(tree->nodeArray , sizeof(TreeNodeOST*) * tree->nodeSize * 2);
        tree->minPriorArray = (int**)realloc(tree->minPriorArray , sizeof(int*) * tree->nodeSize * 2);
        tree->rootArray = (int**)realloc(tree->rootArray , sizeof(int*) * tree->nodeSize * 2);
        for(int i = 0 ; i < tree->nodeSize * 2 ; i ++){
            if(i < tree->nodeSize){
                tree->minPriorArray[i] = (int *)realloc(tree->minPriorArray[i] , sizeof(int) * tree->nodeSize * 2);
                tree->rootArray[i] = (int *)realloc(tree->rootArray[i] , sizeof(int) * tree->nodeSize * 2);
            }else{
                tree->minPriorArray[i] = (int *)malloc(sizeof(int) * tree->nodeSize * 2);
                tree->rootArray[i] = (int *)malloc(sizeof(int) * tree->nodeSize * 2);
            }
        }
        tree->nodeSize *= 2;
    }
    TreeNodeOST * node = create_tree_node_OST(data , compareFunc);
    int position = searchPrevDataBinary(tree->nodeArray , tree->currentNodeSize , 1 , node , compareNode_OST);
    memmove(tree->nodeArray + position + 2 , tree->nodeArray + position + 1 , sizeof(TreeNodeOST*) * (tree->currentNodeSize - position - 1));
    tree->nodeArray[position + 1] = node;
    tree->currentNodeSize ++;
    for(int i = tree->currentNodeSize - 1 ; i > position + 1 ; i --){
        for(int j = tree->currentNodeSize - 1 ; j >= i ; j --){
            tree->minPriorArray[i][j] = tree->minPriorArray[i - 1][j - 1];
            if(tree->rootArray[i - 1][j - 1] >= position + 1){
                tree->rootArray[i - 1][j - 1] ++;
            }
            tree->rootArray[i][j] = tree->rootArray[i - 1][j - 1];
        }
    }
    for(int i = 0 ; i <= position + 1 ; i ++){
        for(int j = position + 1 ; j < tree->currentNodeSize ; j ++){
            tree->minPriorArray[i][j] = -1;
            tree->rootArray[i][j] = -1;
        }
    }
    tree->minPriorArray[position + 1][position + 1] = getPriorFunc(data);
    tree->rootArray[position + 1][position + 1] = position + 1;
    return position + 1;
}

void insertDataOST(OSTTree *tree , void *data , int (*getPriorFunc)(void *), int (*conpareFunc)(void * , void *)){
    int position = add_data_to_tree_OST(tree , data , conpareFunc , getPriorFunc);
    for(int i =  position; i < tree->currentNodeSize ; i ++){
        calPrior_OST(tree , 0 , i);
    }
    build_OSTTree_OST(tree);
}


void showTreeDataOST(OSTTree * tree , void (*showFunc)(void *)){
    for(int i = 0 ; i <tree->currentNodeSize; i ++){
        showFunc(tree->nodeArray[i]->data);       
    }
    printf("\n");
    for(int i = 0 ; i < tree->currentNodeSize ; i ++){
        for(int j = 0 ; j < tree->currentNodeSize ; j ++){
            printf("%d " ,tree->minPriorArray[i][j]);
        }
        printf("\n");
    }
}