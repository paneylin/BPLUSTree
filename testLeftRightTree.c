#include"./leftRightTree.c"
//用于显示中如何显示state状态
int getNodeValueLR(void * nodeNum){
    return *(int *)nodeNum;
}

//节点与值比较函数，相同返回0，值小于节点返回负值，值大于节点返回正值
int compareTreeNodeValueLR(TreeNodeLR *node ,void * nodeNum){
    return getNodeValueLR(nodeNum) - getNodeValueLR(node->nodeNum);
}

//节点与节点比较函数，值相同返回0，节点1小于节点2返回负值，反之返回正值
int compareTreeNodeLR(TreeNodeLR *node1 , TreeNodeLR *node2){
    return getNodeValueLR(node1->nodeNum) - getNodeValueLR(node2->nodeNum);
}

//插入节点时候，相同值节点存在情况下操作
void doWhenNodeExistLR(TreeNodeLR * node , TreeNodeLR * parent){
    printf("node %d exist\n",getNodeValueLR(node->nodeNum));
    free(node);
}

int main(){
    int *number = (int *)malloc(sizeof(int));
    *number = 2;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 50;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 20;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 30;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 40;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 60;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 70;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 80;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 90;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 100;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 110;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    number = (int *)malloc(sizeof(int));
    *number = 120;
    insertTreeLR(number);
    showTreeLR(leftRightTreeInfo->root);
    
    while(scanf("%d",&number) != EOF){
        if(*number == -1){
            break;
        }
        deleteNodeLR(number);
        showTreeLR(leftRightTreeInfo->root);
    }
    return 0;
}