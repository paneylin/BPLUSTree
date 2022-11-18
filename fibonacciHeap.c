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
        heap->degreeList[i] = createArrayList();
    }
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
    }else{
        if(heap->compareFunc(node->key , heap->topNode->key) > 0){
            heap->topNode = node;
        }
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
    int degreeSize = heap->dgreeSize;
    for(int i = 0 ; i < degreeSize ; i++){
        while(getArrayListSize(heap->degreeList[i]) > 1){
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

void insertElementFHeap(void *data , FHeap * heapInfo){
    if(data == NULL || heapInfo == NULL){
        printf("data or heapInfo is null\n");
        return;
    }
    HeapNodeFHeap * node = create_heap_node_FHeap(heapInfo->getKeyFunc(data) , data);
    join_node_to_top_link_FHeap(node , heapInfo);
    keep_degree_only_one_FHeap(heapInfo);
    heapInfo->heapSize++;
}

void insertElementsFHeap(void **datas , int size , FHeap * heapInfo){
    if(datas == NULL || heapInfo == NULL){
        printf("data or heapInfo is null\n");
        return;
    }
    for(int i = 0 ; i < size ; i++){
        HeapNodeFHeap * node = create_heap_node_FHeap(heapInfo->getKeyFunc(datas[i]) , datas[i]);
        join_node_to_top_link_FHeap(node , heapInfo);
    }
    keep_degree_only_one_FHeap(heapInfo);
    heapInfo->heapSize += size;
}

void destroy_Heap_node_keep_data_HHeap(HeapNodeFHeap * node , FHeap * heap){
    if(node == NULL){
        return;
    }
    node->child= node->parent = node->left = node->right = NULL;
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
    node->child= node->parent = node->left = node->right = NULL;
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

/*void link_join_link_FHeap(HeapNodeFHeap * node1 , HeapNodeFHeap * node2){
    if(node1 == NULL || node2 == NULL){
        printf("node1 or node2 is null,join link failed\n");
        return;
    }
    if(node1->left == node1){
        join_node_to_link_FHeap(node2 , node1);
    }else if(node2->left == node2){
        join_node_to_link_FHeap(node1 , node2);
    }else{
        HeapNodeFHeap * node1Left = node1->left;
        HeapNodeFHeap * node2Left = node2->left;
        node1Left->right = node2;
        node2->left = node1Left;
        node2Left->right = node1;
        node1->left = node2Left;
    }
}*/

void re_cal_degree_HHeap(FHeap * heap){
    for(int i = heap->dgreeSize - 2 ; i >= 0 ; i--){
        if(getArrayListSize(heap->degreeList[i]) > 0){
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
            if(getArrayListSize(heap->degreeList[i]) > 0){
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
    deleteElementByKeyAList(topNode , heap->degreeList[topNode->degree - 1]);
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

void * getElementHeap(FHeap *heapInfo){
    if(heapInfo == NULL || heapInfo->topNode == NULL){
        printf("heapInfo is null or heapInfo is empty\n");
        return NULL;
    }
    return heapInfo->topNode->data;
}

int getHeapSizeHeap(FHeap * heap){
    return heap->heapSize;
}

int isEmptyHeap(FHeap * heap){
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
        for(int i = 0 ; i < heap->dgreeSize ; i ++){
            if(getArrayListSize(heap->degreeList[i]) > 0){
                destroy_heap_tree_HHeap(getElementAList(0 , heap->degreeList[i]) , heap);
            }
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
        //heap->topNode = heap1->topNode;
        //link_join_link_FHeap(heap1->topNode , heap2->topNode);
        if(heap->compareFunc(heap1->topNode->key , heap2->topNode->key) < 0){
            heap->topNode = heap2->topNode;
        }
    }
    heap1->topNode = heap2->topNode = NULL;
    for(int i = 0 ; i < heap1->dgreeSize ; i ++){
        insertArrayListAList(heap1->degreeList[i] , heap->degreeList[i]);
        destoryArrayList(heap1->degreeList[i]);
    }
    for(int i = 0 ; i < heap2->dgreeSize ; i ++){
        insertArrayListAList(heap2->degreeList[i] , heap->degreeList[i]);
        destoryArrayList(heap2->degreeList[i]);
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
    }else if(compareValue > 0){
        HeapNodeFHeap * child = node->child;
        if(child != NULL){
            HeapNodeFHeap * temp = child->right;
            while(temp != node->child){
                child = temp->right;
                if(find_node_in_tree_by_key_FHeap(key , temp , heap) != NULL){
                    return temp;
                }
                temp = child;
            }
            if(find_node_in_tree_by_key_FHeap(key , temp , heap) != NULL){
                return temp;
            }
        }
    }else{
        return NULL;
    }
    return NULL;
}

HeapNodeFHeap *find_node_by_key_FHeap(void *key , FHeap * heap){
    for(int i = 0 ; i < heap->dgreeSize ; i ++){
        if(getArrayListSize(heap->degreeList[i]) > 0 && heap->compareFunc(((HeapNodeFHeap *)getFirstElementAList(heap->degreeList[i]))->key , key) >= 0){
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
    HeapNodeFHeap * node = find_node_by_key_FHeap(key1 , heap->topNode);
    if(node == NULL){
        printf("key is not in heap , update fialed\n");
        return;
    }else{
        update_key_With_node_FHeap(key2 , node , heap);
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
    if(heap->freeKeyFunc != NULL){
        heap->freeKeyFunc(node->key);
    }
    node->key = key;
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
    if(node == NULL || node->child == NULL){
        return NULL;
    }
    HeapNodeFHeap * rsl = node;
    HeapNodeFHeap * child = node->child;
    do{
        if(heap->compareFunc(child->key , rsl->key) > 0){
            rsl = child;
        }
        child = child->right;
    }while(child != node->child);
    if(rsl != node){
        exchange_node_data_FHeap(rsl , node);
        return rsl;
    }else{
        return NULL;
    }
}

void *update_key_With_node_FHeap(void * key , HeapNodeFHeap * node , FHeap * heap){
    if(node == NULL || heap == NULL){
        printf("node or heap is null , update fialed\n");
        return;
    }
    if(node->parent == NULL){
        if(heap->compareFunc(key , node->key) > 0){
            modify_node_key_FHeap(key , node , heap);
            if(heap->compareFunc(heap->topNode->key , key) < 0){
                heap->topNode = node;
            }
        }else{
            modify_node_key_FHeap(key , node , heap);
            int newDegree = cut_top_node_child_to_heap_FHeap(key , node->child , heap);
            if(node->degree > newDegree){
                deleteElementAList(node , heap->degreeList[node->degree-1]);
                insertArrayListAList(node , heap->degreeList[newDegree-1]);
                node->degree = newDegree;
            }
            keep_degree_only_one_FHeap(heap);
        }
    }else{
        if(heap->compareFunc(key , node->key) > 0){
            modify_node_key_FHeap(key , node , heap);
            while(node->parent != NULL && heap->compareFunc(node->key , node->parent->key) > 0){
                exchange_node_data_FHeap(node , node->parent);
                node = node->parent;
            }
            if(heap->compareFunc(heap->topNode->key , key) < 0){
                heap->topNode = node;
            }
        }else{
            modify_node_key_FHeap(key , node , heap);
            do{
                node = exchange_node_children_data_FHeap(node , heap);
            }while(node != NULL);
        }
    }
}