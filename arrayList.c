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

void *deleteElementAList(void * data , ArrayList * list){
    for(int i = 0 ; i < list->currentSize ; i++){
        if(list->compareFunc(list->data[i] , data)){
            void * ret = list->data[i];
            memmove(list->data + i , list->data + i + 1, sizeof(void *) * (list->currentSize - i - 1));
            list->currentSize--;
            list->data[list->currentSize] = NULL;
            return ret;
        }
    }
    return NULL;
}

void *deleteElementByIndexAList(int index , ArrayList * list){
    if(index >= list->currentSize){
        printf("index out of range , deleteElementByIndexAList failed\n");
        return NULL;
    }
    void * data = list->data[index];
    memmove(list->data + index , list->data + index + 1, sizeof(void *) * (list->currentSize - index - 1));
    list->currentSize--;
    list->data[list->currentSize] = NULL;
    return data;
}

void * deleteLastElementAList(ArrayList * list){
    if(list->currentSize == 0){
        printf("list is empty , deleteLastElementAList failed\n");
        return NULL;
    }
    void * data = list->data[list->currentSize - 1];
    list->currentSize--;
    list->data[list->currentSize] = NULL;
    return data;
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

void insertElementsAList(void **datas , int length , ArrayList * list){
    while(list->currentSize + length > list->dataSize){
        if(!resize_list_size_AList(list)){
            return;
        }
    }
    memmove(list->data + list->currentSize , datas , sizeof(void *) * length);
}

void insertArrayListAList(ArrayList *list1 , ArrayList *list2){
    while(list1->currentSize + list2->currentSize > list1->dataSize){
        if(!resize_list_size_AList(list1)){
            return;
        }
    }
    memmove(list2->data + list2->currentSize , list1->data , sizeof(void *) * list1->currentSize);
}

void * getFirstElementAList(ArrayList * list){
    if(list->currentSize == 0){
        printf("list is empty , getFirstElementAList failed\n");
        return NULL;
    }
    return list->data[0];
}

void * getLastElementAList(ArrayList * list){
    if(list->currentSize == 0){
        printf("list is empty , getLastElementAList failed\n");
        return NULL;
    }
    return list->data[list->currentSize - 1];
}

void insertElementByIndexAList(int index , void *data , ArrayList * list){
    if(index > list->currentSize){
        printf("index out of range , insertElementByIndexAList failed\n");
        return;
    }
    if(list->currentSize == list->dataSize){
        if(!resize_list_size_AList(list)){
            return;
        }
    }
    memmove(list->data + index + 1 , list->data + index , sizeof(void *) * (list->currentSize - index));
    list->data[index] = data;
    list->currentSize++;
}

int isExistElementAList(void * data , ArrayList * list){
    for(int i = 0 ; i < list->currentSize ; i++){
        if(list->compareFunc(list->data[i] , data)){
            return 1;
        }
    }
    return 0;
}

void *replaceElementByIndexAList(int index , void *data , ArrayList * list){
    if(index >= list->currentSize){
        printf("index out of range , replaceElementByIndexAList failed\n");
        return NULL;
    }
    void * ret = list->data[index];
    list->data[index] = data;
    return ret;
}

int getDataIndexAList(void *data , ArrayList * list){
    if(data == NULL){
        printf("data is NULL , getDataIndexAList failed\n");
        return -1;
    }
    for(int i = 0 ; i < list->currentSize ; i++){
        if(list->compareFunc(list->data[i] , data)){
            return i;
        }
    }
    return -1;
}