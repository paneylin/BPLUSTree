int compare_default_AList(void * a, void * b){
    return a == b;
}

ArrayList * createArrayListAList(int (*compareFunc)(void * , void *)){
    ArrayList * list = (ArrayList *)malloc(sizeof(ArrayList));
    list->data = (void **)malloc(sizeof(void *) * ARRAYLIST_DEFAULT_SIZE);
    for(int i = 0 ; i < ARRAYLIST_DEFAULT_SIZE ; i++){
        list->data[i] = NULL;
    }
    list->dataSize = ARRAYLIST_DEFAULT_SIZE;
    list->currentSize = 0;
    list->compareFunc = compareFunc;
    if(compareFunc == NULL){
        list->compareFunc = compare_default_AList;
    }
    list->freeDataFunc = NULL;
    return list;
}

int resize_list_size_AList(ArrayList *list){
    void ** newData = (void **)realloc(list->data , sizeof(void *) * list->dataSize * 2);
    if(newData == NULL){
        printf("realloc error , insert failed\n");
        return -1;
    }else{
        list->data = newData;
        for(int i = list->dataSize ; i < list->dataSize * 2 ; i++){
            list->data[i] = NULL;
        }
        list->dataSize = list->dataSize * 2;
    }
    return 1;
}

void insertElementAList(void *data , ArrayList * list){
    if(list->currentSize == list->dataSize){
        if(!resize_list_size_AList(list)){
            return;
        }
    }
    list->data[list->currentSize] = data;
    list->currentSize++;
}

void * getElementAList(void * data , ArrayList * list){
    for(int i = 0 ; i < list->currentSize ; i++){
        if(list->compareFunc(list->data[i] , data)){
            return list->data[i];
        }
    }
    return NULL;
}

void * getElementByIndexAList(int index , ArrayList * list){
    if(index >= list->currentSize){
        printf("index out of range , getElementByIndexAList failed\n");
        return NULL;
    }
    return list->data[index];
}

void deleteElementAList(void * data , ArrayList * list){
    for(int i = 0 ; i < list->currentSize ; i++){
        if(list->compareFunc(list->data[i] , data)){
            memmove(list->data + i , list->data + i + 1, sizeof(void *) * (list->dataSize - i - 1));
            list->currentSize--;
            list->data[list->currentSize] = NULL;
        }
    }
}

void deleteElementByIndexAList(int index , ArrayList * list){
    if(index >= list->currentSize){
        return;
    }
    memmove(list->data + index , list->data + index + 1, sizeof(void *) * (list->dataSize - index - 1));
    list->currentSize--;
    list->data[list->currentSize] = NULL;
}

int isEmptyAList(ArrayList * list){
    return list->currentSize == 0;
}

void setFreeDataFuncAList(void (*freeDataFunc)(void * data), ArrayList * list){
    list->freeDataFunc = freeDataFunc;
}

void destroyAList(ArrayList * list){
    if(list->freeDataFunc != NULL){
        for(int i = 0 ; i < list->currentSize ; i++){
            list->freeDataFunc(list->data[i]);
        }
    }
    free(list->data);
    list->data = NULL;
    free(list);
    list = NULL;
}

int getSizeAList(ArrayList * list){
    return list->currentSize;
}

void showAList(ArrayList * list , void (*showFunc)(void *)){
    for(int i = 0 ; i < list->currentSize ; i++){
        showFunc(list->data[i]);
    }
    printf("\n");
}