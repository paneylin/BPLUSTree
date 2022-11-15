#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//二叉树
typedef struct TreeNodeLR
{
    void * nodeNum;
    int height;
    int leftHeight;
    int rightHeight;
    struct TreeNodeLR *parent;
    struct TreeNodeLR *left;
    struct TreeNodeLR *right;
}TreeNodeLR;

typedef struct LeftRightTreeInfoLR
{
    TreeNodeLR *root;
}LeftRightTreeInfoLR;

LeftRightTreeInfoLR *leftRightTreeInfo = NULL;

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
    printf("节点已存在，不做任何操作");
}

void insertTreeLR(void *nodeNum);

void emptyLeftRightTreeLR();

void deleteNodeLR(void *key);

void *findElementLR(void *key);

void show_tree_node_LR(TreeNodeLR * node){
    printf("%d %d",node->height,getNodeValueLR(node->nodeNum));
    if(node -> left != NULL){
        printf("left: %d ",getNodeValueLR(node->left->nodeNum));
    }
    if(node->right != NULL){
        printf("right: %d ",getNodeValueLR(node->right->nodeNum));
    }
    printf("\n");
}

void show_tree_order_back_LR(TreeNodeLR * node){
    TreeNodeLR *preOp = node->parent;
    TreeNodeLR *currentOp = node;
    while(currentOp != node->parent){
        if(preOp == NULL){
            if(currentOp->right == NULL){
                show_tree_node_LR(currentOp);
                if(currentOp->left != NULL){
                    preOp = currentOp;
                    currentOp = currentOp->left;
                }else{
                    preOp = currentOp;
                    currentOp = currentOp->parent;
                }
            }else{
                preOp = currentOp;
                currentOp = currentOp->right;
            }
        }else if(currentOp->right != NULL && preOp == currentOp->parent){
            preOp = currentOp;
            currentOp = currentOp->right;
        }else if(currentOp->right == NULL && preOp == currentOp->parent){
            show_tree_node_LR(currentOp);
            if(currentOp ->left != NULL){
                preOp = currentOp;
                currentOp = currentOp->left;
            }else{
                preOp = currentOp;
                currentOp = currentOp->parent;
            }
        }else if(currentOp->right == preOp){
            show_tree_node_LR(currentOp);
            if(currentOp->left != NULL){
                preOp = currentOp;
                currentOp = currentOp->left;
            }else{
                preOp = currentOp;
                currentOp = currentOp->parent;
            }
        }else{
            preOp = currentOp;
            currentOp = currentOp->parent;
        }
    }
}

void show_tree_order_pre_LR(TreeNodeLR * node){
    TreeNodeLR *preOp = node->parent;
    TreeNodeLR *currentOp = node;
    while(currentOp != node->parent){
        if(preOp == NULL){
            if(currentOp->left == NULL){
                show_tree_node_LR(currentOp);
                if(currentOp->right != NULL){
                    preOp = currentOp;
                    currentOp = currentOp->right;
                }else{
                    preOp = currentOp;
                    currentOp = currentOp->parent;
                }
            }else{
                preOp = currentOp;
                currentOp = currentOp->left;
            }
        }else if(currentOp->left != NULL && preOp == currentOp->parent){
            preOp = currentOp;
            currentOp = currentOp->left;
        }else if(currentOp->left == NULL && preOp == currentOp->parent){
            show_tree_node_LR(currentOp);
            if(currentOp ->right != NULL){
                preOp = currentOp;
                currentOp = currentOp->right;
            }else{
                preOp = currentOp;
                currentOp = currentOp->parent;
            }
        }else if(currentOp->left == preOp){
            show_tree_node_LR(currentOp);
            if(currentOp->right != NULL){
                preOp = currentOp;
                currentOp = currentOp->right;
            }else{
                preOp = currentOp;
                currentOp = currentOp->parent;
            }
        }else{
            preOp = currentOp;
            currentOp = currentOp->parent;
        }
    }
}

void show_tree_order_middle_LR(TreeNodeLR * node){
    TreeNodeLR *preOp = node -> parent;
    TreeNodeLR *currentOP = node;
    while(currentOP != node->parent){
        if(currentOP->parent == preOp){
            show_tree_node_LR(currentOP);
            if(currentOP->left != NULL){
                preOp = currentOP;
                currentOP = currentOP->left;
            }else if(currentOP ->right != NULL){
                preOp = currentOP;
                currentOP = currentOP->right;
            }else{
                preOp = currentOP;
                currentOP = currentOP->parent;
            }
        }else if(preOp == currentOP->left){
            if(currentOP->right != NULL){
                preOp = currentOP;
                currentOP = currentOP->right;
            }else{
                preOp = currentOP;
                currentOP = currentOP->parent;
            }
        }else{
            preOp = currentOP;
            currentOP = currentOP->parent;
        }
    }
}

void show_Tree_info_lr(TreeNodeLR *node){
    if(node == NULL){
        printf("leftrightTree error\n");
        return;
    }
    printf("(%d %d %d %d)",node->height ,getNodeValueLR(node->nodeNum),node->leftHeight,node->rightHeight);
    if(node->left != NULL){
        printf("left: (%d,%d,%d)",getNodeValueLR(node->left->nodeNum),node->left->leftHeight,node->left->rightHeight);
    }
    if(node->right != NULL){
        printf("right： (%d,%d,%d)",getNodeValueLR(node->right->nodeNum),node->right->leftHeight,node->right->rightHeight);
    }
    printf("\n");
}


void get_next_height_node_lr(TreeNodeLR *nodes[] , int length){
    TreeNodeLR **rsl = (TreeNodeLR **)malloc(sizeof(TreeNodeLR *)*length * 2);
    int j = 0;
    for(int i = 0 ; i < length ; i++){
        if(nodes[i]->left != NULL){
            rsl[j++] = nodes[i]->left;
            show_Tree_info_lr(nodes[i]->left);
        }
        if(nodes[i]->right != NULL){
            rsl[j++] = nodes[i]->right;
            show_Tree_info_lr(nodes[i]->right);
        }
    }
    free(nodes);
    if(j > 0){
        get_next_height_node_lr(rsl , j);
    }else{
        free(rsl);
    }
}

void showTreeLR(){
    TreeNodeLR **rsl = (TreeNodeLR **)malloc(sizeof(TreeNodeLR *));
    rsl[0] = leftRightTreeInfo->root;
    show_Tree_info_lr(rsl[0]);
    get_next_height_node_lr(rsl , 1);
    printf("end\n");
}

int update_tree_node_height_lr(TreeNodeLR *node , int height){
    node->height = height ++;
    node->leftHeight = node->left == NULL ? 0 :update_tree_node_height_lr(node->left , height);
    node->rightHeight = node->right == NULL ? 0: update_tree_node_height_lr(node->right ,height);
    return node->leftHeight > node->rightHeight ? (node->leftHeight + 1) : (node->rightHeight + 1);
}

TreeNodeLR *create_tree_node_lr(void *nodeNum ){
    TreeNodeLR *node = (TreeNodeLR *)malloc(sizeof(TreeNodeLR));
    node->nodeNum = nodeNum;
    node->height = 0;
    node->parent = node->left = node->right = NULL;
    node->leftHeight = node->rightHeight = 0;
    return node;
}

void init_tree_lr(TreeNodeLR * node){
    leftRightTreeInfo = (LeftRightTreeInfoLR *)malloc(sizeof(LeftRightTreeInfoLR));
    leftRightTreeInfo->root = node;
}

int test_balance_lr(TreeNodeLR *node){
    return abs(node->leftHeight - node->rightHeight) > 1;
}

TreeNodeLR *get_min_node_lr(TreeNodeLR *node){
    if(node->left == NULL){
        return node;
    }
    return get_min_node_lr(node->left);
}

TreeNodeLR *get_max_node_lr(TreeNodeLR *node){
    if(node->right == NULL){
        return node;
    }else{
        return get_max_node_lr(node->right);
    }
}

TreeNodeLR * find_tree_node_lr(TreeNodeLR *node , void *num){
    //printf("find_tree_node_lr %d in %d\n",num,node->num);
    if(node == NULL){
        printf("Error! \n");
        return NULL;
    }
    int compareValue = compareTreeNodeValueLR(node , num);
    if(compareValue == 0){
        return node;
    }
    if(node->right !=NULL && compareValue > 0){
        return find_tree_node_lr(node->right , num);
    }else if(node->left != NULL && compareValue < 0){
        return find_tree_node_lr(node->left , num);
    }
    return NULL;
}

TreeNodeLR * check_child_branch_lr(TreeNodeLR * child , TreeNodeLR * parent){
    return child == parent->left ? parent->left : parent->right;
}

void switch_node_to_balance_lr(TreeNodeLR * nodelow , TreeNodeLR * nodehigh);

void balance_tree(TreeNodeLR *node){
    TreeNodeLR * temp = node->leftHeight > node->rightHeight ? get_max_node_lr(node->left) : get_min_node_lr(node->right);
    switch_node_to_balance_lr(temp , node);
}

int insert_tree_node_lr(TreeNodeLR *node , TreeNodeLR *parent){
    node->height = parent->height + 1;
    int compareValue = compareTreeNodeLR(node , parent);
    if(compareValue > 0){
        if(parent->right == NULL){
            parent->right = node;
            node->parent = parent;
            parent->rightHeight ++;
            if(parent->left == NULL){
                return 1;
            }else{
                return 0;
            }
        }else{
            if(insert_tree_node_lr(node , parent->right)){
                parent->rightHeight ++;
                if(test_balance_lr(parent)){
                    balance_tree(parent);
                    return 0;
                }
                return 1;
            }else{
                return 0;
            }
        }
    }else if(compareValue < 0){
        if(parent->left == NULL){
            parent->left = node;
            node->parent = parent;
            parent->leftHeight ++;
            if(parent ->right == NULL){
                return 1;
            }else{
                return 0;
            }
        }else{
            if(insert_tree_node_lr(node , parent->left)){
                parent->leftHeight ++;
                if(test_balance_lr(parent)){
                    balance_tree(parent);
                    return 0;
                }
                return 1;
            }else{
                return 0;
            }
        }
    }else if(compareValue == 0){
        printf("node all ready exist!\n");
        doWhenNodeExistLR(node , parent);
    }
}

void switch_node_to_balance_lr(TreeNodeLR * nodelow , TreeNodeLR * nodehigh){
    if(nodelow->parent != NULL){
        if(nodelow->parent->left == nodelow){
            nodelow->parent->left = NULL;
        }else{
            nodelow->parent->right = NULL;
        }
    }
    nodelow->parent = nodehigh->parent;
    if(nodelow->parent == NULL){
        leftRightTreeInfo->root = nodelow;
    }else{
        nodehigh == nodehigh->parent->left ? (nodehigh->parent->left = nodelow) : (nodehigh->parent->right = nodelow);    
    }
    TreeNodeLR * temp = nodelow->left == NULL ? nodelow->right : nodelow->left;
    int compareHighLowValue = compareTreeNodeLR(nodehigh , nodelow);
    if(compareHighLowValue < 0){
        nodelow->left = nodehigh;
        nodehigh->parent = nodelow;
        if(nodehigh->right != NULL ){
            nodelow->right = nodehigh->right;
            nodehigh->right->parent = nodelow;
        }
        nodehigh->right = NULL;
    }else{
        nodelow->right = nodehigh;
        nodehigh->parent = nodelow;
        if(nodehigh->left != NULL){
            nodelow->left = nodehigh->left;
            nodehigh->left->parent = nodelow;
        }
        nodehigh->left = NULL;
    }
    if(temp != NULL && nodelow->left != temp && nodelow->right != temp){
        printf("error\n");
        insert_tree_node_lr(temp , nodelow);
    }
    update_tree_node_height_lr(leftRightTreeInfo->root , 0);
    if(test_balance_lr(nodelow->left)){
        balance_tree(nodelow->left);
    }
    if(test_balance_lr(nodelow->right)){
        balance_tree(nodelow->right);
    }
}

void insertTreeLR(void *nodeNum){
    TreeNodeLR *node = create_tree_node_lr(nodeNum);
    if(leftRightTreeInfo == NULL){
        init_tree_lr(node);
    }else{
        if(leftRightTreeInfo->root == NULL){
            leftRightTreeInfo->root = node;
        }else{
            insert_tree_node_lr(node , leftRightTreeInfo->root);
        }
    }
}

TreeNodeLR * combind(TreeNodeLR *left , TreeNodeLR *right , int leftOrRight){
    if(left == NULL || right == NULL){
        return left == NULL ? right:left;
    }
    printf("combind %d %d\n",getNodeValueLR(left->nodeNum),getNodeValueLR(right->nodeNum));
    TreeNodeLR * temp;
    if(leftOrRight == 1){ 
        temp = get_max_node_lr(left);
        temp->parent->right = NULL;
        if(temp->left != NULL && temp != left){
            temp->left->parent = temp->parent;
            temp->parent->right = temp->left;
        }
        if(temp != left){
            temp->left = left;
            left->parent = temp;
        }
        temp->right = right;
        right->parent = temp;
    }else{
        temp = get_min_node_lr(right);
        temp ->parent->left = NULL;
        if(temp->right != NULL && temp != right){
            temp->right->parent = temp->parent;
            temp->parent->left = temp->right;
        }
        if(temp != right){
            temp->right = right;
            right->parent = temp;
        }
        temp->left = left;
        left->parent = temp;
    }
    if(temp !=right && temp !=left){
        update_tree_node_height_lr(temp->parent , 0);
        if(test_balance_lr(temp->parent))
            balance_tree(temp->parent);

    }else{
        update_tree_node_height_lr(temp , 0);
        if(test_balance_lr(temp)){
            balance_tree(temp);
        }
    }
    return temp;
}

void replace_node_lr(TreeNodeLR *node1 , TreeNodeLR *node2){
    if(node1->parent != NULL){
        if(node1->parent->left == node1){
            node1->parent->left = node2;
        }else{
            node1->parent->right = node2;
        }
        node2->parent = node1->parent;
    }else{
        node2->parent = NULL;
        leftRightTreeInfo->root = node2;
    }
}

TreeNodeLR * get_unBalance_lr(TreeNodeLR * node){
    printf("find node %d , %d , %d\n", getNodeValueLR(node->nodeNum) ,node->leftHeight , node->rightHeight);
    if(test_balance_lr(node)){
        return node;
    }
    if(node->left != NULL && get_unBalance_lr(node->left) != NULL){
        return get_unBalance_lr(node->left);
    }
    if(node->right != NULL && get_unBalance_lr(node->right) != NULL){
        return get_unBalance_lr(node->right);
    }
    return NULL;
}

void deleteNodeLR(void * number){
    if(leftRightTreeInfo == NULL){
        printf("tree is empty\n");
        return;
    }
    TreeNodeLR *node = find_tree_node_lr(leftRightTreeInfo->root , number);
    if(node == NULL){
        printf("not found\n");
        return;
    }
    int leftOrRight = node->leftHeight > node->rightHeight ? 1 : 0;
    TreeNodeLR *temp = combind(node->left , node->right , leftOrRight);
    TreeNodeLR * unBalance = NULL;
    if(temp == NULL){
        if(node->parent == NULL){
            leftRightTreeInfo->root = NULL;
        }else if(node->parent->left == node){
            node->parent->left = NULL;
        }else{
            node->parent->right = NULL;
        }
        if(node->parent != NULL){
            update_tree_node_height_lr(leftRightTreeInfo->root , 0);
            //showTreeLR(root);
            while((unBalance = get_unBalance_lr(leftRightTreeInfo->root)) != NULL){
                printf("balance node %d\n",getNodeValueLR(unBalance->nodeNum));
                balance_tree(unBalance);
            }
        }
        free(node);
        return;
    }
    replace_node_lr(node , temp);
    update_tree_node_height_lr(leftRightTreeInfo->root , 0);
    while((unBalance = get_unBalance_lr(leftRightTreeInfo->root)) != NULL){
        printf("balance node %d\n",getNodeValueLR(unBalance->nodeNum));
        balance_tree(unBalance);
    }
    free(node);
}

void *findElementLR(void *key){
    TreeNodeLR * temp = find_tree_node_lr(leftRightTreeInfo->root , key);
    if(temp == NULL){
        return NULL;
    }else{
        return temp->nodeNum;
    }
}

void destroy_tree_lr(TreeNodeLR * node){
    if(node->left != NULL){
        destroy_tree_lr(node->left);
    }
    if(node->right != NULL){
        destroy_tree_lr(node->right);
    }
    free(node);
}

void emptyLeftRightTreeLR(){
    if(leftRightTreeInfo == NULL || leftRightTreeInfo->root == NULL){
        return;
    }
    destroy_tree_lr(leftRightTreeInfo->root);
    leftRightTreeInfo->root = NULL;
}

int main(){
    int n = 0;
    scanf("%d" , &n);
    for(int i = 0 ; i < n ; i ++){
        int * p = malloc(sizeof(int));
        *p = rand() %10000;
        insertTreeLR(p);
    }
    showTreeLR(leftRightTreeInfo->root);
    show_tree_order_middle_LR(leftRightTreeInfo->root);
    show_tree_order_back_LR(leftRightTreeInfo->root);
}
