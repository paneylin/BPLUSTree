
int compare_function_default_LRTree(void *key1 , void *key2){
    return *(int *)key1 - *(int *)key2;
}

void *get_key_function_default_LRTree(void *data){
    return data;
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

void setBalanceFactorLRTree(TreeLRTree *tree , double balanceFactor){
    tree->balanceFactor = balanceFactor;
}

TreeLRTree *createTreeLRTree(int (*compareFunc)(void * , void *) , void *(*getKeyFunc)(void*) , void (*freeKeyFunc)(void *)){
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
    tree->freeKeyFunc = freeKeyFunc;
    tree->balanceFactor = 0.7;
} 

TreeNodeLRTree * create_tree_node_LRTree(void * data , TreeLRTree * tree){
    TreeNodeLRTree * node = (TreeNodeLRTree *)malloc(sizeof(TreeNodeLRTree));
    node->parent = node->left = node->right = NULL;
    node->data = data;
    node->key = tree->getKeyFunc(data);
    node->size = 1;
    return node;
}

int key_compare_LRTree(void * key1 , void * key2 , TreeLRTree * tree){
    return tree->compareFunc(key1 , key2);
}

void destroy_tree_node_LRTree(TreeNodeLRTree * node){
    node->parent = node->left = node->right = NULL;
    free(node);
}

void increase_size_LRTree(TreeNodeLRTree * node){
    while(node != NULL){
        node->size++;
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
    TreeNodeLRTree * curNode = node;
    TreeNodeLRTree * preNode = node->parent;
    ArrayList *list = createArrayListAList(NULL , NULL);
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
    if(tree == NULL){
        printf("error , tree is null");
        return;
    }
    if(tree->root == NULL){
        tree->root = create_tree_node_LRTree(data , tree);
        return;
    }
    TreeNodeLRTree *node = tree->root;
    TreeNodeLRTree *insertNode = create_tree_node_LRTree(data , tree);
    while(node != NULL){
        int value = key_compare_LRTree(tree->getKeyFunc(data) , node->key , tree);
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
            destroy_tree_node_LRTree(insertNode);
            break;
        }
    }
    while((node = check_balance_LRTree(tree)) != NULL){
        balance_current_node_LRTree(node , tree);
    }
}

TreeNodeLRTree * find_node_in_tree_LRTree(void * key , TreeNodeLRTree * node ,TreeLRTree * tree){
    if(node == NULL){
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
    if(tree == NULL){
        printf("not find , tree is null");
    }
    TreeNodeLRTree * node = find_node_in_tree_LRTree(key ,tree->root , tree);
    if(node == NULL){
        return NULL;
    }
    return node->data;
}

void search_order_middle_op_LRTree(TreeNodeLRTree * node , void (*opFunc)(void * , void *)){
    TreeNodeLRTree *preOp = NULL;
    TreeNodeLRTree *currentOP = node;
    while(currentOP != NULL){
        if(preOp == NULL || currentOP->parent == preOp){
            opFunc(currentOP->key , currentOP->data);
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

void destroy_tree_nodes_LRTree(TreeNodeLRTree * node){
    ArrayList * listNode = get_tree_datas_left_to_right_LRTree(node , get_node_from_node_LRtree);
    int size = getSizeAList(listNode);
    for(int i = 0 ; i < size ; i++){
        destroy_tree_node_LRTree(getElementByIndexAList(i , listNode));
    }
}

void destroyTreeLRTree(TreeLRTree * tree){
    if(tree == NULL){
        return;
    }
    destroy_tree_node_LRTree(tree->root);
    free(tree);
}