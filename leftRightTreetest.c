#include"./leftRightTree.h"
#include"./CircleQueue.h"

int compare(void *data1,void *data2){
    return *(int *)data1 - *(int *)data2;
}

void *getKey(void *data){
    return data;
}

int test_node_num(TreeLRTree * tree , int num){
    int count = 0;
    TreeNodeLRTree *cur = tree->root;
    printf("%d %d\n" , cur == NULL , num);
    if(cur == NULL && count == num){
        printf("node count test end\n");
        return 1;
    }else if(cur == NULL){
        printf("node count error");
        return 0;
    }
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    while(!isEmptyCircleQueue(queue)){
        TreeNodeLRTree * node = popCircleQueue(queue);
        if(node->left != NULL){
            pushCircleQueue(node->left ,queue);
        }
        if(node->right != NULL){
            pushCircleQueue(node->right ,queue);
        }
        count ++;
    }
    if(count != num){
        printf("node count error");
        return 0;
    }
    printf("node count test end\n");
    return 1;
}

int test_node_Index(TreeLRTree * tree){
    int count = 0;
    TreeNodeLRTree *cur = tree->root;
    if(cur == NULL){
        printf("node index test end\n");
        return 1;
    }
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    while(!isEmptyCircleQueue(queue)){
        TreeNodeLRTree * node = popCircleQueue(queue);
        if(node->left != NULL){
            if(compare(getKey(node->data) , getKey(node->left->data)) < 0){
                printf("error indexing");
                return 0;
            }
            pushCircleQueue(node->left ,queue);
        }
        if(node->right != NULL){
            if(compare(getKey(node->data) , getKey(node->right->data)) > 0){
                printf("error indexing");
                return 0;
            }
            pushCircleQueue(node->right ,queue);
        }
        count ++;
    }
    printf("node index test end\n");
    return 1;
}

int test_node_in(void *data , TreeLRTree* tree){
    TreeNodeLRTree *cur = tree->root;
    if(cur == NULL){
        return 0;
    }
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    int find = 0;
    while(!isEmptyCircleQueue(queue)){
        TreeNodeLRTree * node = popCircleQueue(queue);
        if(compare(node->data , data) == 0){
            find = 1;
            break;
        }
        if(node->left != NULL){
            pushCircleQueue(node->left ,queue);
        }
        if(node->right != NULL){
            pushCircleQueue(node->right ,queue);
        }
    }
    if(find == 0){
        printf("data not find");
        return 0;
    }
    return 1;
}

int test_getAlldata(TreeLRTree * tree){
    ArrayList * list = getTreeAllDatasLeftToRightLRTree(tree);
    if(list == NULL){
        printf("get all data test end\n");
        return 1;
    }
    int i = 0;
    int * lastdata = getElementByIndexAList(i , list);
    while( ++i < getSizeAList(list)){
        int * qData = getElementByIndexAList(i , list);
        if(compare(lastdata , qData)>0){
            printf("error get data in seq\n");
            return 0;
        }
        lastdata = qData;
    }
    destroyAList(list);
    printf("get all data test end\n");
    return 1;
}

int text_balance(TreeLRTree *tree){
    TreeNodeLRTree *cur = tree->root;
    if(cur == NULL){
        printf("balance test end\n");
        return 1;
    }
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    while(!isEmptyCircleQueue(queue)){
        TreeNodeLRTree * node = popCircleQueue(queue);
        int size = 0;
        if(node->left != NULL){
            pushCircleQueue(node->left ,queue);
            size += node->left->size;
        }
        if(node->right != NULL){
            pushCircleQueue(node->right ,queue);
            size += node->right->size;
        }
        
        if(node->size != size + 1){
            printf("error balance size\n");
            return 0;
        }
        if(!balance_function_default_LRTree(node, tree->balanceFactor)){
            printf("error balance\n");
            return 0;
        }
    }
    printf("balance test end\n");
    return 1;
}

int test_LRTree(TreeLRTree *tree , void ** datas , int dataSize){
    if(!test_node_num(tree , dataSize)){
        return 0;
    }
    for(int i = 0 ; i < dataSize ; i ++){
        if(!test_node_in(datas[i] , tree)){
            return 0;
        }
    }
    if(!test_node_Index(tree)){
        return 0;
    }
    if(!test_getAlldata(tree)){
        return 0;
    }
    if(!text_balance(tree)){
        return 0;
    }
    printf("test success\n");
    return 1;
}

int main(){
    TreeLRTree * tree = createTreeLRTree(compare , getKey);
    int num;
    scanf("%d" , &num);
    int **datas = (int **)malloc(sizeof(int *) * num);
    for(int i = 0 ; i < num ;){
        int *data = (int *)malloc(sizeof(int));
        *data = rand()%1000;
        datas[i] = data;
        if(getDataFromTreeLRTree(data , tree) == NULL){
            insertDataLRTree(data , tree);
            i++;
        }
    }
    printf("insert success!\n");
    if(!test_LRTree(tree , datas , num)){
        printf("test failed\n");
        return ;
    }
    while(tree->root != NULL){
        printf("%d " , *(int *)popMaxDataLRTree(tree));
    }
    /*for(int i = 0 ; i < num ; i ++){
        //printf("deleting %d\n" , *(datas[num - 1 - i]));
        deleteElementLRTree(datas[num - 1 - i] , tree);
        //printf("delete end %d %d\n" , *(datas[num - 1 - i]) , i);
        if(!test_LRTree(tree , datas , num - 1 - i)){
            printf("delete %d failed\n" , i + 1);
            return ;
        }
    }*/
    printf("end\n");
}



