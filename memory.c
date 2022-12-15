int compareMemory(void *a , void *b){
    if(a > b){
        return 1;
    }else if(a == b){
        return 0;
    }else{
        return -1;
    }
}

TreeLRTree *memoryTreeMemory = NULL;

void* mallocMemory(int size){
    if(memoryTreeMemory == NULL){
        memoryTreeMemory = createTreeLRTree(compareMemory , NULL);
    }
    void *memory = malloc(size);
    insertDataLRTree(memory , memoryTreeMemory);
    return memory;
}

void *rellocMemory(void *memory , int size){
    if(memoryTreeMemory == NULL){
        printf("try to relloc memory but memoryTreeMemory is NULL\n");
        return NULL;
    }
    if(memory != NULL && getDataFromTreeLRTree(memory , memoryTreeMemory) == NULL){
        printf("try to relloc memory but memory is not in memoryTreeMemory\n");
        return NULL;
    }
    if(memory == NULL){
        return mallocMemory(size);
    }else{
        void *newMemory = realloc(memory , size);
        if(newMemory == NULL){
            printf("realloc error , rellocMemory failed\n");
            return NULL;
        }else if(newMemory == memory){
            return memory;
        }else{
            deleteElementLRTree(memory , memoryTreeMemory);
            insertDataLRTree(newMemory , memoryTreeMemory);
            return newMemory;
        }
    }
}

void freeMemory(void *memory){
    if(memoryTreeMemory == NULL){
        printf("try to free memory but memoryTreeMemory is NULL\n");
        return ;
    }
    if(getDataFromTreeLRTree(memory , memoryTreeMemory) == NULL){
        printf("try to free memory but memory is not in memoryTreeMemory\n");
        return ;
    }
    free(memory);
    deleteElementLRTree(memory , memoryTreeMemory);
}

int isMemoryAllFreeed(){
    if(memoryTreeMemory == NULL){
        return 1;
    }
    return memoryTreeMemory->root == NULL;
}
