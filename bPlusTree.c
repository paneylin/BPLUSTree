#include "./sort.h"
#include "./CircleQueue.h"

typedef struct TreeNodeDataBPlus
{
    void * key;
    void * child;
}TreeNodeDataBPlus;

typedef struct TreeNodeLeafDataBPlus
{
    void * key;
    void * data;
}TreeNodeLeafDataBPlus;

typedef struct TreeNodeBPlus
{
    TreeNodeDataBPlus **datas;
    int size;
    struct TreeNodeBPlus *parent;
    struct TreeNodeBPlus *brother;
}TreeNodeBPlus;

typedef struct  TreeNodeLeafBPlus{
    TreeNodeLeafDataBPlus **datas;
    int size;
    TreeNodeBPlus *parent;
    struct TreeNodeLeafBPlus *brother;
}TreeNodeLeafBPlus;

typedef struct BPlusTree
{
    int maxNodeSize;
    int maxLeafSize;
    int treeHeight;
    TreeNodeBPlus *root;
    int (*compareFunc)(void * , void *);
    void * (*getKeyFunc)(void *);
    void (*freeKeyFunc)(void *);
    void (*freeDataFunc)(void *);
}BPlusTree;
//创建b+树
BPlusTree *createBPlusTreeBPlus(void * (*getKeyFunc)(void *data) , int (*compareFunc)(void * key1, void *key2) , int maxNode);
//设置B+树key释放函数
void setFreeKeyFuncBPlus(void (*freeKeyFunc)(void *key) , BPlusTree *tree);
//释放B+树数据释放函数
void setFreeDataFuncBPlus(void (*freeDataFunc)(void *data) , BPlusTree *tree);
//插入数据到B+树中
int insertElementBPlus(BPlusTree *tree, void *data);
//找到key对应的数据
void *findElementBPlus(void *key , BPlusTree *tree);
//删除key对应的数据
int deleteElementBPlus(void *Key , BPlusTree *tree);
//显示B+树结构
void showTreeBplus(BPlusTree *tree , void (*showNodeFunc)(void *key), void (*showLeafFunc)(void *key , void *data));
//销毁B+树并释放内存
void destroyBPlusTreeBPlus(BPlusTree *tree);

int compare_key_func_default_BPlus(void *Key1, void *Key2){
    return *(int *)Key1 - *(int *)Key2;
}

void * getKey_func_default_BPlus(void *data){
    return data;
}

void * getKey_nodeData_func(TreeNodeDataBPlus * data){
    return data->key;
}

void * getKey_leafData_func(TreeNodeLeafDataBPlus * data){
    return data->key;
}

void destroy_node_BPlus(TreeNodeBPlus * node , BPlusTree * tree){
    if(node == NULL){
        return;
    }
    node->brother = NULL;
    node->parent = NULL;
    for(int i = 0 ; i < node->size ; i ++){
        destroy_node_data_BPlus(node->datas[i], tree);
    }
    free(node->datas);
    node->datas = NULL;
    free(node);
    node = NULL;
}

void destroy_leaf_node_BPlus(TreeNodeLeafBPlus * leafNode , BPlusTree * tree){
    if(leafNode == NULL){
        return;
    }
    leafNode->brother = NULL;
    leafNode->parent = NULL;
    for(int i = 0 ; i < leafNode->size ; i ++){
        destroy_leaf_data_BPlus(leafNode->datas[i], tree);
    }
    free(leafNode->datas);
    leafNode->datas = NULL;
    free(leafNode);
    leafNode = NULL;
}

void destroy_node_data_BPlus(TreeNodeDataBPlus * nodeData , BPlusTree * tree){
    if(nodeData == NULL){
        return;
    }
    if(tree->freeKeyFunc != NULL){
        tree->freeKeyFunc(nodeData->key);
        nodeData->key = NULL;
    }
    nodeData->child = NULL;
    free(nodeData);
    nodeData = NULL;
}

void destroy_leaf_data_BPlus(TreeNodeLeafDataBPlus * leafData , BPlusTree * tree){
    if(leafData == NULL){
        return;
    }
    if(tree->freeKeyFunc != NULL){
        tree->freeKeyFunc(leafData->key);
        leafData->key = NULL;
    }
    if(tree->freeDataFunc != NULL){
        tree->freeDataFunc(leafData->data);
        leafData->data = NULL;
    }
    free(leafData);
    leafData = NULL;
}

void destroy_tree_BPlus(BPlusTree * tree){
    if(tree == NULL){
        return;
    }
    tree->compareFunc = tree->freeDataFunc = tree->freeKeyFunc = tree->getKeyFunc = NULL;
    tree->root = NULL;
    free(tree);
    tree = NULL;
}

void destroyBPlusTreeBPlus(BPlusTree * tree){
    if(tree == NULL){
        return;
    }
    int height = 1;
    TreeNodeBPlus * opNode = NULL;
    TreeNodeLeafBPlus * opLeafNode = NULL;
    void * nextOpNode = tree->root;
    while(height < tree->treeHeight){
        opNode = nextOpNode;
        nextOpNode = opNode->datas[0]->child;
        while(opNode != NULL){
            TreeNodeBPlus * temp = opNode->brother;
            destroy_node_BPlus(opNode, tree);
            opNode = temp;
        }
        height ++;
    }
    opLeafNode = nextOpNode;
    while(opLeafNode != NULL){
        TreeNodeLeafBPlus * temp = opLeafNode->brother;
        destroy_leaf_node_BPlus(opLeafNode, tree);
        opLeafNode = temp;
    }
    destroy_tree_BPlus(tree);
}

TreeNodeBPlus *create_tree_node_BPlus(int maxSize){
    TreeNodeBPlus *treeNode = (TreeNodeBPlus *)malloc(sizeof(TreeNodeBPlus));
    treeNode->datas = (TreeNodeDataBPlus **)malloc(sizeof(TreeNodeDataBPlus *) * maxSize);
    for(int i = 0; i < maxSize; i++){
        treeNode->datas[i] = NULL;
    }
    treeNode->size = 0;
    treeNode->parent = treeNode->brother = NULL;
    return treeNode;
}

TreeNodeLeafBPlus *create_tree_node_leaf_BPlus(int maxSize){
    TreeNodeLeafBPlus *treeNode = (TreeNodeLeafBPlus *)malloc(sizeof(TreeNodeLeafBPlus));
    treeNode->datas = (TreeNodeLeafDataBPlus **)malloc(sizeof(TreeNodeLeafDataBPlus *) * maxSize);
    for(int i = 0; i < maxSize; i++){
        treeNode->datas[i] = NULL;
    }
    treeNode->size = 0;
    treeNode->parent = treeNode->brother = NULL;
    return treeNode;
}

TreeNodeLeafDataBPlus * create_tree_node_leaf_data_BPlus(void * key ,void *data , BPlusTree * tree){
    if(data == NULL || key == NULL){
        printf("data is NULL, create_tree_node_leaf_data_BPlus error\n");
        return NULL;
    }
    if(key == NULL){
        printf("key is NULL, create_tree_node_leaf_data_BPlus error\n");
        return NULL;
    }
    TreeNodeLeafDataBPlus *treeNode = (TreeNodeLeafDataBPlus *)malloc(sizeof(TreeNodeLeafDataBPlus));
    treeNode->key = key;
    treeNode->data = data;
    return treeNode;
}

TreeNodeDataBPlus * create_tree_node_data_by_leaf_data_BPlus(TreeNodeLeafDataBPlus * leafData , TreeNodeLeafBPlus * nodeLeaf){
    if(leafData == NULL || nodeLeaf == NULL){
        printf("leafData is null, create_tree_node_data_by_leaf_data_BPlus error\n");
        return NULL;
    }
    TreeNodeDataBPlus *treeNodeData = (TreeNodeDataBPlus *)malloc(sizeof(TreeNodeDataBPlus));
    treeNodeData->key = leafData->key;
    treeNodeData->child = nodeLeaf;
    return treeNodeData;
}

TreeNodeDataBPlus * create_tree_node_data_by_node_data_BPlus(TreeNodeDataBPlus * nodeData , TreeNodeBPlus * node){
    if(nodeData == NULL || node == NULL){
        printf("nodeData is null, create_tree_node_data_by_node_data_BPlus error\n");
        return NULL;
    }
    TreeNodeDataBPlus *treeNodeData = (TreeNodeDataBPlus *)malloc(sizeof(TreeNodeDataBPlus));
    treeNodeData->key = nodeData->key;
    treeNodeData->child = node;
    return treeNodeData;
}

void split_node_BPlus(TreeNodeBPlus *node , TreeNodeDataBPlus * nodeData , BPlusTree * tree){
    if(node == NULL || nodeData == NULL){
        printf("node or nodeData is null, split_node_BPlus error\n");
        return;
    }
    int mid = (node->size + 1) / 2;
    TreeNodeBPlus *newNode = create_tree_node_BPlus(tree->maxNodeSize);
    int flag = 0;
    for(int j = node->size - 1; j >= 0; ){
        if(tree->compareFunc(node->datas[j]->key , nodeData->key) < 0  && !flag){
            newNode->datas[mid - 1 - newNode->size] = nodeData;
            ((TreeNodeBPlus *)nodeData->child)->parent = newNode;
            flag = 1;
        }else{
            newNode->datas[mid - 1 - newNode->size] = node->datas[j];
            ((TreeNodeBPlus *)node->datas[j]->child)->parent = newNode;
            node->datas[j] = NULL;
            node->size --;
            j --;
        }
        newNode->size ++;
        if(newNode->size == mid){
            break;
        }
    }
    if(!flag){
        node->datas[node->size] = nodeData;
        node->size ++;
        insertSortHasKeySort(node->datas , node->size , getKey_nodeData_func ,tree->compareFunc);
    }
    newNode->brother = node->brother;
    node->brother = newNode;
    if(node->parent == NULL){
        TreeNodeDataBPlus * pushData = create_tree_node_data_by_node_data_BPlus(node->datas[0] , node);
        push_node_data_to_node_BPlus(node->parent , pushData , tree);
    }
    newNode->parent = node->parent;
    TreeNodeDataBPlus * pushData = create_tree_node_data_by_node_data_BPlus(newNode->datas[0] , newNode);
    push_node_data_to_node_BPlus(node->parent , pushData , tree);
}

void split_leaf_node_BPlus(TreeNodeLeafBPlus *node , TreeNodeLeafDataBPlus * leafData , BPlusTree * tree){
    if(node == NULL || leafData == NULL){
        printf("node or leafData is null, split_leaf_node_BPlus error\n");
        return;
    }
    int mid = (node->size + 1) / 2;
    TreeNodeLeafBPlus *newNode = create_tree_node_leaf_BPlus(tree->maxLeafSize);
    int flag = 0;
    for(int j = node->size - 1; j >= 0; ){
        if(tree->compareFunc(node->datas[j]->key , leafData->key) < 0  && !flag){
            newNode->datas[mid - 1 - newNode->size] = leafData;
            flag = 1;
        }else{
            newNode->datas[mid - 1 - newNode->size] = node->datas[j];
            node->datas[j] = NULL;
            node->size --;
            j --;
        }
        newNode->size ++;
        if(newNode->size == mid){
            break;
        }
    }
    if(!flag){
        node->datas[node->size] = leafData;
        node->size ++;
        insertSortHasKeySort(node->datas , node->size ,getKey_leafData_func , tree->compareFunc);
    }
    newNode->brother = node->brother;
    node->brother = newNode;
    newNode->parent = node->parent;
    push_leaf_data_to_node_BPlus(node->parent , newNode->datas[0] ,newNode , tree);
}

void push_node_data_to_node_BPlus(TreeNodeBPlus * node ,TreeNodeDataBPlus * treeNodeData , BPlusTree *tree){
    if(treeNodeData == NULL){
        printf("treeNodeData is null, push_node_data_to_node_BPlus error\n");
        return;
    }
    if(node == NULL){
        node = create_tree_node_BPlus(tree->maxNodeSize);
        tree->root = node;
        ((TreeNodeBPlus *)treeNodeData->child)->parent = node;
        tree->treeHeight ++;
    }
    if(node->size == tree->maxNodeSize){
        split_node_BPlus(node , treeNodeData ,tree);
    }else{
        node->datas[node->size] = treeNodeData;
        node->size++;
        insertSortHasKeySort(node->datas , node->size , getKey_nodeData_func, tree->compareFunc);
    }
}

void push_leaf_data_to_node_BPlus(TreeNodeBPlus * node ,TreeNodeLeafDataBPlus * leafData , TreeNodeLeafBPlus * leaf , BPlusTree *tree){
    if(leafData == NULL || leaf == NULL){
        printf("leafData or leaf is null, push_leaf_data_to_node_BPlus error\n");
        return;
    }
    if(node == NULL){
        node = create_tree_node_BPlus(tree->maxNodeSize);
        tree->root = node;
        leaf->parent = node;
        tree->treeHeight = 2;
    }
    //printf("push_leaf_data_to_node_BPlus");
    TreeNodeDataBPlus *treeNodeData = create_tree_node_data_by_leaf_data_BPlus(leafData , leaf);
    push_node_data_to_node_BPlus(node , treeNodeData , tree);
}

void setFreeDataFuncBPlus(void (*freeDataFunc)(void *data) , BPlusTree *tree){
    if(tree == NULL){
        printf("tree is null, setFreeDataFuncBPlus error\n");
        return;
    }
    tree->freeDataFunc = freeDataFunc;
}

void setFreeKeyFuncBPlus(void (*freeKeyFunc)(void *key) , BPlusTree *tree){
    if(tree == NULL){
        printf("tree is null, setFreeKeyFuncBPlus error\n");
        return;
    }
    tree->freeKeyFunc = freeKeyFunc;
}

void showTreeBplus(BPlusTree *tree , void (*showNodeFunc)(void *data) , void (*showLeafFunc)(void *key, void * data)){
    if(tree->root == NULL){
        printf("empty tree\n");
        return;
    }
    printf("tree node maxSize %d , tree leaf maxSzie is %d , tree height is %d\n",tree->maxNodeSize , tree->maxLeafSize , tree->treeHeight);
    CircleQueue *queue = createCircleQueue();
    CircleQueue * queue2 = createCircleQueue();
    int height = 1;
    pushCircleQueue(tree->root , queue);
    while(!isEmptyCircleQueue(queue) || !isEmptyCircleQueue(queue2)){
        if(height != tree->treeHeight){
            TreeNodeBPlus *node = height%2 == 1 ?(TreeNodeBPlus *)popCircleQueue(queue):(TreeNodeBPlus *)popCircleQueue(queue2);
            if(node->parent != NULL){
                printf("node parent is ");
                showNodeFunc(node->parent->datas[0]->key);
            }
            if(node->brother != NULL){
                printf("node brother is ");
                showNodeFunc(node->brother->datas[0]->key);
            }
            printf(" node size is %d , node datas is \n" , node->size);
            for(int i = 0; i < node->size; i++){
                showNodeFunc(node->datas[i]->key);
                pushCircleQueue(node->datas[i]->child , height%2 == 1 ? queue2: queue);
            }
            printf("\n");
        }else{
            TreeNodeLeafBPlus *node = height%2 == 1 ?(TreeNodeLeafBPlus *)popCircleQueue(queue):(TreeNodeLeafBPlus *)popCircleQueue(queue2);
            if(node == NULL){
                printf("node is null\n");
            }
            if(node->parent != NULL){
                printf("node parent is ");
                showNodeFunc(node->parent->datas[0]->key);
            }
            if(node->brother != NULL){
                printf("node brother is ");
                showNodeFunc(node->brother->datas[0]->key);
            }
            printf(" node size is %d , node datas is \n" , node->size);
            for(int i = 0; i < node->size; i++){
                showLeafFunc(node->datas[i]->key , node->datas[i]->data);
            }
            printf("\n");
        }
        if(isEmptyCircleQueue(height%2 == 1 ? queue:queue2)){
            height ++;
            printf("height is %d\n",height);
        }
    }
    destroyCircleQueue(queue);
    destroyCircleQueue(queue2);
}

int node_full_BPlus(TreeNodeBPlus *node , BPlusTree *tree){
    if(node == NULL){
        printf("node is null, node_full_BPlus error\n");
        return 0;
    }
    return node->size == tree->maxNodeSize;
}

int node_too_less_BPlus(TreeNodeBPlus *node , BPlusTree *tree){
    if(node == NULL){
        printf("node is null, node_too_less_BPlus error\n");
        return 0;
    }
    return node->size < 2 ? 1 : node->size < tree->maxNodeSize/2;
}

int get_node_min_size_BPlus(BPlusTree *tree){
    return tree->maxNodeSize/2;
}

int get_possible_position_in_leaf_BPlus(TreeNodeLeafBPlus * nodeleaf , void * key , BPlusTree * tree){
    if(nodeleaf == NULL || key == NULL){
        printf("nodeleaf or key is null, get_possible_position_in_leaf_BPlus error\n");
        return -1;
    }
    return searchPrevDataHasKeyBinarySort(nodeleaf->datas , nodeleaf->size ,key ,getKey_leafData_func ,tree->compareFunc);
}

int get_position_in_node_BPlus(TreeNodeBPlus * node , void * key , BPlusTree * tree){
    if(node == NULL || key == NULL){
        printf("node or key is null, get_position_in_node_BPlus error\n");
        return -1;
    }
    return searchPrevDataHasKeyBinarySort(node->datas , node->size , key ,getKey_nodeData_func , tree->compareFunc);
}

int get_position_in_leaf_BPlus(TreeNodeLeafBPlus * nodeLeaf , void * key , BPlusTree * tree){
    if(nodeLeaf == NULL || key == NULL){
        printf("nodeLeaf or key is null, get_position_in_leaf_BPlus error\n");
        return -1;
    }
    return binarySearchHasKeySort(nodeLeaf->datas , nodeLeaf->size ,key ,getKey_leafData_func ,tree->compareFunc);
}

TreeNodeLeafBPlus * find_leaf_by_key_BPlus(void * key ,BPlusTree *tree){
    if(key == NULL){
        printf("key is null, find_leaf_by_key_BPlus error\n");
        return NULL;
    }
    int i = 1;
    TreeNodeBPlus *node = tree->root;
    while(i < tree->treeHeight){
        int position = get_position_in_node_BPlus(node , key , tree);
        if(position == -1){
            while(i < tree->treeHeight){
                node = node->datas[0]->child;
                i ++;
            }
        }else{
            node = node->datas[position]->child;
            i++;
        }
    }
    return (TreeNodeLeafBPlus *)node;
}

void update_node_index_BPlus(TreeNodeBPlus *node , void *key , void *newKey , BPlusTree *tree){
    if(newKey == NULL){
        printf("newKey is null, update_node_index_BPlus error\n");
        return;
    }
    if(node == NULL){
        return;
    }
    int position = get_position_in_node_BPlus(node , key , tree);
    if(position == -1){
        printf("error in update_node_index_BPlus\n");
        return;
    }
    node->datas[position]->key = newKey;
    if(position == 0){
        update_node_index_BPlus(node->parent , key , newKey , tree);
    }
}

void update_min_index_data_BPlus(TreeNodeBPlus *node , void * key){
    if(node == NULL){
        printf("node is null, update_min_index_data_BPlus error\n");
        return;
    }
    while(node != NULL){
        node->datas[0]->key = key;
        node = node->parent;
    }
}

void  insert_data_to_leaf_BPlus(TreeNodeLeafBPlus *node , void * key , void *data  , BPlusTree * tree){
    if(node == NULL || data == NULL){
        printf("node or data is null, insert_data_to_leaf_BPlus error\n");
        return;
    }
    TreeNodeLeafDataBPlus * leafData = create_tree_node_leaf_data_BPlus(key ,data , tree);
    if(node->size == 0){
        node->datas[0] = leafData;
        node->size = 1;
        push_leaf_data_to_node_BPlus(node->parent , leafData , node , tree);
        return ;
    }
    if(tree->compareFunc(key, node->datas[0]->key) < 0){
        update_min_index_data_BPlus(node->parent , leafData->key);
    }
    //printf("insert data to leaf node %d\n", tree->maxLeafSize);
    if(node->size == tree->maxLeafSize){
        split_leaf_node_BPlus(node , leafData , tree);
    }else{
        node->datas[node->size] = leafData;
        node->size++;      
        insertSortHasKeySort(node->datas , node->size ,getKey_leafData_func, tree->compareFunc);   
    }
}

int get_half_BPlus(int num){
    return num / 2;
}

int insertElementBPlus(BPlusTree *tree, void *data){
    if(tree == NULL || data == NULL){
        printf("tree or data is null, insertElementBPlus error\n");
        return 0;
    }
    void * key = tree->getKeyFunc(data);
    if(tree->root == NULL){
        TreeNodeLeafBPlus * leaf = create_tree_node_leaf_BPlus(tree->maxLeafSize);
        insert_data_to_leaf_BPlus(leaf , key , data , tree);
    }else{
        if(findElementBPlus(key , tree) != NULL){
            printf("error , the key is exist\n");
            return 0;
        }
        TreeNodeLeafBPlus * leaf = find_leaf_by_key_BPlus(key , tree);
        //printf("leaf size is %d\n",leaf->size);
        insert_data_to_leaf_BPlus(leaf , key , data ,tree);
    }
    //printf("insert success\n");
    return 1;
}

TreeNodeBPlus *get_node_brother_BPlus(TreeNodeBPlus * node ,void * key ,  BPlusTree * tree){
    if(node == NULL || key == NULL){
        printf("node or key is null, get_node_brother_BPlus error\n");
        return NULL;
    }
    if(node->parent == NULL){
        return NULL;
    }
    TreeNodeBPlus * parent = node->parent;
    int posstion = get_position_in_node_BPlus(parent , key , tree);
    if(posstion == parent->size - 1){
        return parent->datas[parent->size - 2]->child;
    }else{
        return parent->datas[posstion + 1]->child;
    }
}

void move_elements_BPlus(TreeNodeBPlus *node1 , TreeNodeBPlus *node2 , int moveNum , BPlusTree * tree){
    if(node1 == NULL || node2 == NULL){
        printf("node1 or node2 is null, move_elements_BPlus error\n");
        return;
    }
    if(node1->brother == node2){
        for(int i = 0 ; i < moveNum ; i ++){
            node1->datas[node1->size + i] = node2->datas[i];
            node2->datas[i] = node2->datas[i + moveNum];
            ((TreeNodeBPlus * )node1->datas[node1->size + i]->child)->parent = node1;

        }
        memmove(node2->datas +moveNum, node2->datas + moveNum*2 , sizeof(TreeNodeDataBPlus *) * (node2->size - moveNum*2));
        for(int i = 0 ; i < moveNum ; i ++){
            node2->datas[node2->size - i - 1] = NULL;
        }
        update_node_index_BPlus(node2->parent , node1->datas[node1->size]->key , node2->datas[0]->key , tree);
        node1->size += moveNum;
        node2->size -= moveNum;
    }else{
        memmove(node1->datas + moveNum , node1->datas , sizeof(TreeNodeDataBPlus *) * node1->size);
        for(int i = 0 ; i < moveNum ; i ++){
            node1->datas[i] = node2->datas[node2->size - moveNum + i];
            ((TreeNodeBPlus * )node1->datas[i]->child)->parent = node1;
            node2->datas[node2->size - moveNum + i] = NULL;
        }
        update_node_index_BPlus(node1->parent , node1->datas[moveNum]->key , node1->datas[0]->key , tree);
        node1->size += moveNum;
        node2->size -= moveNum;
    }
}

void move_elaments_All_BPlus(TreeNodeBPlus *node1 , TreeNodeBPlus *node2 , BPlusTree * tree){
    printf("node1->brother is node2 %d\n",node1->brother == node2);
    if(node1 == NULL || node2 == NULL){
        printf("node1 or node2 is null, move_elaments_All_BPlus error\n");
        return;
    }
    for(int i = 0 ; i < node2->size ; i ++){
        node1->datas[node1->size + i] = node2->datas[i];
        node2->datas[i] = NULL;
        ((TreeNodeBPlus * )node1->datas[node1->size + i]->child)->parent = node1;
    }
    delete_data_in_node_BPlus(node1->datas[node1->size]->key  ,node2->parent, tree);
    node1->size += node2->size;
    node1->brother = node2->brother;
    destroy_node_BPlus(node2 , tree);
}

int combind_node_BPlus(TreeNodeBPlus * node1 , TreeNodeBPlus * node2 , BPlusTree *tree){
    if(node1 == NULL || node2 == NULL){
        printf("error in combind node, node is null\n");
        return 0;
    }
    int minSize = get_node_min_size_BPlus(tree) + 1;
    if(node1->size + node2->size >= 2 * minSize){
        move_elements_BPlus(node1 , node2 , get_half_BPlus(node1->size + node2->size) - node1->size , tree);
    }else{
        if(node1->brother == node2){
            move_elaments_All_BPlus(node1 , node2 , tree);
        }else{
            move_elaments_All_BPlus(node2 , node1 , tree);
        }
    }
}

void update_leaf_brother_link_BPlus(TreeNodeLeafBPlus * leafNode , BPlusTree * tree){
    if(leafNode == NULL){
        printf("leafNode is null, update_leaf_brother_link_BPlus error\n");
        return;
    }
    TreeNodeBPlus * node = tree->root;
    int height = 1;
    while(height < tree->treeHeight){
        node = node->datas[0]->child;
        height ++;
    }
    if(node == leafNode){
        return;
    }
    while(node != NULL && node->brother != leafNode ){
        node = node->brother;
    }
    if(node == NULL){
        return;
    }
    node->brother = leafNode->brother;
}

void delete_data_in_node_BPlus(void * key , TreeNodeBPlus * node , BPlusTree * tree){
    if(key == NULL || node == NULL){
        printf("key or node is null, delete_data_in_node_BPlus error\n");
        return;
    }
    int position = get_position_in_node_BPlus(node , key , tree);
    printf("delete data in node , key is %d node is %d , position is %d , node size is %d\n" , *(int *)key , *(int *)node->datas[0]->key , position , node->size);
    TreeNodeDataBPlus * dataToDelete = node->datas[position];
    memmove(node->datas + position , node->datas + position + 1 , (node->size - position - 1) * sizeof(TreeNodeDataBPlus *));
    node->size --;
    node->datas[node->size] = NULL;
    if(node->size == 0){
        destroy_node_data_BPlus(dataToDelete , tree);
        destroy_node_BPlus(node , tree);
        tree->root = NULL;
        tree->treeHeight = 0;
        return;
    }
    if(node->parent == NULL && node->size == 1 && tree->treeHeight > 2){
        tree->root = node->datas[0]->child;
        tree->root->parent = NULL;
        tree->treeHeight --;
        destroy_node_data_BPlus(dataToDelete , tree);
        destroy_node_BPlus(node , tree);
        return;
    }
    if(position == 0){
        update_node_index_BPlus(node->parent , key , node->datas[0]->key , tree);
    }
    if(node_too_less_BPlus(node , tree)){
        TreeNodeBPlus * brother = get_node_brother_BPlus(node , node->datas[0]->key , tree);
        if(brother == NULL){
            return;
        }
        combind_node_BPlus(node , brother , tree);
    }
    destroy_node_data_BPlus(dataToDelete , tree);
}

int delete_data_in_leaf_BPlus(TreeNodeLeafBPlus *leafNode , void *key , int position , BPlusTree *tree){
    if(leafNode == NULL || key == NULL || position < 0 || position >= leafNode->size){
        printf("leafNode or key is null or possition not correct, delete_data_in_leaf_BPlus error\n");
        return 0;
    }
    //printf("delete data in leaf , key is %d , position is %d , leafNode is %d \n" , *(int *)key , position , *(int *)leafNode->datas[0]->key);
    TreeNodeLeafDataBPlus * dataToDelete = leafNode->datas[position];
    memmove(leafNode->datas + position , leafNode->datas + position + 1 , (leafNode->size - position - 1) * sizeof(TreeNodeLeafDataBPlus *));
    leafNode->size--;
    leafNode->datas[leafNode->size] = NULL;
    if(leafNode->size == 0){
        delete_data_in_node_BPlus(key , leafNode->parent , tree);
        update_leaf_brother_link_BPlus(leafNode , tree);
        destroy_leaf_node_BPlus(leafNode , tree);
    }else if(position == 0){
        update_node_index_BPlus(leafNode->parent , key , leafNode->datas[0]->key , tree);
    }
    destroy_leaf_data_BPlus(dataToDelete , tree);
}

void *findElementBPlus(void *key , BPlusTree *tree){
    if(tree == NULL || key == NULL){
        printf("tree or key is null, findElementBPlus error\n");
        return NULL;
    }
    TreeNodeLeafBPlus * node =  find_leaf_by_key_BPlus(key , tree);
    int position = get_position_in_leaf_BPlus(node , key , tree);
    if(position == -1){
        return NULL;
    }
    return node->datas[position]->data;
}

int deleteElementBPlus(void *key , BPlusTree * tree){
    if(tree == NULL || key == NULL){
        printf("tree or key is null, deleteElementBPlus error\n");
        return 0;
    }
    TreeNodeLeafBPlus *nodeLeaf = find_leaf_by_key_BPlus(key ,tree);
    int position = get_position_in_leaf_BPlus(nodeLeaf , key , tree);
    if(position == -1){
        printf("data not exists\n");
    }
    return delete_data_in_leaf_BPlus(nodeLeaf , key , position , tree);    
}

BPlusTree *createBPlusTreeBPlus(void * (*getKeyFunc)(void *data) , int (*compareFunc)(void * key1, void *key2) , int maxNode){
    if(maxNode < 3){
       printf("tree nodeMax must bigger than 2\n");
       return NULL;
    }
    BPlusTree *tree = (BPlusTree*)malloc(sizeof(BPlusTree));
    tree->root = NULL;
    tree->maxNodeSize = maxNode;
    tree->maxLeafSize = 2* maxNode;
    tree->getKeyFunc = getKeyFunc;
    if(getKeyFunc == NULL){
        tree->getKeyFunc = getKey_func_default_BPlus;
    }
    tree->compareFunc = compareFunc;
    if(compareFunc == NULL){
        tree->compareFunc = compare_key_func_default_BPlus;
    }
    tree->freeKeyFunc = NULL;
    tree->freeDataFunc = NULL;
    tree->treeHeight = 0;
    return tree;
}