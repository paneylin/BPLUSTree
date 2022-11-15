#include "./bPlusTree.c"

int compareFunc(int * a , int * b){
    return *a - *b;
}

int testIndex(BPlusTree * tree ){
    CircleQueue *queue = createCircleQueue();
    CircleQueue * queue2 = createCircleQueue();
    int height = 1;
    pushCircleQueue(tree->root , queue);
    TreeNodeBPlus * brotherNode = NULL;
    while(!isEmptyCircleQueue(queue) || !isEmptyCircleQueue(queue2)){
        if(height != tree->treeHeight){
            TreeNodeBPlus *node = height%2 == 1 ?(TreeNodeBPlus *)popCircleQueue(queue):(TreeNodeBPlus *)popCircleQueue(queue2);
            for(int i = 0; i < node->size; i++){
                if(((TreeNodeBPlus *)node->datas[i]->child)->parent != node){
                    printf("testIndex fail ,node is %d , height is %d , node->datas[%d] parent-child wrong!\n" , *(int *)node->datas[0]->key , height , i);
                    return 0;
                }
                if(((TreeNodeBPlus *)node->datas[i]->child)->datas[0]->key != node->datas[i]->key){
                    printf("testIndex fail ,node is %d , height is %d , node->datas[%d] key wrong!\n" , *(int *)node->datas[0]->key , height , i);
                    return 0;
                }
                pushCircleQueue(node->datas[i]->child , height%2 == 1 ? queue2: queue);
            }
            if(brotherNode != NULL && brotherNode != node){
                printf("testIndex fail ,node is %d , height is %d ,brotherNode is %d node->preBrother wrong!\n" , *(int *)node->datas[0]->key , height , *(int *)brotherNode->datas[0]->key);
                return 0;
            }
            brotherNode = node->brother;
        }else{
            TreeNodeLeafBPlus *node = height%2 == 1 ?(TreeNodeLeafBPlus *)popCircleQueue(queue):(TreeNodeLeafBPlus *)popCircleQueue(queue2);
            if(brotherNode != NULL && (TreeNodeLeafBPlus *)brotherNode != node){
                printf("testIndex fail ,leafNode is %d , height = %d ,brotherNode is %d, leafNode->preBrother wrong!\n" , *(int *)node->datas[0]->key , height ,*(int *)brotherNode->datas[0]->key);
                return 0;
            }
            brotherNode = node->brother;
        }
        if(isEmptyCircleQueue(height%2 == 1 ? queue:queue2)){
            height ++;
        }
    }
    destroyCircleQueue(queue);
    destroyCircleQueue(queue2);
    //printf("testIndex success\n");
    return 1;
}

int testDatain(BPlusTree * tree , int ** datas , int dataSize){
    for(int i = 0 ; i < dataSize ; i ++){
        void * data = findElementBPlus(datas[i] , tree);
        if(data == NULL){
            printf("error , %d is not in tree \n" , *datas[i]);
            return 0;
        }
    }
    //printf("testDatain success\n");
    return 1;
}

int testDataNum(BPlusTree * tree , int ** datas , int dataSize){
    int height = 1;
    TreeNodeBPlus * node = tree->root;
    while (height < tree->treeHeight) {
        node = node->datas[0]->child;
        height++;
    }
    TreeNodeLeafBPlus * leaf = (TreeNodeLeafBPlus *)node;
    int size = 0;
    while(leaf != NULL){
        size += leaf->size;
        leaf = leaf->brother;
    }
    if(size != dataSize){
        printf("testDataNum fail , size = %d , dataSize = %d \n" , size , dataSize);
        return 0;
    }
    //printf("testDataNum success\n");
    return 1;
}

int testNodeSize(BPlusTree * tree) {
    CircleQueue *queue = createCircleQueue();
    CircleQueue * queue2 = createCircleQueue();
    int height = 1;
    pushCircleQueue(tree->root , queue);
    while(!isEmptyCircleQueue(queue) || !isEmptyCircleQueue(queue2)){
        if(height != tree->treeHeight){
            TreeNodeBPlus *node = height%2 == 1 ?(TreeNodeBPlus *)popCircleQueue(queue):(TreeNodeBPlus *)popCircleQueue(queue2);
            if(height != 1 && (node_too_less_BPlus(node ,tree) || node->size > tree->maxNodeSize) ){
                printf("testNodeSize fail ,node size not correct ,node %d , size = %d ,height = %d \n" , *(int *)node->datas[0]->key , node->size , height);
                return 0;
            }
            for(int i = 0; i < node->size; i++){
                if(node->datas[i] == NULL){
                    printf("testNodeSize fail ,node is %d , height is %d , node->datas[%d] == NULL\n" , *(int *)node->datas[0]->key , height , i);
                    return 0;
                }
                pushCircleQueue(node->datas[i]->child , height%2 == 1 ? queue2: queue);
            }
            if(node->size != tree->maxNodeSize &&node->datas[node->size] != NULL){
                printf("testNodeSize fail ,node is %d , height is %d , data value out of range , data is %d\n" , *(int *)node->datas[0]->key , height , *(int *)node->datas[node->size]->key);
                return 0;
            }
        }else{
            TreeNodeLeafBPlus *node = height%2 == 1 ?(TreeNodeLeafBPlus *)popCircleQueue(queue):(TreeNodeLeafBPlus *)popCircleQueue(queue2);
            if(node->size <= 0 || node->size > tree->maxLeafSize){
                printf("testNodeSize fail ,leaf size not correct ，node %d , size = %d ,height = %d \n" , *(int *)node->datas[0]->key , node->size , height);
                return 0;
            }
            for(int i = 0; i < node->size; i++){
                if(node->datas[i] == NULL){
                    printf("testNodeSize fail ,leaf is %d , height is %d , node->datas[%d] == NULL\n" , *(int *)node->datas[0]->key , height , i);
                    return 0;
                }
            }
            if(node->size != tree->maxLeafSize && node->datas[node->size] != NULL){
                printf("testNodeSize fail ,leaf is %d , height is %d , data value out of range\n" , *(int *)node->datas[0]->key , height);
                return 0;
            }
        }
        if(isEmptyCircleQueue(height%2 == 1 ? queue:queue2)){
            height ++;
        }
    }
    destroyCircleQueue(queue);
    destroyCircleQueue(queue2);
    //printf("testNodeSize success\n");
    return 1;
}

int testBPlusTree(BPlusTree * tree , int ** datas , int dataSize) {  
    if(tree == NULL || tree->root == NULL){
        printf("testBPlusTree success , tree is NULL\n");
        return 1;
    }
    if(!testNodeSize(tree)){
        return 0;
    }
    if(!testIndex(tree)){
        return 0;
    }
    if(!testDatain(tree, datas, dataSize)){
        return 0;
    }
    if(!testDataNum(tree, datas, dataSize)){
        return 0;
    }
    printf("testBPlusTree success\n");
    return 1;
}

void showNodedata(void * _Key) {
    printf("%d ", *(int *)_Key);
}

void showLeafData(void * key , void * data) {
    printf("key : %d  data : %d ", *(int *)key , *(int *)data);
}

int main(){
    BPlusTree *tree = createBPlusTreeBPlus(NULL,NULL,3);
    printf("tree create success\n");
    int number = 0;
    scanf("%d",&number);
    int **datas = (int **)malloc(sizeof(int*) * number);
    for(int i = 0;i < number;){
        int *key = (int*)malloc(sizeof(int)); 
        *key = rand() % 10000;
        if(insertElementBPlus(tree , key)){
            datas[i] = key;
            i++;
        }
    }
    if(testBPlusTree(tree, datas, number)){
        printf("test insert success\n");
        for(int i = number - 1 ; i >= 0 ; i--){
            deleteElementBPlus(datas[i] , tree);
            printf("delete index %d success\n" , i);
            if(!testBPlusTree(tree , datas , i)){
                printf("error , testBPlusTree fail , delete index is %d , delete data is %d\n\n\n\n\n\n\n\n\n" , i , *datas[i]);
                //showTreeBplus(tree , showNodedata , showLeafData);
                break;
            }
        }
    }
    printf("\n");
    showTreeBplus(tree , showNodedata , showLeafData);
    printf("success\n");
}