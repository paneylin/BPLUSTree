#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const char RED = 'R';
const char BLACK = 'B';
const int DERECT_INSERT_STATE = 1;
const int INSERT_ROTATION_STATE = 2;
const int INSERT_NEED_PUSH_STATE = 3;

const int DERECT_DELETE = 1;
const int REBALANCE_PUSH = 2;
const int BALANCE_SELF = 3;
//队列
typedef struct TreeNodeRB{
    void *data;
    char color;
    struct TreeNodeRB *parent;
    struct TreeNodeRB *left;
    struct TreeNodeRB *right;
}TreeNodeRB;


typedef struct TreeRB{
    TreeNodeRB *root;
}TreeRB;

void insertElementRBTree(TreeRB *tree, void *data);
void showTreeRB(TreeRB *tree);
int compareVlaueRBTree(void *a, void *b){
    return *(int *)a - *(int *)b;
}
int count = 0;

void show_tree_node_RB(TreeNodeRB *node){
    if(node == NULL){
        return;
    }
    printf("(%d %c)  :", *(int *)node->data, node->color);
    if(node->left != NULL){
        printf("L : (%d %c) ;" , *(int *)node->left->data, node->left->color);
    }
    if(node->right != NULL){
        printf("R : (%d %c) ;" , *(int *)node->right->data, node->right->color);
    }
    if(node->parent != NULL){
        printf("P : (%d %c) ;" , *(int *)node->parent->data, node->parent->color);
    }
    count ++;
    printf("\n");
    show_tree_node_RB(node->left);
    show_tree_node_RB(node->right);
}

void showTreeRB(TreeRB *tree){
    show_tree_node_RB(tree->root);
    printf("count = %d\n", count);
}

TreeRB *createTreeRB(){
    TreeRB *tree = (TreeRB *)malloc(sizeof(TreeRB));
    tree->root = NULL;
    return tree;
}

void destroy_node_RB(TreeNodeRB *node){
    if(node == NULL){
        return;
    }
    free(node->data);
    free(node);
}

TreeNodeRB *create_tree_nodeRB(void *data,char color){
    TreeNodeRB *newNode = (TreeNodeRB *)malloc(sizeof(TreeNodeRB));
    newNode->data = data;
    newNode->color = color;
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

int validRed_RB(TreeNodeRB *node){
    return node->color == RED;
}

int get_inserted_node_state(TreeNodeRB *node){
    if(!validRed_RB(node)){
        return DERECT_INSERT_STATE;
    }else{
        if(node->parent == NULL){
            printf("error: root is red\n");
            return 0;
        }
        return compareVlaueRBTree(node->data,node->parent->data) < 0 ? 
            ((node->parent->right == NULL|| node->parent->right->color == BLACK) ? INSERT_ROTATION_STATE : INSERT_NEED_PUSH_STATE)
            :((node->parent->left == NULL || node->parent->left->color == BLACK) ? INSERT_ROTATION_STATE : INSERT_NEED_PUSH_STATE); 
    }
}

void generate_three_node_basic_branch_RB(TreeNodeRB *nodeSmall , TreeNodeRB * nodeMiddle , TreeNodeRB * nodeBig){
    nodeMiddle->left = nodeSmall;
    nodeMiddle->right = nodeBig;
    nodeSmall->parent = nodeMiddle;
    nodeBig->parent = nodeMiddle;
    nodeMiddle->color = BLACK;
    nodeSmall->color = RED;
    nodeBig->color = RED;
}

void gen_node_parent_relation_RB(TreeNodeRB *targetNode , TreeNodeRB *sourceNode , TreeRB * tree){
    sourceNode->parent = targetNode->parent;
    if(sourceNode->parent == NULL){
        tree->root = sourceNode;
    }else if(compareVlaueRBTree(sourceNode->data,sourceNode->parent->data) < 0){
        sourceNode->parent->left = sourceNode;
    }else{
        sourceNode->parent->right = sourceNode;
    }
    targetNode->parent = NULL;
}

void balance_tree_node_RB(TreeNodeRB *parentNode, TreeNodeRB *childNode ,TreeRB * tree){
    int state = get_inserted_node_state(parentNode);
    if(state == DERECT_INSERT_STATE){
        childNode->color = RED;
    }
    if(state == INSERT_ROTATION_STATE){
        TreeNodeRB *grandParent = parentNode->parent;
        if(compareVlaueRBTree(parentNode->data,grandParent->data) < 0){
            if(compareVlaueRBTree(childNode->data,parentNode->data) < 0){
                gen_node_parent_relation_RB(grandParent,parentNode,tree);
                grandParent->left = parentNode->right;
                grandParent->left->parent = grandParent;
                generate_three_node_basic_branch_RB(childNode,parentNode,grandParent);
            }else{
                gen_node_parent_relation_RB(grandParent,childNode,tree);
                grandParent->left = childNode->right;
                grandParent->left->parent = grandParent;
                parentNode->right = childNode->left;
                parentNode->right->parent = parentNode;
                generate_three_node_basic_branch_RB(parentNode,childNode,grandParent);
            }
        }else{
            if(compareVlaueRBTree(childNode->data,parentNode->data) < 0){
                gen_node_parent_relation_RB(grandParent,childNode,tree);
                grandParent->right = childNode->left;
                grandParent->right->parent = grandParent;
                parentNode->left = childNode->right;
                parentNode->left->parent = parentNode;
                generate_three_node_basic_branch_RB(grandParent,childNode,parentNode);
            }else{
                gen_node_parent_relation_RB(grandParent,parentNode,tree);
                grandParent->right = parentNode->left;
                grandParent->right->parent = grandParent;
                generate_three_node_basic_branch_RB(grandParent,parentNode,childNode);
            }
        }
    }
    if(state == INSERT_NEED_PUSH_STATE){
        TreeNodeRB *grandParent = parentNode->parent;
        TreeNodeRB *oldGrandParent = grandParent->parent;
        grandParent->left ->color = BLACK;
        grandParent->right->color = BLACK;
        childNode->color = RED;
        if(oldGrandParent == NULL){
            return;
        }else{
            balance_tree_node_RB(oldGrandParent,grandParent,tree);
        } 
    } 
}

void push_node_to_tree_node_RB(TreeNodeRB *toBeInsertNode, TreeNodeRB *insertNode ,TreeRB * tree){
    //printf("%d %d\n", *(int *)toBeInsertNode->data, *(int *)insertNode->data);
    int state = get_inserted_node_state(toBeInsertNode);
    if(*(int *)insertNode->data == 5705){
        printf("%d\n",state);
    }
    if(state == DERECT_INSERT_STATE){
        compareVlaueRBTree(insertNode->data,toBeInsertNode->data) < 0 ? (toBeInsertNode->left = insertNode ,toBeInsertNode->left->parent = toBeInsertNode) 
            : (toBeInsertNode->right = insertNode , toBeInsertNode->right->parent = toBeInsertNode); 
    }
    if(state == INSERT_ROTATION_STATE){
        TreeNodeRB *parent = toBeInsertNode->parent;
        if(compareVlaueRBTree(toBeInsertNode->data,parent->data) < 0){
            if(compareVlaueRBTree(insertNode->data,toBeInsertNode->data) < 0){
                gen_node_parent_relation_RB(parent,toBeInsertNode,tree);
                generate_three_node_basic_branch_RB(insertNode,toBeInsertNode,parent);
                parent->left = parent->right = NULL;
            }else{
                gen_node_parent_relation_RB(parent,insertNode,tree);
                generate_three_node_basic_branch_RB(toBeInsertNode,insertNode,parent);
                parent->left = parent->right = NULL;
                toBeInsertNode->left = toBeInsertNode->right = NULL;
            }
        }else{
            if(compareVlaueRBTree(insertNode->data,toBeInsertNode->data) < 0){
                gen_node_parent_relation_RB(parent,insertNode,tree);
                generate_three_node_basic_branch_RB(parent,insertNode,toBeInsertNode);
                parent->left = parent->right = NULL;
                toBeInsertNode->left = toBeInsertNode->right = NULL;
            }else{
                gen_node_parent_relation_RB(parent,toBeInsertNode,tree);
                generate_three_node_basic_branch_RB(parent,toBeInsertNode,insertNode);
                parent->left = parent->right = NULL;
            }
        }
    }
    if(state == INSERT_NEED_PUSH_STATE){
        TreeNodeRB *parent = toBeInsertNode->parent;
        TreeNodeRB *grandParent = parent->parent;
        parent->left ->color = BLACK;
        parent->right->color = BLACK;
        compareVlaueRBTree(insertNode->data,toBeInsertNode->data) < 0 ? (toBeInsertNode->left = insertNode , toBeInsertNode->left->parent = toBeInsertNode) 
            : (toBeInsertNode->right = insertNode , toBeInsertNode->right->parent = toBeInsertNode);
        if(grandParent == NULL){
            return;
        }else{
            parent->color = RED;
            balance_tree_node_RB(grandParent,parent,tree);
        }        
    }
}


void insert_node_to_tree_RB(TreeNodeRB *parent, TreeNodeRB *node , TreeRB * tree){
    int compareValue = compareVlaueRBTree(node->data,parent->data);
    //printf("%d\n", compareValue);
    if(compareValue < 0){
        if(parent->left == NULL){
            push_node_to_tree_node_RB(parent, node , tree);
        }else{
            insert_node_to_tree_RB(parent->left, node , tree);
        }
    }else if(compareValue > 0){
        if(parent->right == NULL){
            push_node_to_tree_node_RB(parent, node , tree);
        }else{
            insert_node_to_tree_RB(parent->right, node , tree);
        }
    }else if(compareValue == 0){
        printf("the node is already in the tree\n");
    }
}

void insertElementRBTree(TreeRB *tree , void *data){
    if(tree == NULL){
        printf("tree is NULL\n");
        return;
    }
    if(tree->root == NULL){
        tree->root = create_tree_nodeRB(data,BLACK);
        return;
    }
    TreeNodeRB *cur = create_tree_nodeRB(data,RED);
    insert_node_to_tree_RB(tree->root,cur,tree);
}

TreeNodeRB * find_node_from_node_RB(TreeNodeRB *node , void *data){
    if(node == NULL){
        printf("not find ,node is NULL\n");
        return NULL;
    }
    TreeNodeRB * cur = node;
    while(cur != NULL){
        int compareValue = compareVlaueRBTree(data , cur->data);
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

TreeNodeRB * find_node_in_tree_RB(void *data , TreeRB * tree){
    if(tree == NULL){
        printf("tree is NULL\n");
        return NULL;
    }
    if(tree->root == NULL){
        printf("not find ,tree is empty\n");
        return NULL;
    }
    return find_node_from_node_RB(tree->root,data);
}

void * findElementRBTree(TreeRB *tree , void *data){
    if(tree == NULL){
        printf("tree is NULL\n");
        return NULL;
    }
    if(tree->root == NULL){
        printf("tree is empty\n");
        return NULL;
    }
    TreeNodeRB *node = find_node_in_tree_RB(data,tree);
    if(node != NULL){
        return node->data;
    }
    return NULL;
}

TreeNodeRB * get_max_value_node_in_node_RB(TreeNodeRB * node){
    if(node == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    TreeNodeRB * rsl = node;
    while(rsl != NULL){
        if(rsl->right == NULL){
            return rsl;
        }
        rsl = rsl->right;
    }
}

TreeNodeRB * get_min_value_node_in_node_RB(TreeNodeRB * node){
    if(node == NULL){
        printf("node is NULL\n");
        return NULL;
    }
    TreeNodeRB * rsl = node;
    while(rsl != NULL){
        if(rsl->left == NULL){
            return rsl;
        }
        rsl = rsl->left;
    }
}

int valid_min_node_RB(TreeNodeRB * node){
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

int get_small_node_delate_state_RB(TreeNodeRB * node){
    if(node == NULL){
        printf("node is NULL\n");
        return -1;
    }
    if(!valid_min_node_RB(node)){
        printf("not fit function\n");
        return 0;
    }
    if(validRed_RB(node)){
        if(node->left == NULL){
            return DERECT_DELETE;
        }else{
            return BALANCE_SELF;
        }
    }
    if(!validRed_RB(node)){ 
        if(node->right == NULL && node->left == NULL){
            return REBALANCE_PUSH;
        }else if(node->right != NULL && node->left != NULL){
            if(!validRed_RB(node->right) && !validRed_RB(node->left)){
                return REBALANCE_PUSH;
            }
            return BALANCE_SELF;
        }else{
            return BALANCE_SELF;
        }
    }
}

void delete_node_from_parent_RB(TreeNodeRB *node , TreeRB *tree){
    if(node == NULL){
        printf("node is NULL\n");
        return;
    }
    if(node->parent == NULL){
        printf("error root is red\n");
        return;
    }
    if(node->parent->left == node){
        node->parent->left = NULL;
    }else if(node->parent->right == node){
        node->parent->right = NULL;
    }else{
        printf("relationship error\n");
    }
}

void balance_small_node_self_RB(TreeNodeRB *node , TreeRB *tree){
    if(node == NULL){
        printf("node is NULL\n");
        return;
    }
    if(node->right != NULL){
        gen_node_parent_relation_RB(node,node->right , tree);
        node->right->color = BLACK;
        if(node->left != NULL){
            node->left->color = RED;
            node->right->left = node->left;
            node->right->left->parent = node->right;
        }
    }else if(node->left != NULL){
        gen_node_parent_relation_RB(node,node->left , tree);
        node->left->color = BLACK;
    }
}

int judge_red_blanch_RB(TreeNodeRB * node){
    return node != NULL && validRed_RB(node);
}

int combind_two_node_small_part_RB(TreeNodeRB *toBeCombindnode , TreeNodeRB *combindNode , TreeRB *tree){
    if(toBeCombindnode == NULL || combindNode == NULL){
        printf("node is NULL\n");
        return 0;
    }
    if(judge_red_blanch_RB(toBeCombindnode->right) && judge_red_blanch_RB(toBeCombindnode->left)){
        toBeCombindnode->right->color = BLACK;
        toBeCombindnode->left->color = BLACK;
        combindNode->color = RED;
        if(compareVlaueRBTree(toBeCombindnode->data,combindNode->data) < 0){
            TreeNodeRB * temp = toBeCombindnode->right->right;
            toBeCombindnode->right->right = combindNode;
            combindNode->parent = toBeCombindnode->right;
            combindNode->left = temp;
            temp != NULL ? temp->parent = combindNode : 0;
        }else{
            TreeNodeRB * temp = toBeCombindnode->left->left;
            toBeCombindnode->left->left = combindNode;
            combindNode->parent = toBeCombindnode->left;
            combindNode->right = temp;
            temp != NULL ? temp->parent = combindNode : 0;
        }
        return 1;
    }else{
        if(compareVlaueRBTree(toBeCombindnode->data,combindNode->data) < 0){
            if(judge_red_blanch_RB(toBeCombindnode->right)){
                TreeNodeRB *middle = toBeCombindnode->right;
                gen_node_parent_relation_RB(toBeCombindnode , middle , tree);
                toBeCombindnode->right = middle->left;
                toBeCombindnode->right != NULL ? toBeCombindnode->right->parent = toBeCombindnode : 0;
                combindNode->left = middle->right;
                combindNode->left != NULL ? combindNode->left->parent = combindNode : 0;
                generate_three_node_basic_branch_RB(toBeCombindnode, middle , combindNode);
                toBeCombindnode = middle;
            }else{
                combindNode->left = toBeCombindnode->right;
                combindNode->left != NULL ? combindNode->left->parent = combindNode : 0;
                toBeCombindnode->right = combindNode;
                toBeCombindnode->right != NULL ? toBeCombindnode->right->parent = toBeCombindnode : 0;
            }
        }else{
            if(judge_red_blanch_RB(toBeCombindnode->left)){
                TreeNodeRB *middle = toBeCombindnode->left;
                gen_node_parent_relation_RB(toBeCombindnode , middle , tree);
                toBeCombindnode->left = middle->right;
                toBeCombindnode->left != NULL ? toBeCombindnode->left->parent = toBeCombindnode : 0;
                combindNode->right = middle->left;
                combindNode->right != NULL ? combindNode->right->parent = combindNode : 0;
                generate_three_node_basic_branch_RB(combindNode, middle , toBeCombindnode);
                toBeCombindnode = middle;
            }else{
                combindNode->right = toBeCombindnode->left;
                combindNode->right != NULL ? combindNode->right->parent = combindNode : 0;
                toBeCombindnode->left = combindNode;
                toBeCombindnode->left != NULL ? toBeCombindnode->left->parent = toBeCombindnode : 0;
            }
        }
        return 0;
    }
}

void balance_push_node_with_brother_RB(TreeNodeRB *node , TreeRB *tree){
   if(node->parent == NULL){
       return;
   }
   int brotherInLeftOrRight = 1;
   TreeNodeRB *parent = node->parent;
   TreeNodeRB *brother = (parent->left == NULL || parent->left == node) ? (brotherInLeftOrRight = 0, node->parent->right) : node->parent->left;
   if(brotherInLeftOrRight){
       brother->parent->left = NULL;
       brother->parent = NULL;
   }else{
       brother->parent->right = NULL;
       brother->parent = NULL;
   }
   if(brother == NULL){
       printf("brother is NULL , tree error\n");
       return;
   }
   if(validRed_RB(brother)){
       brother->color = BLACK;
       gen_node_parent_relation_RB(parent,brother,tree);  
       TreeNodeRB * combindNode = brotherInLeftOrRight ? brother -> right : brother ->left;  
       if(combind_two_node_small_part_RB(combindNode , parent , tree)){
           combindNode -> color = RED;
       }
       return;
   }else{
        gen_node_parent_relation_RB(parent,brother,tree);
        if(validRed_RB(parent)){
            parent->color = BLACK;            
            if(combind_two_node_small_part_RB(brother,parent,tree)){
                brother->color = RED;
            }
            return;
        }
        if(combind_two_node_small_part_RB(brother,parent,tree)){
            return;
        }else{
            balance_push_node_with_brother_RB(brother,tree);
        }       
   }
}

void balance_small_node_Push_RB(TreeNodeRB *node , TreeRB *tree){
    if(node == NULL){
        printf("node is NULL\n");
        return;
    }
    TreeNodeRB *pushNode = node;
    pushNode->color = RED;
    if(node->left != NULL){
        gen_node_parent_relation_RB(node,node->left , tree);
        node->left->color = BLACK;
        pushNode = node->left;
        if(node->right != NULL){
            node->right->color = RED;
            node->left->right = node->right;
            node->left->right->parent = node->left;
        }
    }
    if(pushNode->parent == NULL){
        if(validRed_RB(pushNode)){
            tree->root = NULL;
        }else{
            tree->root = pushNode;
        }
        return;
    }
    pushNode == node ? pushNode == pushNode ->parent->left ? (pushNode->parent->left = NULL) : (pushNode->parent->right = NULL):0;
    balance_push_node_with_brother_RB(pushNode , tree);
}

void balance_small_node_part_RB(TreeNodeRB * node , TreeRB *tree){
    if(node == NULL){
        printf("node is NULL\n");
        return;
    }
    int state = get_small_node_delate_state_RB(node);
    if(state == DERECT_DELETE){
        delete_node_from_parent_RB(node,tree);
    }
    if(state == BALANCE_SELF){
        balance_small_node_self_RB(node,tree);
    }
    if(state == REBALANCE_PUSH){
        balance_small_node_Push_RB(node,tree);
    }
}


int deleteElementRBTree(TreeRB *tree , void *data){
    TreeNodeRB * node = find_node_in_tree_RB(data,tree);
    if(node == NULL){
        printf("not find ,delate failed\n");
        return 0;
    }
    if(valid_min_node_RB(node)){
        balance_small_node_part_RB(node,tree);
        destroy_node_RB(node);
        return 1;
    }
    TreeNodeRB *exChangeNode = get_max_value_node_in_node_RB(node->left);
    //printf("exchangeNode is %d" , *(int *)exChangeNode->data);
    balance_small_node_part_RB(exChangeNode,tree);
    gen_node_parent_relation_RB(node,exChangeNode,tree);
    exChangeNode->left = node->left;
    exChangeNode->right = node->right;
    exChangeNode->left->parent = exChangeNode;
    exChangeNode->right->parent = exChangeNode;
    destroy_node_RB(node);
    return 1;
}

int main(){
    TreeRB *tree = createTreeRB();
    int number = 0;
    scanf("%d",&number);
    for(int i = 0;i < number;i++){
        int *key = (int*)malloc(sizeof(int)); 
        *key = rand() % 10000;
        printf("%d ",*key);
        insertElementRBTree(tree , key);
    }
    printf("\n");
    showTreeRB(tree);
    while(1){
        int *key = (int*)malloc(sizeof(int)); 
        scanf("%d",key);
        deleteElementRBTree(tree,key);
        showTreeRB(tree);
    }
}
