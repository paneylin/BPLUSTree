#include"./leftRightTree.h"
#include"./CircleQueue.h"

int compare(void *data1,void *data2){
    return *(int *)data1 - *(int *)data2;
}

void *getKey(void *data){
    return data;
}

void test_node_num(TreeLRTree * tree , int num){
    int count = 0;
    TreeNodeLRTree *cur = tree->root;
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    while(getElementsFromCircleQueue(queue)!= NULL){
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
    }
    printf("node count test end\n");
}

void test_node_Index(TreeLRTree * tree){
    int count = 0;
    TreeNodeLRTree *cur = tree->root;
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    while(getElementsFromCircleQueue(queue)!= NULL){
        TreeNodeLRTree * node = popCircleQueue(queue);
        if(node->left != NULL){
            if(compare(getKey(node->data) , getKey(node->left->data)) < 0){
                printf("error indexing");
            }
            pushCircleQueue(node->left ,queue);
        }
        if(node->right != NULL){
            if(compare(getKey(node->data) , getKey(node->right->data)) > 0){
                printf("error indexing");
            }
            pushCircleQueue(node->right ,queue);
        }
        count ++;
    }
    printf("node index test end\n");
}

void test_node_in(void *data , TreeLRTree* tree){
    TreeNodeLRTree *cur = tree->root;
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    int find = 0;
    while(getElementsFromCircleQueue(queue)!= NULL){
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
    }
}

void test_getAlldata(TreeLRTree * tree){
    ArrayList * list = getTreeAllDatasLeftToRightLRTree(tree);
    int i = 0;
    int * lastdata = getElementByIndexAList(i , list);
    while( ++i < getSizeAList(list)){
        int * qData = getElementByIndexAList(i , list);
        if(compare(lastdata , qData)>0){
            printf("error get data in seq\n");
        }
        lastdata = qData;
    }
    destroyAList(list);
    printf("get all data test end\n");
}

void text_balance(TreeLRTree *tree){
    TreeNodeLRTree *cur = tree->root;
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(cur , queue);
    while(getElementsFromCircleQueue(queue)!= NULL){
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
        }
        if(!balance_function_default_LRTree(node, tree->balanceFactor)){
            printf("error balance\n");
        }
    }
    printf("balance test end\n");
}

void test_LRTree(TreeLRTree *tree , void ** datas , int dataSize){
    test_node_num(tree , dataSize);
    for(int i = 0 ; i < dataSize ; i ++){
        test_node_in(datas[i] , tree);
    }
    printf("node in test end\n");
    test_node_Index(tree);
    test_getAlldata(tree);
    text_balance(tree);
    printf("test success");
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
    test_LRTree(tree , datas , num);
}



