#include "./heap.h"

int compare_value_default_Heap(void *data1, void *data2){
    return *(int *)data1 - *(int *)data2;
}

HeapInfo *createHeapInfoHeap(int basicHeapSize ,int (*compareFunc)(void * , void *)){
    HeapInfo *heapInfo = (HeapInfo *)malloc(sizeof(HeapInfo));
    heapInfo->heapData = (void **)malloc(sizeof(void *) * basicHeapSize);
    heapInfo->heapSize = basicHeapSize;
    heapInfo->dataSize = 0;
    heapInfo->compareFunc = compareFunc;
    if(compareFunc == NULL){
        heapInfo->compareFunc = compare_value_default_Heap;
    }
    heapInfo->freeDataFunc = NULL;
    return heapInfo;
}

void showHeapInfo(HeapInfo * heapInfo , void (*showFunc)(void *)){
    printf("dataSize:%d\n",heapInfo->dataSize);
    printf("heapData:\n");
    for(int i = 0;i < heapInfo->dataSize;i++){
        showFunc(heapInfo->heapData[i]);
    }
    printf("\n");
}

int get_left_child_index_Heap(int index){
    return 2 * ( index + 1 ) - 1;
}

int get_right_child_index_Heap(int index){
    return 2 * ( index + 1 );
}

int get_parent_index_Heap(int index){
    return (index + 1) / 2 - 1;
}

int get_tree_leaf_num_Heap(int height){
    return (int)pow(2,height - 1);
}

int get_tree_height_Heap(int dataSize){
    int height = 1;
    while(dataSize/2 != 0){
        height ++;
        dataSize /= 2;
    }
    return height;
}

int get_tree_height_start_index_Heap(int height){
    int start = 0;
    for(int i = 1;i < height;i++){
        start += pow(2,i-1);
    }
    return start;
}

void exchange_data_position_heap(HeapInfo *heapInfo ,int index1 , int index2){
    void *temp = heapInfo->heapData[index1];
    heapInfo->heapData[index1] = heapInfo->heapData[index2];
    heapInfo->heapData[index2] = temp;
}

int exchange_heap(HeapInfo *heapInfo , int parentIndex , int leftIndex , int rightIndex){
    if(leftIndex == -1 ){
        return -1;
    }else if(rightIndex == -1){
        return heapInfo->compareFunc(heapInfo->heapData[parentIndex] , heapInfo->heapData[leftIndex]) > 0 ? -1 :
                                                        exchange_data_position_heap(heapInfo, parentIndex, leftIndex),leftIndex;
    }else{
        if(heapInfo->compareFunc(heapInfo->heapData[leftIndex] ,heapInfo->heapData[rightIndex]) > 0){
            return heapInfo->compareFunc(heapInfo->heapData[parentIndex],heapInfo->heapData[leftIndex]) > 0 ? -1 
                : (exchange_data_position_heap(heapInfo,parentIndex,leftIndex) ,leftIndex);
        }else{
            return heapInfo->compareFunc(heapInfo->heapData[parentIndex],heapInfo->heapData[rightIndex]) > 0 ? -1 
                : (exchange_data_position_heap(heapInfo,parentIndex,rightIndex) ,rightIndex);
        }
    }
}

void exchange_from_top_heap(HeapInfo *heapInfo, int blankPositon){
    //printf("exchange_from_top_heap %d\n" , blankPositon);
    int left = get_left_child_index_Heap(blankPositon);
    int right = left + 1;
    left = left > heapInfo->dataSize - 1 ? -1 : left;
    right = right > heapInfo->dataSize - 1 ? -1 : right;
    int newExchangePoint = exchange_heap(heapInfo, blankPositon, left, right);
    if(newExchangePoint != -1){
        exchange_from_top_heap(heapInfo, newExchangePoint);
    }
}

void exchange_from_bottom_heap(HeapInfo *heapInfo , int position){
    if(position == 0){
        return;
    }
    int parent = get_parent_index_Heap(position);
    int left ,right;
    position % 2 == 0 ? (right = position , left = position - 1) : 
        position == (heapInfo->dataSize -1) ? right = -1 , left = position : (left = position , right = position + 1);
    if(exchange_heap(heapInfo, parent, left, right) != -1){
        exchange_from_bottom_heap(heapInfo, parent);
    }
}

void repare_down_heap(HeapInfo * heapInfo){
    exchange_from_top_heap(heapInfo, 0);
}

void repare_up_heap(HeapInfo * heapInfo){
    exchange_from_bottom_heap(heapInfo, heapInfo->dataSize - 1);
}

void reBuid_Heap(HeapInfo *heapInfo , int startIndex , int endIndex){
    CircleQueue *queue = createCircleQueue();
    for(int i = endIndex ; i >= startIndex ; ){
        int parent = get_parent_index_Heap(i);
        if(parent < 0){
            break;
        }
        if(i % 2 == 0 && (i - 1) >= startIndex){
            int moveNode = exchange_heap(heapInfo , parent , i -1 , i);
            if(moveNode != -1 ){
                exchange_from_top_heap(heapInfo , moveNode);
                if(parent < startIndex){
                    pushCircleQueue((void *)&parent , queue);
                }
            }
            i -= 2;
        }else{
            int moveNode = exchange_heap(heapInfo , parent , i , -1);
            if(moveNode != -1){
                exchange_from_top_heap(heapInfo , moveNode);
                if(parent < startIndex){
                    pushCircleQueue((void *)&parent , queue);
                }
            }
            i --;
        }
    }
    while(!isEmptyCircleQueue(queue) && *(int *)getElementsFromCircleQueue(queue) > 0){
        int curPoints = *(int *)popCircleQueue(queue);
        int parent = get_parent_index_Heap(curPoints);
        if(curPoints % 2 == 0 && isEmptyCircleQueue(queue) && *(int *)getElementsFromCircleQueue(queue)  == curPoints -1){
            int moveNodeindex = exchange_heap(heapInfo , parent , *(int *)getElementsFromCircleQueue(queue) , curPoints) ;
            if(moveNodeindex != -1){
                exchange_from_top_heap(heapInfo , moveNodeindex);
                pushCircleQueue((void *)&parent , queue);
            }
            popCircleQueue(queue);
        }else{
            int moveNodeindex = exchange_heap(heapInfo , parent , curPoints , -1); 
            if(moveNodeindex != -1){
                exchange_from_top_heap(heapInfo , moveNodeindex);
                pushCircleQueue((void *)&parent , queue);
            }
        }
    }
    destroyCircleQueueQ(queue);
}

void * getElementHeap(HeapInfo *heapInfo){
    if(heapInfo->heapSize == 0 || heapInfo->dataSize == 0){
        printf("heap is empty\n");
        return NULL;
    }
    return heapInfo->heapData[0];
}

void * popElementHeap(HeapInfo * heapInfo){
    if(heapInfo->heapSize == 0 || heapInfo->dataSize == 0){
        printf("heap is empty\n");
        return NULL;
    }
    void *data = heapInfo->heapData[0];
    if(heapInfo->dataSize > 1){
        heapInfo->heapData[0] = heapInfo->heapData[heapInfo->dataSize - 1];
        heapInfo->heapData[heapInfo->dataSize - 1] = NULL;
        heapInfo->dataSize --;
        repare_down_heap(heapInfo);
    }else{
        heapInfo->dataSize --;
    }
    return data;
}

void insertElementsHeap(void **data , int dataSize , HeapInfo *heapInfo){
    if(heapInfo == NULL){
        printf("heap is empty\n");
        return;
    }
    if(heapInfo->dataSize + dataSize > heapInfo->heapSize){
        void ** tempPt = (void **)realloc(heapInfo->heapData, sizeof(void *) * (heapInfo->dataSize + dataSize));
        if(tempPt != NULL){
            heapInfo->heapData = tempPt;
        }else{
            printf("not enough memorys , insert failed! \n");
            return;
        }
        
        heapInfo->heapSize *= heapInfo->dataSize + dataSize;
    }
    int startDataIndex = heapInfo->dataSize;
    for(int i = 0; i < dataSize ; i ++){
        heapInfo->heapData[heapInfo->dataSize + i] = data[i];
    }
    heapInfo->dataSize += dataSize;
    int endIndex = heapInfo->dataSize - 1;
    reBuid_Heap(heapInfo,startDataIndex,endIndex);
}

void insertElementHeap(void *data , HeapInfo * heapInfo){
    if(heapInfo == NULL){
        printf("heap is NULL\n");
        return;
    }
    if(heapInfo->heapSize == 0){
        printf("heapSize is 0\n");
        return;
    }
    if(heapInfo->dataSize == heapInfo->heapSize){
        void ** tempPt = (void **)realloc(heapInfo->heapData, sizeof(void *) * (heapInfo->heapSize*2));
        if(tempPt != NULL){
            heapInfo->heapData = tempPt;
        }else{
            printf("not enough memorys , insert failed! \n");
            return;
        }
        heapInfo->heapSize *= 2;
    }
    heapInfo->heapData[heapInfo->dataSize] = data;
    heapInfo->dataSize ++;
    repare_up_heap(heapInfo);
}

int getHeapSizeHeap(HeapInfo * heap){
    return heap->dataSize;
}

int isEmptyHeap(HeapInfo * heap){
    return heap->dataSize == 0;
}

void setFreeDataFuncHeap(void (*freeDataFunc)(void *),HeapInfo *heapInfo){
    heapInfo->freeDataFunc = freeDataFunc;
}

void destroyHeapInfoHeap(HeapInfo * heapInfo){
    if(heapInfo == NULL){
        return;
    }
    if(heapInfo->heapData != NULL){
        if(heapInfo->freeDataFunc != NULL){
            for(int i = 0 ; i < heapInfo->dataSize ; i ++){
                heapInfo->freeDataFunc(heapInfo->heapData[i]);
                heapInfo->heapData[i] = NULL;
            }
        }
        free(heapInfo->heapData);
        heapInfo->heapData = NULL;
    }
    free(heapInfo);
    heapInfo = NULL;
}