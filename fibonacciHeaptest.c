#include "./fibonacciHeap.h"
#include "./sort.h"

int compareKeyFunc(void * key1 , void * key2){
    if(*(int *)key1 - *(int *)key2 != 0){
        return *(int *)key1 - *(int *)key2;
    }
    return (int)key1 - (int)key2;
}

int * getKeyFunc(int * data){
    return data;
}

void showNodeValue(void * key){
    printf("%d ",*(int *)key);
}

int testFHeapNum(FHeap * heap , int dataSize){
    int totalNUm = 0;
    int * data = NULL;
    CircleQueue *queue = createCircleQueue();
    for(int i = 0 ; i < heap->dgreeSize ; i ++){
        if(getSizeAList(heap->degreeList[i]) > 0){
            pushCircleQueue(getFirstElementAList(heap->degreeList[i]) , queue);
        }
    }
    while(!isEmptyCircleQueue(queue)){
        HeapNodeFHeap * node = (HeapNodeFHeap *)popCircleQueue(queue);
        totalNUm ++;
        if(node->child != NULL){
            pushCircleQueue(node->child , queue);
            HeapNodeFHeap * temp = node->child->right;
            while(temp != node->child){
                pushCircleQueue(temp , queue);
                temp = temp->right;
            }
        }
    }
    if(totalNUm != dataSize){
        printf("testFHeapNum error , totalNUm is %d , dataSize is %d, heap degreeSize is %d\n" , totalNUm , dataSize , heap->dgreeSize);
        return 0;
    }else{
        printf("testFHeapNum success\n");
        return 1;
    }
}

int testFheapStruct(FHeap * heap){
    if(heap->dgreeSize == 0){
        if(heap->topNode != NULL){
            printf("testFheapStruct error , heap->dgreeSize is 0 , but heap->topNode is not NULL\n");
            return 0;
        }
        printf("testFheapStruct success\n");
        return 1;
    }
    CircleQueue *queue = createCircleQueue();
    for(int i = 0 ; i < heap->dgreeSize ; i ++){
        if(getSizeAList(heap->degreeList[i]) > 1){
            printf("error , degreeList[%d] size is %d\n" , i , getSizeAList(heap->degreeList[i]));
            return 0;
        }else if(getSizeAList(heap->degreeList[i]) == 1){
            HeapNodeFHeap * node = (HeapNodeFHeap *)getFirstElementAList(heap->degreeList[i]);
            if(node->left != NULL || node->right != NULL || node->parent != NULL){
                printf("error , degreeList[%d] first node left or right or parent is not NULL\n" , i);
                return 0;
            }
            pushCircleQueue(node, queue);
        }
    }
    while(!isEmptyCircleQueue(queue)){
        HeapNodeFHeap * node = (HeapNodeFHeap *)popCircleQueue(queue);
        int degree = node->degree;
        if(node->child != NULL){
            int childDegree = 1;
            int flag = 1;
            HeapNodeFHeap * temp = node->child;
            do{   
                if(temp->parent != node){
                    printf("testFheapStruct error , node %d child-degree is %d , child-parent is not node\n" ,*(int *)node->key , childDegree);
                    return 0;
                }
                if(temp->degree != childDegree){
                    printf("testFheapStruct error , node %d , %d child-degree is %d\n" , *(int *)node->key , childDegree , temp->degree);
                    return 0;
                }
                if(temp == node->topChild){
                    flag = 0;
                }
                pushCircleQueue(temp , queue);
                temp = temp->right;
                childDegree ++;
            }while(temp != node->child);
            if(childDegree != node->degree){
                printf("testFheapStruct error , node %d degree is %d , last childDegree is %d\n" , *(int *)node->key , node->degree , childDegree - 1);
                return 0;
            }
            if(flag){
                printf("testFheapStruct error , node %d topChild is not in child list\n" , *(int *)node->key);
                return 0;
            }
        }else{
            if(node->topChild != NULL){
                printf("testFheapStruct error , node %d topChild is not NULL\n" , *(int *)node->key);
                return 0;
            }
        }
    }
    printf("testFheapStruct success\n");
    return 1;
}

int testFHeapSequence(FHeap * heap){
    if(heap->heapSize == 0){
        printf("testFHeapSequence success\n");
        return 1;
    }
    CircleQueue *queue = createCircleQueue();
    HeapNodeFHeap * topNode = heap->topNode;
    for(int i = 0 ; i < heap->dgreeSize ; i ++){
        if(getSizeAList(heap->degreeList[i]) > 0){
            HeapNodeFHeap * node = (HeapNodeFHeap *)getFirstElementAList(heap->degreeList[i]);
            if(i == topNode->degree - 1){
                if(node != topNode){
                    printf("testFHeapSequence error , topNode is %d , degreeList[%d] first node is %d\n" , *(int *)topNode->key , i , *(int *)node->key);
                    return 0;
                }
            }else if(compareKeyFunc(node->key , topNode->key) > 0){
                printf("testFHeapSequence error , node %d is more likely be heap topNode, topNode is %d\n" ,*(int *)node->key , (int *)topNode->key);
                return 0;
            }
            pushCircleQueue(getFirstElementAList(heap->degreeList[i]) , queue);
        }
    }
    while(!isEmptyCircleQueue(queue)){
        HeapNodeFHeap * node = (HeapNodeFHeap *)popCircleQueue(queue);
        if(node->child != NULL){
            topNode = node->topChild;
            if(compareKeyFunc(topNode->key , node->key) > 0){
                printf("testFHeapSequence error ,child node %d is more likely be father, PNode is %d\n " , *(int *)topNode->key , *(int *)node->key );
                return 0;
            }
            HeapNodeFHeap * temp = node->child;
            do{
                pushCircleQueue(temp , queue);
                if(compareKeyFunc(temp->key , topNode->key) > 0){
                    printf("testFHeapSequence error , node %d is more likely be topNode, topNode is %d\n" , *(int *)temp->key , *(int *)topNode->key);
                    return 0;
                }
                temp = temp->right;
            }while(temp != node->child);
        }
    }
    printf("testFHeapSequence success\n");
    return 1;
}

int testDataINFHeap(FHeap * heap ,int ** data , int dataSize){
    for(int i = 0 ; i < dataSize ; i ++){
        if(!isInFHeap(data[i] , heap)){
            printf("testDataINFHeap error , data %d is not in heap\n" , *data[i]);
            return 0;
        }
    }
    printf("testDataINFHeap success\n");
    return 1;
}

int testFHeap(FHeap * heap , int ** datas , int dataSize){
    if(testFHeapNum(heap , dataSize) == 0){
        return 0;
    }
    if(testFheapStruct(heap) == 0){
        return 0;
    }
    if(testFHeapSequence(heap) == 0){
        return 0;
    }
    if(testDataINFHeap(heap , datas , dataSize) == 0){
        return 0;
    }
    printf("testFHeap success\n");
    return 1;
}

int main(){
    FHeap * heao1 = createFHeap(compareKeyFunc , getKeyFunc);
    int num;
    scanf("%d" , &num);
    int **datas = (int **)malloc(sizeof(int *) * num * 2);
    HeapNodeFHeap **nodes = (HeapNodeFHeap **)malloc(sizeof(HeapNodeFHeap *) * num);
    HeapNodeFHeap * node = NULL;
    for(int i = 0 ; i < num ; ){
        int *data = (int *)malloc(sizeof(int));
        *data = rand()%10000;
        if((node = insertElementFHeap(data , heao1)) != NULL){
            nodes[i] = node;
            datas[i] = data;
            i++;
        }
    }
    FHeap * heap2 = createFHeap(compareKeyFunc , getKeyFunc);
    for(int i = 0 ; i < num ; ){
        int *data = (int *)malloc(sizeof(int));
        *data = rand()%10000;
        if((node = insertElementFHeap(data , heap2)) != NULL){
            nodes[num + i] = node;
            datas[num + i] = data;
            i++;
        }
    }
    printf("insert success\n");
    for(int i = 0 ; i < num ; i ++){
        printf("old key is %d " , *datas[i]);
        int *newKey = datas[i];
        *newKey = rand()%10000;
        printf("new key is %d\n" , *newKey);
        updateKeyWithNodeFHeap(newKey ,nodes[i] , heao1);
        if(!testFHeap(heao1 , datas , num)){
            printf("testFHeap error\n");
            return 0;
        } 
    }
    
    if(!testFHeap(heao1 , datas , num)){
        printf("testFHeap error\n");
        return 0;
    }  
    heao1 = combindHeapsFHeap(heao1 , heap2);
    if(!testFHeap(heao1 , datas , num * 2)){
        printf("testFHeap combind error\n");
        return 0;
    }  
    int i = 1;
    quickSort(datas , num * 2 , compareKeyFunc);
    while(!isEmptyFHeap(heao1)){
        int *data = (int *)popElementHeapFHeap(heao1);
        if(!testFHeap(heao1 , datas , num * 2 - i)){
            printf("pop %d error , index is %d\n" , *(int *)data , i);
            return 0;
        }
        i ++;
    }
    printf("end ,success\n");
}