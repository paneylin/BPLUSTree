#include"./persistentRBTree.c"
#include"./CircleQueue.h"

int compare(void *data1,void *data2){
    return *(int *)data1 - *(int *)data2;
}

void show(void *data){
    printf("%d ",*(int *)data);
}

void showNode(void *data){
    TreeNodePRB * node = (TreeNodePRB *)data;
    show(node->data);
}

void test_root_black_PRBTree(TreeNodePRB * root){
    if(validRed_PRBTree(root)){
        printf("root is not black");
    }
}

void test_no_continue_red_PRBTree(TreeNodePRB * root){
    if(root == NULL){
        return;
    }
    if(validRed_PRBTree(root)){
        if(root->left != NULL){
            if(validRed_PRBTree(root->left)){
                printf("continue red node");
            }
            test_no_continue_red_PRBTree(root->left);
        }
        if(root->right != NULL){
            if(validRed_PRBTree(root->right)){
                printf("continue red node");
            }
            test_no_continue_red_PRBTree(root->right);
        }
    }
}

void test_leaf_height_PRBTree(TreeNodePRB * root , int version){
    int height = 0;
    TreeNodePRB * node = root;
    while(node != NULL){
        validRed_PRBTree(node) ? height : height ++;
        node = node->left;
    }
    CircleQueue * queue = createCircleQueue();
    pushCircleQueue(root , queue);
    while(!isEmptyCircleQueue(queue)){
        node = popCircleQueue(queue);
        if(node->left != NULL){
            pushCircleQueue(node->left , queue);
        }
        if(node->right != NULL){
            pushCircleQueue(node->right , queue);
        }
        //show(node->data);
        if(node->left == NULL || node->right == NULL){
            int nodeHeight = 0;
            while(node != NULL){
                validRed_PRBTree(node) ? nodeHeight : nodeHeight ++;
                node = get_parent_node_PRBTree(version , node->parent);
            }
            if(nodeHeight != height){
                printf("leaf height is not same");
                break;
            }
        }
    }
    destroyCircleQueueQ(queue);
}

void test_data_correct_PRBTree(TreeNodePRB * root , void **data , int version , int dataSize , int (*compareFunc)(void * , void *)){
    if(root == NULL && dataSize != 0){
        printf("data size is not right");
        return;
    }
    TreeNodePRB * node = NULL;
    for(int i = 0 ; i < dataSize ; i ++){
        TreeNodePRB * node = find_node_from_node_PRBTree(root , data[i] , compareFunc);
        if(node == NULL){
            printf("data is not in tree , data :");
            show(data[i]);
            printf("\n");
            return;
        }
    }
    int size = 0;
    node = root;
    CircleQueue * queue = createCircleQueue();
    CircleQueue * leafNodeQueue = createCircleQueue();
    pushCircleQueue(node , queue);
    while(!isEmptyCircleQueue(queue)){
        node = popCircleQueue(queue);
        size ++;
        if(node->left != NULL){
            pushCircleQueue(node->left , queue);
        }
        if(node->right != NULL){
            pushCircleQueue(node->right , queue);
        }
        if(node->left == NULL && node->right == NULL){
            pushCircleQueue(node , leafNodeQueue);
        }
    }
    if(size != dataSize){
        printf("data size is not right");
    }
    while(!isEmptyCircleQueue(leafNodeQueue)){
        node = popCircleQueue(leafNodeQueue);
        while(node->parent != NULL){
            node = node->parent == NULL ? NULL : get_parent_node_PRBTree(version ,node->parent);
        }
        if(node != root){
            printf("data verion is not right");
        }
    }
    destroyCircleQueueQ(queue);
    destroyCircleQueueQ(leafNodeQueue);
}

int test_tree_insert_PRBTree(PersistentTreeRB * tree , void **data ,int versionStart , int versionEnd , int (*compareFunc)(void * , void *)){
    int treeDataSize = 1;
    for(int i = versionStart ; i <= versionEnd ; i ++){
        TreeNodePRB * root = get_root_node_by_vertion_PRBTree(tree , i);
        //show_tree_node_PRBTree(root , i , show);
        test_root_black_PRBTree(root);
        test_no_continue_red_PRBTree(root);
        test_leaf_height_PRBTree(root , i);
        test_data_correct_PRBTree(root , data , i , treeDataSize , compareFunc);
        treeDataSize ++;
    }
    printf("test tree insert end\n");
}

void test_tree_delete_PRBTree(PersistentTreeRB * tree , void **data ,int dataSize, int versionStart , int versionEnd ,int (*compareFunc)(void * , void *)){
    int deleteNum = 1;
    for(int i = versionStart ; i <= versionEnd ; i ++){
        TreeNodePRB * root = get_root_node_by_vertion_PRBTree(tree , i);
        if(dataSize - deleteNum == 0 && root == NULL){
            continue;
        }
        test_root_black_PRBTree(root);
        test_no_continue_red_PRBTree(root);
        test_leaf_height_PRBTree(root , i);
        test_data_correct_PRBTree(root , data , i , dataSize - deleteNum , compareFunc);
        deleteNum ++;
    }
    printf("test tree delete end\n");
}

void rand_data_sequence(void **data , int dataSize){
    for(int i = 0 ; i < dataSize ; i ++){
        int exchange1 = rand() % dataSize;
        int exchange2 = rand() % dataSize;
        void * temp = data[exchange1];
        data[exchange1] = data[exchange2];
        data[exchange2] = temp;
    }
}

void show_data(void **data , int dataSize){
    for(int i = 0 ; i < dataSize ; i ++){
        printf("%d ",*(int *)data[i]);
    }
    printf("\n");
}

int main(){
    PersistentTreeRB * tree = createPersistentTreeRB();
    int num;
    scanf("%d",&num);
    int **datas = (int **)malloc(sizeof(int *) * num);
    int dataSize = 0;
    for(int i = 0;i < num;i++){
        int *data = (int *)malloc(sizeof(int));
        *data = rand() % 10000;
        if(findElementPRBTree(tree , data , tree->currentVersion, compare) == NULL){
            datas[dataSize] = data;
            insertElementPRBTree(tree , data, compare);
            dataSize ++;
        }
    }
    test_tree_insert_PRBTree(tree , datas , 1 , dataSize , compare);  
    show_data(datas , dataSize);
    rand_data_sequence(datas , dataSize);
    show_data(datas , dataSize);
    showTreePRBTree(tree , tree->currentVersion ,show);
    for(int i = dataSize - 1;i >= 0 ;i --){
        deleteElementPRBTree(tree , datas[i] , compare);
    } 
    showTreePRBTree(tree , tree->currentVersion ,show);
    test_tree_delete_PRBTree(tree , datas , dataSize , dataSize + 1 , dataSize + 20 , compare);
}
