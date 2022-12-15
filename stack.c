StackNode * create_stack_node_Stack(void *data){
    StackNode *node = (StackNode *)(malloc(sizeof(StackNode)));
    node->data = data;
    node->next = NULL;
}

void destroy_stack_node_Stack(StackNode *node){
    node ->next =NULL;
    free(node);
    node = NULL;
}

void setFreeDataFuncStack(void (*freeDataFunc)(void *data) , Stack *stack){
    if(stack == NULL){
        printf("stack is null , set free data func failed\n");
        return;
    }
    stack->freeDataFunc = freeDataFunc;
}

Stack *createStackStack(){
    Stack *temp = (Stack *)malloc(sizeof(Stack));
    temp->head = NULL;
    temp->dataSize = 0;
    temp->freeDataFunc = NULL;
    return temp;
}

void pushStack(void *data , Stack *stack){
    if(stack == NULL){
        printf("stack is null , push failed\n");
        return;
    }
    StackNode *node = create_stack_node_Stack(data);
    if(stack->head == NULL){
        stack->head = node;
    }else{
        node->next = stack->head;
        stack->head = node;
    }
    stack->dataSize ++;
}

int isEmptyStack(Stack *stack){
    if(stack == NULL){
        printf("statkc is null\n");
        return 1;
    }
    return stack->head == NULL;
}

void *popStack(Stack *stack){
    if(isEmptyStack(stack)){
        printf("stack is empty\n");
        return NULL;
    }
    StackNode *node = stack->head;
    void *data = node ->data;
    stack->head = stack->head->next;
    destroy_stack_node_Stack(node);
    return data;
}

void destroyStack(Stack *stack){
    if(stack == NULL){
        return;
    }
    while(stack->head != NULL){
        StackNode *temp = stack->head;
        if(stack->freeDataFunc != NULL){
            stack->freeDataFunc(temp->data);
        }
        stack->head = stack->head->next;
        destroy_stack_node_Stack(temp);
    }
    stack->freeDataFunc = NULL;
    free(stack);
    stack = NULL;
}

int validDataExistStack(void *data , int (*compareFunc)(void * data1 , void *data2) , Stack *stack){
    if(isEmptyStack(stack)){
        printf("stack is null , data not exist\n");
        return 0;
    }
    StackNode *node = stack->head;
    while(node != NULL){
        if(compareFunc(node->data,data) == 0){
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void *getTopStack(Stack *stack){
    if(isEmptyStack(stack)){
        printf("stack is empty , get top failed\n");
        return NULL;
    }
    return stack->head->data;
}

ArrayList *getAfterDataStack(void *data , int (*compareFunc)(void * data1 , void *data2) , Stack *stack){
    if(isEmptyStack(stack)){
        printf("stack is empty , get after data failed\n");
        return NULL;
    }
    ArrayList *list = createArrayListAList(NULL);
    StackNode *node = stack->head;
    while(node != NULL){
        if(compareFunc(node->data,data) == 0){
            insertElementAList(node->data,list);
            return list;
        }
        insertElementAList(node->data,list);
        node = node->next;
    }
    return NULL;
}