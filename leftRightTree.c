int compare_function_default_LRTree(void *key1 , void *key2){
    return *(int *)key1 - *(int *)key2;
}

void *get_key_function_default_LRTree(void *data){
    return data;
}

void setFreeKeyFuncLRTree(void (*freeKeyFunc)(void * key) , TreeLRTree * tree){
    tree->freeKeyFunc = freeKeyFunc;
}

void setFreeDataFuncLRTree(void (*freeDataFunc)(void * data) , TreeLRTree * tree){
    tree->freeDataFunc = freeDataFunc;
}

int balance_function_default_LRTree(TreeNodeLRTree *node , double balanceFactor){
    if(node->left != NULL){
        if(node->left->size > node->size * balanceFactor){
            return 0;
        }
    }
    if(node->right != NULL){
        if(node->right->size > node->size * balanceFactor){
            return 0;
        }
    }
    return 1;
}

void * get_key_from_node_LRtree(TreeNodeLRTree * node){
    return node->key;
}

void* get_data_from_node_LRtree(TreeNodeLRTree * node){
    return node->data;
}

TreeNodeLRTree * get_node_from_node_LRtree(TreeNodeLRTree * node){
    return node;
}

TreeLRTree *createTreeLRTree(int (*compareFunc)(void * , void *) , void *(*getKeyFunc)(void*)){
    TreeLRTree * tree = (TreeLRTree*)malloc(sizeof(TreeLRTree));
    tree->root = NULL;
    tree->compareFunc = compareFunc;
    if(compareFunc == NULL){
        tree->compareFunc = compare_function_default_LRTree;
    }
    tree->getKeyFunc = getKeyFunc;
    if(getKeyFunc == NULL){
        tree->getKeyFunc = get_key_function_default_LRTree;
    }
    tree->freeKeyFunc = NULL;
    tree->freeDataFunc = NULL;
    tree->balanceFactor = DEFAULT_BALANCE_FACTOR_LRTREE;
} 

TreeNodeLRTree * create_tree_node_LRTree(void * key ,void * data , TreeLRTree * tree){
    TreeNodeLRTree * node = (TreeNodeLRTree *)malloc(sizeof(TreeNodeLRTree));
    node->parent = node->left = node->right = NULL;
    node->key = key;
    node->data = data;
    node->key = tree->getKeyFunc(data);
    node->size = 1;
    return node;
}

int key_compare_LRTree(void * key1 , void * key2 , TreeLRTree * tree){
    return tree->compareFunc(key1 , key2);
}

void destroy_tree_node_LRTree(TreeNodeLRTree * node , TreeLRTree * tree){
    node->parent = node->left = node->right = NULL;
    if(tree->freeKeyFunc != NULL){
        tree->freeKeyFunc(node->key);
        node->key = NULL;
    }
    if(tree->freeDataFunc != NULL){
        tree->freeDataFunc(node->data);
        node->data = NULL;
    }
    free(node);
    node = NULL;
}

void increase_size_LRTree(TreeNodeLRTree * node){
    while(node != NULL){
        node->size++;
        node = node->parent;
    }
}

void reduce_size_LRTree(TreeNodeLRTree * node){
    while(node != NULL){
        node->size--;
        node = node->parent;
    }
}

TreeNodeLRTree *check_balance_LRTree(TreeLRTree * tree){
    TreeNodeLRTree *preOp = NULL;
    TreeNodeLRTree *currentOP = tree->root;
    while(currentOP != NULL){
        if(preOp == NULL || currentOP->parent == preOp){
            if(!balance_function_default_LRTree(currentOP , tree->balanceFactor)){
                return currentOP;
            }
            preOp = currentOP;
            if(currentOP->left != NULL || currentOP->right != NULL){
                currentOP = currentOP->left == NULL ? currentOP->right :currentOP->left;
            }else{
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
        }else if(preOp == currentOP->right){
            preOp = currentOP;
            currentOP = currentOP->parent;
        }
    } 
    return NULL;
}

void setBalanceFactorLRTree(TreeLRTree *tree , double balanceFactor){
    tree->balanceFactor = balanceFactor;
    TreeNodeLRTree * node = NULL;
    while((node = check_balance_LRTree(tree)) != NULL){
        balance_current_node_LRTree(node , tree);
    }
}

TreeNodeLRTree *create_new_balance_tree_LRTree(ArrayList * listData , int start , int end){
    TreeNodeLRTree * root , * left , * right;
    if(start == end){
        root =  getElementByIndexAList(start , listData);
        root->left = root->right = NULL;
        root->size  = 1;
        return root;
    }
    if(end - start <= 2){
        left = getElementByIndexAList(start , listData);
        left->left = left->right = NULL;
        left->size = 1;
        root = getElementByIndexAList(start + 1 , listData);
        root->left = root->right = NULL;
        right = end == start + 2 ? getElementByIndexAList(end , listData) : NULL;
        if(right != NULL){
            right->left = right->right = NULL;
            right->size = 1;
        }
    }else{
        int mid = (start + end) / 2;
        //printf("mid = %d , start = %d , end = %d\n" , mid , start , end);
        root = getElementByIndexAList(mid , listData);
        left = create_new_balance_tree_LRTree(listData , start , mid - 1);
        right = create_new_balance_tree_LRTree(listData , mid + 1 , end);
    }
    root->left = left;
    left->parent = root;
    if(right != NULL){
        root->right = right;
        right->parent = root;
    }
    root->size = end - start + 1;
    return root;
}

ArrayList * get_tree_datas_left_to_right_LRTree(TreeNodeLRTree * node ,void * (*getNodeDataFunc)(void *)){
    if(node == NULL){
        return NULL;
    }
    TreeNodeLRTree * curNode = node;
    TreeNodeLRTree * preNode = node->parent;
    ArrayList *list = createArrayListAList(NULL);
    while(curNode != node->parent){
        if(curNode->parent == preNode){
            if(curNode->left != NULL){
                preNode = curNode;
                curNode = curNode->left;
            }else{
                insertElementAList(getNodeDataFunc(curNode), list);
                if(curNode->right != NULL){
                    preNode = curNode;
                    curNode = curNode->right;
                }else{
                    preNode = curNode;
                    curNode = curNode->parent;
                }
            }
        }else if(preNode->parent == curNode){
            if(curNode->left == preNode){
                insertElementAList(getNodeDataFunc(curNode) , list);
                if(curNode->right != NULL){
                    preNode = curNode;
                    curNode = curNode->right;
                }else{
                    preNode = curNode;
                    curNode = curNode->parent;
                }
            }else if(curNode->right = preNode){
                preNode = curNode;
                curNode = curNode->parent;
            }
        }
    }
    return list;
}

void balance_current_node_LRTree(TreeNodeLRTree *node , TreeLRTree *tree){
    ArrayList * listDatas = get_tree_datas_left_to_right_LRTree(node , get_node_from_node_LRtree);
    int dataSize = getSizeAList(listDatas);
    TreeNodeLRTree *pNode = node->parent;
    TreeNodeLRTree * newRoot = create_new_balance_tree_LRTree(listDatas , 0 , dataSize - 1);
    if(pNode == NULL){
        tree->root = newRoot;
        newRoot->parent = NULL;
        return;
    }
    if(pNode->left == node){
        pNode->left = newRoot; 
    }else{
        pNode->right = newRoot;
    }
    newRoot->parent = pNode;
}

void insertDataLRTree(void * data , TreeLRTree * tree){
    if(data == NULL){
        printf("error , insert data is null\n");
        return;
    }
    if(tree == NULL){
        printf("error , tree is null\n");
        return;
    }
    void * key = tree->getKeyFunc(data);
    TreeNodeLRTree *insertNode = create_tree_node_LRTree(key , data , tree);
    if(tree->root == NULL){
        tree->root = insertNode;
        return;
    }
    TreeNodeLRTree *node = tree->root;
    while(node != NULL){
        int value = key_compare_LRTree(key , node->key , tree);
        if(value > 0){
            if(node->right == NULL){
                node->right = insertNode;
                insertNode->parent = node;
                increase_size_LRTree(node);
                break;
            }else{
                node = node->right;
            }
        }else if(value < 0){
            if(node->left == NULL){
                node->left = insertNode;
                insertNode->parent = node;
                increase_size_LRTree(node);
                break;
            }else{
                node = node->left;
            }
        }else{
            destroy_tree_node_LRTree(insertNode , tree);
            printf("key exists , insert fail\n");
            return;
        }
    }
    while((node = check_balance_LRTree(tree)) != NULL){
        balance_current_node_LRTree(node , tree);
    }
}

TreeNodeLRTree * find_node_in_tree_LRTree(void * key , TreeNodeLRTree * node ,TreeLRTree * tree){
    if(node == NULL || key == NULL){
        printf("error , node or key is null\n");
        return NULL;
    }
    TreeNodeLRTree * cur = node;
    while(cur != NULL){
        int value = key_compare_LRTree(key , cur->key , tree);
        if(value > 0){
            cur = cur->right;
        }else if(value < 0){
            cur = cur->left;
        }else{
            return cur;
        }
    }
    return cur;
}

void * getDataFromTreeLRTree(void * key , TreeLRTree * tree){
    if(key == NULL){
        printf("key is null , find error\n");
        return NULL;
    }
    if(tree == NULL || tree->root == NULL){
        return NULL;
    }
    TreeNodeLRTree * node = find_node_in_tree_LRTree(key ,tree->root , tree);
    if(node == NULL){
        return NULL;
    }
    return node->data;
}

void showTreeFunc_LRTree(TreeNodeLRTree * node , void (*opFunc)(void * , void *)){
    opFunc(node->key , node->data);
    printf(" size is %d" , node->size);
    if(node->left != NULL){
        printf("left: ");
        opFunc(node->left->key , node->left->data);
    }
    if(node->right != NULL){
        printf("right: ");
        opFunc(node->right->key , node->right->data);
    }
    printf("\n ");
}


void search_order_middle_op_LRTree(TreeNodeLRTree * node , void (*opFunc)(void * , void *)){
    TreeNodeLRTree *preOp = NULL;
    TreeNodeLRTree *currentOP = node;
    while(currentOP != NULL){
        if(preOp == NULL || currentOP->parent == preOp){
            showTreeFunc_LRTree(currentOP ,opFunc);
            preOp = currentOP;
            if(currentOP->left != NULL || currentOP->right != NULL){
                currentOP = currentOP->left == NULL ? currentOP->right :currentOP->left;
            }else{
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
        }else if(preOp == currentOP->right){
            preOp = currentOP;
            currentOP = currentOP->parent;
        }
    }
}

void ShowTree(TreeLRTree *tree , void (*showNodeFunc)(void * ,void *)){
    TreeNodeLRTree *root = tree->root;
    search_order_middle_op_LRTree(root , showNodeFunc);
}

ArrayList * getTreeAllKeysLeftToRightLRTree(TreeLRTree * tree){
    return get_tree_datas_left_to_right_LRTree(tree->root ,get_key_from_node_LRtree);
}

ArrayList * getTreeAllDatasLeftToRightLRTree(TreeLRTree * tree){
    return get_tree_datas_left_to_right_LRTree(tree->root ,get_data_from_node_LRtree);
}

void destroy_tree_nodes_LRTree(TreeNodeLRTree * node , TreeLRTree * tree){
    if(node == NULL){
        return;
    }
    ArrayList * listNode = get_tree_datas_left_to_right_LRTree(node , get_node_from_node_LRtree);
    int size = getSizeAList(listNode);
    for(int i = 0 ; i < size ; i++){
        destroy_tree_node_LRTree(getElementByIndexAList(i , listNode) , tree);
    }
}

void destroyTreeLRTree(TreeLRTree * tree){
    if(tree == NULL){
        return;
    }
    destroy_tree_nodes_LRTree(tree->root , tree);
    tree->root = NULL;
    free(tree);
    tree = NULL;
}

TreeNodeLRTree *get_min_node_from_tree_LRTree(TreeNodeLRTree *node){
    if(node == NULL){
        return NULL;
    }
    TreeNodeLRTree *cur = node;
    while(cur->left != NULL){
        cur = cur->left;
    }
    return cur;
}

TreeNodeLRTree *get_max_node_from_tree_LRTree(TreeNodeLRTree *node){
    if(node == NULL){
        return NULL;
    }
    TreeNodeLRTree *cur = node;
    while(cur->right != NULL){
        cur = cur->right;
    }
    return cur;
}

void replace_cNode_pNode_LRTree(TreeNodeLRTree * cNode , TreeNodeLRTree * pNode , TreeLRTree * tree){
    if(cNode == NULL || pNode == NULL){
        printf("error , node is null\n");
        return;
    }
    TreeNodeLRTree *gNode = pNode->parent;
    if(gNode == NULL){
        tree->root = cNode;
    }else{
        if(gNode->left == pNode){
            gNode->left = cNode;
        }else{
            gNode->right = cNode;
        }
    }
    cNode->parent = gNode;
    pNode->parent = NULL;
    if(cNode == pNode->left){
        cNode->right = pNode->right;
        pNode->right != NULL ? pNode->right->parent = cNode : 0;
    }else if(cNode == pNode->right){
        cNode->left = pNode->left;
        pNode->left != NULL ? pNode->left->parent = cNode : 0;
    }else{
        cNode->right = pNode->right;
        cNode->left = pNode->left;
        pNode->left != NULL ? pNode->left->parent = cNode : 0;
        pNode->right != NULL ? pNode->right->parent = cNode : 0;
    }
    pNode->left = pNode->right = NULL;
    cNode->size = pNode->size;
}

void delete_node_from_tree_LRTree(TreeNodeLRTree * node , TreeLRTree * tree){
    if(node == NULL){
        return;
    }
    TreeNodeLRTree * pNode = node->parent;
    if(node->left == NULL && node->right == NULL){
        if(pNode == NULL){
            tree->root = NULL;
        }else{
            if(pNode->left == node){
                pNode->left = NULL;
                
            }else{
                pNode->right = NULL;
            }
            reduce_size_LRTree(pNode);
        }
        destroy_tree_node_LRTree(node , tree);
    }else{
        TreeNodeLRTree *cNode = NULL;
        if(node->left == NULL){
            cNode = get_min_node_from_tree_LRTree(node->right);
        }else{
            cNode = get_max_node_from_tree_LRTree(node->left);
        }
        if(cNode == node->right || cNode == node->left){
            replace_cNode_pNode_LRTree(cNode , node , tree);
            reduce_size_LRTree(cNode);
            destroy_tree_node_LRTree(node , tree);
        }else{
            reduce_size_LRTree(cNode);
            if(cNode->left != NULL){
                replace_cNode_pNode_LRTree(cNode->left , cNode , tree);
            }else if(cNode->right != NULL){
                replace_cNode_pNode_LRTree(cNode->right , cNode , tree);
            }else{
                if(cNode->parent->left == cNode){
                    cNode->parent->left = NULL;
                }else{
                    cNode->parent->right = NULL;
                }
            }
            replace_cNode_pNode_LRTree(cNode , node , tree);
            destroy_tree_node_LRTree(node , tree);
        }
    }
    while((node = check_balance_LRTree(tree)) != NULL){
        balance_current_node_LRTree(node , tree);
    }
}

int isEmptyLRTree(TreeLRTree * tree){
    if(tree == NULL){
        printf("tree is null\n");
        return 1;
    }
    return tree->root == NULL;
}

void* deleteElementLRTree(void * key , TreeLRTree * tree){
    if(key == NULL){
        printf("key is null , delete error\n");
        return NULL;
    }
    if(isEmptyLRTree(tree)){
        return NULL;
    }
    TreeNodeLRTree * node = find_node_in_tree_LRTree(key ,tree->root , tree);
    if(node == NULL){
        return NULL;
    }
    void * data = node->data;
    delete_node_from_tree_LRTree(node , tree);
    return data;
}

void *popMinDataLRTree(TreeLRTree * tree){
    if(isEmptyLRTree(tree)){
        printf("not find , tree is null or empty , delete error\n");
        return NULL;
    }
    TreeNodeLRTree * node = get_min_node_from_tree_LRTree(tree->root);
    void * data = node->data;
    delete_node_from_tree_LRTree(node , tree);
    return data;
}

void *popMaxDataLRTree(TreeLRTree * tree){
    if(isEmptyLRTree(tree)){
        printf("not find , tree is null or empty , delete error\n");
        return NULL;
    }
    TreeNodeLRTree * node = get_max_node_from_tree_LRTree(tree->root);
    void * data = node->data;
    delete_node_from_tree_LRTree(node , tree);
    return data;
}

void *getMinDataLRTree(TreeLRTree * tree){
    if(tree == NULL){
        printf("not find , tree is null , find error\n");
        return NULL;
    }
    TreeNodeLRTree * node = get_min_node_from_tree_LRTree(tree->root);
    if(node == NULL){
        return NULL;
    }
    return node->data;
}

void * getMaxDataLRTree(TreeLRTree * tree){
    if(tree == NULL){
        printf("not find , tree is null , find error\n");
        return NULL;
    }
    TreeNodeLRTree * node = get_max_node_from_tree_LRTree(tree->root);
    if(node == NULL){
        return NULL;
    }
    return node->data;
}

int getSizeLRTree(TreeLRTree * tree){
    if(isEmptyLRTree(tree)){
        return 0;
    }
    return tree->root->size;
}