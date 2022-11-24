#include "./fibonacciHeap.h"

int compare_value_default_FHeap(void *data1, void *data2){
    return *(int *)data1 - *(int *)data2;
}

void * getKey_default_FHeap(void *data){
    return data;
}

FHeap *createFHeap(int (*compareFunc)(void * , void *) , void *(*getKeyFunc)(void *)){
    FHeap * heap = (FHeap *)malloc(sizeof(FHeap));
    heap->compareFunc = compareFunc;
    if(compareFunc == NULL){
        heap->compareFunc = compare_value_default_FHeap;
    }
    heap->getKeyFunc = getKeyFunc;
    if(getKeyFunc == NULL){
        heap->getKeyFunc = getKey_default_FHeap;
    }
    heap->freeDataFunc = NULL;
    heap->freeKeyFunc = NULL;
    heap->degreeList = (ArrayList **)malloc(sizeof(ArrayList *) * DEFAULT_DEGREE_SIZE);
    for(int i = 0 ; i < DEFAULT_DEGREE_SIZE ; i++){
        heap->degreeList[i] = createArrayListAList(NULL);
    }
    heap->maxDegree = DEFAULT_DEGREE_SIZE;
    heap->topNode = NULL;
    heap->dgreeSize = 0;
    heap->heapSize = 0;
    return heap;
}

HeapNodeFHeap * create_heap_node_FHeap(void * key ,void *data){
    HeapNodeFHeap * node = (HeapNodeFHeap *)malloc(sizeof(HeapNodeFHeap));
    node->key = key;
    node->data = data;
    node->parent = node->child = node->left = node->right = node->topChild = NULL;
    node->degree = 1;
    return node;
}

void setFreeDataFuncFHeap(void (*freeDataFunc)(void * data) , FHeap * heap){
    heap->freeDataFunc = freeDataFunc;
}

void join_node_to_top_link_FHeap(HeapNodeFHeap * node , FHeap * heap){
    if(heap->topNode == NULL){
        heap->topNode = node;
        heap->dgreeSize = 1;
    }else{
        if(heap->compareFunc(node->key , heap->topNode->key) > 0){
            heap->topNode = node;
        }
    }
    if(heap->dgreeSize < node->degree){
        heap->dgreeSize = node->degree;
    }
    insertElementAList(node , heap->degreeList[node->degree - 1]);
}

void join_node_to_link_FHeap(HeapNodeFHeap * node1 , HeapNodeFHeap * node2){
    if(node1 == NULL || node2 == NULL){
        printf("node1 or node2 is null,join link failed\n");
        return;
    }
    node1->left->right = node2;
    node2->left = node1->left;
    node1->left = node2;
    node2->right = node1;
}

void merge_heap_node_FHeap(HeapNodeFHeap *node1 , HeapNodeFHeap *node2 , FHeap *heap){
    if(node1->child == NULL){
        node1->child = node2;
        node2->left = node2->right = node2;
        node1->topChild = node2;
    }else{
        join_node_to_link_FHeap(node1->child , node2);
        if(heap->compareFunc(node2->key , node1->topChild->key) > 0){
            node1->topChild = node2;
        }
    }
    node2->parent = node1;
    node1->degree++;
    if(node1->degree > heap->dgreeSize){
        heap->dgreeSize = node1->degree;
    }
    insertElementAList(node1 , heap->degreeList[node1->degree - 1]);
}

void keep_degree_only_one_FHeap(FHeap * heap){
    for(int i = 0 ; i < heap->dgreeSize ; i++){
        while(getSizeAList(heap->degreeList[i]) > 1){
            HeapNodeFHeap * node1 = (HeapNodeFHeap *)deleteLastElementAList(heap->degreeList[i]);
            HeapNodeFHeap * node2 = (HeapNodeFHeap *)deleteLastElementAList(heap->degreeList[i]);
            if(heap->compareFunc(node1->key , node2->key) > 0){
                merge_heap_node_FHeap(node1 , node2 , heap);
            }else{
                merge_heap_node_FHeap(node2 , node1 , heap);
            }
        }
    }
}

HeapNodeFHeap *insertElementFHeap(void *data , FHeap * heapInfo){
    if(data == NULL || heapInfo == NULL){
        printf("data or heapInfo is null\n");
        return NULL;
    }
    HeapNodeFHeap * node = create_heap_node_FHeap(heapInfo->getKeyFunc(data) , data);
    join_node_to_top_link_FHeap(node , heapInfo);
    keep_degree_only_one_FHeap(heapInfo);
    heapInfo->heapSize++;
    return node;
}

ArrayList *insertElementsFHeap(void **datas , int size , FHeap * heapInfo){
    if(datas == NULL || heapInfo == NULL){
        printf("data or heapInfo is null\n");
        return;
    }
    ArrayList * list = createArrayListAList(NULL);
    for(int i = 0 ; i < size ; i++){
        HeapNodeFHeap * node = create_heap_node_FHeap(heapInfo->getKeyFunc(datas[i]) , datas[i]);
        join_node_to_top_link_FHeap(node , heapInfo);
        insertElementAList(node , list);
    }
    keep_degree_only_one_FHeap(heapInfo);
    heapInfo->heapSize += size;
    return list;
}

void destroy_Heap_node_keep_data_HHeap(HeapNodeFHeap * node , FHeap * heap){
    if(node == NULL){
        return;
    }
    node->child= node->parent = node->left = node->right = node->topChild = NULL;
    if(heap->freeKeyFunc != NULL){
        heap->freeKeyFunc(node->key);
    }
    node->key = NULL;
    node->data = NULL;
    free(node);
    node = NULL;
}

void destroy_Heap_node_HHeap(HeapNodeFHeap * node , FHeap * heap){
    if(node == NULL){
        return;
    }
    node->child= node->parent = node->left = node->right = node->topChild = NULL;
    if(heap->freeKeyFunc != NULL){
        heap->freeKeyFunc(node->key); 
    }
    node->key = NULL;
    if(heap->freeDataFunc != NULL){
        heap->freeDataFunc(node->data);
    }
    node->data = NULL;
    free(node);
    node = NULL;
}

void re_cal_degree_HHeap(FHeap * heap){
    for(int i = heap->dgreeSize - 2 ; i >= 0 ; i--){
        if(getSizeAList(heap->degreeList[i]) > 0){
            heap->dgreeSize = i + 1;
            break;
        }else{
            heap->dgreeSize = i;
        }
    }
}

void deside_new_top_node_FHeap(FHeap * heap){
    if(heap->dgreeSize == 0){
        heap->topNode = NULL;
    }else{
        for(int i = 0 ; i < heap->dgreeSize ; i++){
            if(getSizeAList(heap->degreeList[i]) > 0){
                if(heap->topNode == NULL){
                    heap->topNode = (HeapNodeFHeap *)getFirstElementAList(heap->degreeList[i]);
                }else{
                    HeapNodeFHeap * node = (HeapNodeFHeap *)getFirstElementAList(heap->degreeList[i]);
                    if(heap->compareFunc(node->key , heap->topNode->key ) > 0){
                        heap->topNode = node;
                    }
                }
            }
        }
    }
}

void * popElementHeapFHeap(FHeap * heap){
    if(heap == NULL || heap->topNode == NULL){
        printf("heap is null or heap is empty\n");
        return NULL;
    }
    HeapNodeFHeap * topNode = heap->topNode;
    heap->topNode = NULL;
    deleteElementAList(topNode , heap->degreeList[topNode->degree - 1]);
    if(topNode->degree == heap->dgreeSize){
        re_cal_degree_HHeap(heap);
    }
    deside_new_top_node_FHeap(heap);
    HeapNodeFHeap * child = topNode->child;
    while(child != NULL){
        child->parent = NULL;
        child->left->right = NULL;
        child->left = NULL;
        join_node_to_top_link_FHeap(child , heap);
        child = child->right;
    }
    keep_degree_only_one_FHeap(heap);
    heap->heapSize--;
    void * data = topNode->data;
    destroy_Heap_node_keep_data_HHeap(topNode , heap);
    return data;
}

void * getElementFHeap(FHeap *heapInfo){
    if(heapInfo == NULL || heapInfo->topNode == NULL){
        printf("heapInfo is null or heapInfo is empty\n");
        return NULL;
    }
    return heapInfo->topNode->data;
}

int getHeapSizeFHeap(FHeap * heap){
    return heap->heapSize;
}

int isEmptyFHeap(FHeap * heap){
    return heap->heapSize == 0;
}

void destroy_heap_tree_HHeap(HeapNodeFHeap * node , FHeap * heap){
    if(node == NULL){
        return;
    }
    HeapNodeFHeap * child = node->child;
    if(child != NULL){
        HeapNodeFHeap * temp = child->right;
        while(temp != node->child){
            child = temp->right;
            destroy_heap_tree_HHeap(temp , heap);
            temp = child;
        }
        destroy_heap_tree_HHeap(temp , heap);
    }
    destroy_Heap_node_HHeap(node , heap);
}

void destoryFHeap(FHeap * heap){
    if(heap == NULL){
        return;
    }
    if(heap->degreeList != NULL){
        for(int i = 0 ; i < heap->maxDegree ; i ++){
            if(getSizeAList(heap->degreeList[i]) > 0){
                destroy_heap_tree_HHeap(getElementAList(0 , heap->degreeList[i]) , heap);
            }
            destroyAList(heap->degreeList[i]);
        }
    }
    heap->topNode = NULL;
    free(heap->degreeList);
    heap->degreeList = NULL;
    free(heap);
    heap = NULL;
}

FHeap * combindHeapsFHeap(FHeap *heap1 , FHeap *heap2){
    if(heap1 == NULL || heap2 == NULL){
        printf("heap1 or heap2 is null\n");
        return NULL;
    }
    if(heap1->getKeyFunc != heap2->getKeyFunc || heap1->compareFunc != heap2->compareFunc){
        printf("heap1 or heap2 is not same type\n");
        return NULL;
    }
    if(heap1->freeDataFunc != heap2->freeDataFunc || heap1->freeKeyFunc != heap2->freeKeyFunc){
        printf("heap1 or heap2 has diffrent freeFunc\n");
        return NULL;
    }
    FHeap * heap = createFHeap(heap1->getKeyFunc , heap1->compareFunc);
    heap->freeDataFunc = heap1->freeDataFunc;
    heap->freeKeyFunc = heap1->freeKeyFunc;
    heap->heapSize = heap1->heapSize + heap2->heapSize;
    if(heap1->topNode == NULL){
        heap->topNode = heap2->topNode;
    }else if(heap2->topNode == NULL){
        heap->topNode = heap1->topNode;
    }else{
        if(heap->compareFunc(heap1->topNode->key , heap2->topNode->key) < 0){
            heap->topNode = heap2->topNode;
        }
    }
    heap1->topNode = heap2->topNode = NULL;
    for(int i = 0 ; i < heap1->maxDegree ; i ++){
        if(getSizeAList(heap1->degreeList[i]) > 0){
            insertArrayListAList(heap1->degreeList[i] , heap->degreeList[i]);
        }
        destroyAList(heap1->degreeList[i]);
    }
    for(int i = 0 ; i < heap2->maxDegree ; i ++){
        if(getSizeAList(heap2->degreeList[i]) > 0){
            insertArrayListAList(heap2->degreeList[i] , heap->degreeList[i]);
        }
        destroyAList(heap2->degreeList[i]);
    }
    heap->dgreeSize = heap1->dgreeSize > heap2->dgreeSize ? heap1->dgreeSize : heap2->dgreeSize;
    heap2->degreeList = heap1->degreeList = NULL;
    keep_degree_only_one_FHeap(heap);
    destoryFHeap(heap1);
    destoryFHeap(heap2);
    return heap;
}

HeapNodeFHeap *find_node_in_tree_by_key_FHeap(void * key , HeapNodeFHeap *node , FHeap * heap){
    if(node == NULL){
        return;
    }
    int compareValue = heap->compareFunc(node->key , key);
    if(compareValue == 0){
        return node;
    }else if(compareValue < 0){
        return NULL;
    }
    if(node->child != NULL){
        compareValue = heap->compareFunc(node->topChild->key , key);
        if(compareValue == 0){
            return node->topChild;
        }else if(compareValue > 0){
            HeapNodeFHeap * temp = node->child;
            do{
                if(find_node_in_tree_by_key_FHeap(key , temp , heap) != NULL){
                    return temp;
                }
                temp = temp->right;
            }while(temp != node->child);
        }else{
            return NULL;
        }
    }
    return NULL;
}

HeapNodeFHeap *find_node_by_key_FHeap(void *key , FHeap * heap){
    for(int i = 0 ; i < heap->dgreeSize ; i ++){
        if(getSizeAList(heap->degreeList[i]) > 0 && heap->compareFunc(((HeapNodeFHeap *)getFirstElementAList(heap->degreeList[i]))->key , key) >= 0){
            HeapNodeFHeap * node = find_node_in_tree_by_key_FHeap(key , getFirstElementAList(heap->degreeList[i]) , heap);
            if(node != NULL){
                return node;
            }
        }
    }
    return NULL;
}

void updateKeyFHeap(void *key1 , void *key2 , FHeap *heap){
    if(heap == NULL || heap->topNode == NULL){
        printf("heap is null or heap is empty\n");
        return;
    }
    if(key1 == key2){
        printf("key1 adrress is equal to key2\n");
    }
    HeapNodeFHeap * node = find_node_by_key_FHeap(key1 , heap);
    if(node == NULL){
        printf("key is not in heap , update fialed\n");
        return;
    }else{
        updateKeyWithNodeFHeap(key2 , node , heap);
    }
}

int cut_top_node_child_to_heap_FHeap(void * key , HeapNodeFHeap *node , FHeap * heap){
    if(node == NULL){
        return;
    }
    HeapNodeFHeap * temp = node;
    HeapNodeFHeap * endNode = NULL;
    temp->left->right = NULL;
    temp->left = NULL;
    while(temp != NULL){
        if(heap->compareFunc(temp->key , key) <= 0){
            endNode = temp;
            temp = temp->right;
        }else{
            break;
        }
    }
    while(temp != NULL){
        temp->parent = NULL;
        temp->left->right = NULL;
        temp->left = NULL;
        join_node_to_top_link_FHeap(temp , heap);
        temp = temp->right;
    }
    if(endNode != NULL){
        endNode->right = node;
        node->left = endNode;
    }
    return endNode == NULL ? 1 : endNode->degree + 1;
}

void modify_node_key_FHeap(void *key , HeapNodeFHeap *node , FHeap *heap){
    if(key == node->key){
        return;
    }
    if(heap->freeKeyFunc != NULL){
        heap->freeKeyFunc(node->key);
    }
    node->key = key;
}

void exchange_node_relation_up_FHeap(HeapNodeFHeap * node , FHeap * heap){
    printf("exchange_node_relation_up_FHeap , node is %d , pNode is %d \n",*((int *)node->key) , *((int *)node->parent->key));
    HeapNodeFHeap * pNode = node->parent;
    HeapNodeFHeap  *temp = NULL;
    temp = pNode->left;
    if(node->left == node){
        pNode->left = pNode;
    }else{
        pNode->left = node->left;
        pNode->left->right = pNode;
    }
    if(temp == NULL){
        node->left = NULL;
    }else if(temp->left == temp){
        node->left = node;
    }else{
        node->left = temp;
        node->left->right = node;
    }
    temp = pNode->right;
    if(node->right == node){
        pNode->right = pNode;
    }else{
        pNode->right = node->right;
        pNode->right->left = pNode;
    }
    if(temp == NULL){
        node->right = NULL;
    }else if(temp->right == temp){
        node->right = node;
    }else{
        node->right = temp;
        node->right->left = node;
    }
 
    node->parent = pNode->parent;
    if(pNode->parent != NULL && pNode->parent ->child == pNode){
        pNode->parent->child = node;
    }
    if(pNode->parent != NULL && pNode->parent ->topChild == pNode){
        pNode->parent->topChild = node;
    }
    temp = pNode;
    do{
        temp->parent = node;
        temp = temp->right;
    }while (pNode != temp);
    temp = pNode->child;
    pNode->child = node->child;
    pNode->topChild = node->topChild;
    if(temp == node){
        node->child = pNode; 
    }else{
        node->child = temp;
    }
    node->topChild = pNode;
    if(pNode->child != NULL){
        temp = pNode->child;
        do{
            temp->parent = pNode;
            temp = temp->right;
        }while(temp != pNode->child);
    }
    int degree = pNode->degree;
    pNode->degree = node->degree;
    node->degree = degree;
}

void exchange_node_relation_down_FHeap(HeapNodeFHeap *pNode , FHeap *heap){
    printf("exchange_node_relation_down_FHeap , pNode is %d , topNode is %d" , *(int *)pNode->key , *(int *)pNode->topChild->key);
    HeapNodeFHeap * topChild = pNode->topChild;
    HeapNodeFHeap * node = NULL , *temp = NULL;
    
    temp = pNode->left;
    if(topChild->left == topChild){
        pNode->left = pNode;
    }else{
        pNode->left = topChild->left;
        pNode->left->right = pNode;
    }
    if(temp == NULL){
        topChild->left = NULL;
    }else if(temp->left == temp){
        topChild->left = topChild;
    }else{
        topChild->left = temp;
        topChild->left->right = topChild;
    }
    
    temp = pNode->right;
    if(topChild->right == topChild){
        pNode->right = pNode;
    }else{
        pNode->right = topChild->right;
        pNode->right->left = pNode;
    }
    if(temp == NULL){
        topChild->right = NULL;
    }else if(temp->right == temp){
        topChild->right = topChild;
    }else{
        topChild->right = temp;
        topChild->right->left = topChild;
    }
    topChild->parent = pNode->parent;
    if(pNode->parent != NULL){
        if(pNode->parent->child == pNode){
            pNode->parent->child = topChild;
        }
        pNode->parent->topChild = topChild;
        node = pNode->parent->child;
        do{
            if(heap->compareFunc(node->key , pNode->parent->topChild->key) > 0){
                pNode->parent->topChild = node;
            }
            node = node->right;
        }while(node != pNode->parent->child);
    }
    temp = topChild->child;
    if(pNode->child == topChild){
        topChild->child = pNode;
    }else{
        topChild->child = pNode->child;
    }
    pNode->topChild = topChild->topChild;
    topChild->topChild = pNode;
    node = pNode;
    do{
        node->parent = topChild;
        if(heap->compareFunc(node->key , topChild->topChild->key) > 0){
            topChild->topChild = node;
        }
        node = node->right;
    }while(node != pNode);
    printf(" new topNode is %d \n", *(int *)topChild->topChild->key);
    pNode->child = temp;
    if(temp != NULL){
        do{
            temp->parent = pNode;
            temp = temp->right;
        }while(temp != pNode->child);
    }
    int degree = node->degree;
    node->degree = topChild->degree;
    topChild->degree = degree;
    if(topChild->parent == NULL){
        deleteElementByIndexAList(0 , heap->degreeList[topChild->degree - 1]);
        insertElementAList(topChild , heap->degreeList[topChild->degree - 1]);
    }
}

void exchange_node_data_FHeap(HeapNodeFHeap *node1 , HeapNodeFHeap *node2){
    void * temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
    temp = node1->key;
    node1->key = node2->key;
    node2->key = temp;
}

HeapNodeFHeap * exchange_node_children_data_FHeap(HeapNodeFHeap *node, FHeap *heap){
    if(node == NULL){
        printf("node is null , exchange failed\n");
        return NULL;
    }
    HeapNodeFHeap * child = node->topChild;
    if(child != NULL && heap->compareFunc(child->key , node->key) > 0){
        exchange_node_data_FHeap(node , child);
        HeapNodeFHeap * temp = child->right;
        while(temp != child){
            if(heap->compareFunc(temp->key , node->topChild->key) > 0){
                node->topChild = temp;
            }
            temp = temp->right;
        }
        return child;
    }
    return node;
}

void updateKeyWithNodeFHeap(void * key , HeapNodeFHeap * node , FHeap * heap){
    if(node == NULL || heap == NULL){
        printf("node or heap is null , update fialed\n");
        return;
    }
    if(key != node->key){
        modify_node_key_FHeap(key , node , heap);
    }
    if(node == heap->topNode){
        for(int i = 0 ; i < heap->dgreeSize ; i ++){
            if(getSizeAList(heap->degreeList[i]) > 0){
                HeapNodeFHeap * temp = (HeapNodeFHeap *)getFirstElementAList(heap->degreeList[i]);
                if(heap->compareFunc(temp->key , heap->topNode->key) > 0){
                    heap->topNode = temp;
                }
            }
        }
    }
    if(node->parent != NULL && node == node->parent->topChild){
        HeapNodeFHeap * temp = node;
        do{
            if(heap->compareFunc(temp->key , temp->parent->topChild->key) > 0){
                temp->parent->topChild = temp;
            }
            temp = temp->right;
        }while(temp != node);
    }
    int compareValue = 0;
    if(node->topChild != NULL && heap->compareFunc(key , node->topChild->key) < 0){
        compareValue = -1;
    }else if(node->parent != NULL && heap->compareFunc(key , node->parent->topChild->key) >= 0){
        compareValue = 1;
    }
    printf("compareValue is %d\n" , compareValue);
    if(compareValue == 0){
        if(heap->compareFunc(heap->topNode->key , key) < 0){
            heap->topNode = node;
        }
        return;
    }else if(compareValue < 0){
        do{
            exchange_node_relation_down_FHeap(node , heap);
        }while(node->topChild != NULL && heap->compareFunc(node->key , node->topChild->key) < 0);
        
        /*HeapNodeFHeap * temp = node;
        do{
            node = temp;
            temp = exchange_node_children_data_FHeap(node , heap);
        }while(temp != NULL && node != temp);
        return node;*/
    }else{
        while(node->parent != NULL){
            if(heap->compareFunc(node->key , node->parent->key) > 0){
                exchange_node_relation_up_FHeap(node , heap);
            }else if(heap->compareFunc(node->key , node->parent->topChild->key) > 0){
                node->parent->topChild = node;
                break;
            }else{
                break;
            }
        }
        if(node->parent == NULL){
            deleteElementByIndexAList(0 , heap->degreeList[node->degree - 1]);
            insertElementAList(node , heap->degreeList[node->degree - 1]);
        }
        if(heap->compareFunc(heap->topNode->key , key) < 0){
            heap->topNode = node;
        }
    }
    printf("update key success\n");
    //return node;
}

int isInFHeap(void *key , FHeap *heap){
    if(heap == NULL || heap->topNode == NULL){
        return 0;
    }
    return find_node_by_key_FHeap(key , heap) == NULL ? 0 : 1;
}