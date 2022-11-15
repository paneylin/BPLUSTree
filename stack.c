#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//双向循环链表
typedef struct Stack{
    void *num;
    struct stack *next;
}Stack;

typedef struct StackInfo{
    Stack *head ,*tail;
}StackInfo;

StackInfo * create_stack_info(){
    StackInfo *stack = (StackInfo *)malloc(sizeof(StackInfo));
    stack->head = stack->tail = NULL;
    return stack;
}

StackInfo *stack = NULL;

Stack *createStack(void *num){
    Stack *temp = (Stack *)malloc(sizeof(Stack));
    temp->num = num;
    temp->next = NULL;
    return temp;
}

void push(void *num){
    if(stack == NULL){
        stack = create_stack_info();
    }
    Stack *temp = createStack(num);
    if(stack->head == NULL){
        stack->head = stack->tail = temp;
    }else{
        temp->next = stack->head;
        stack->head = temp;
    }
}

void *pop(){
    if(stack == NULL || stack->head == NULL){
        printf("stack is empty\n");
        return NULL;
    }
    Stack *temp = stack->head;
    stack->head = stack->head->next;
    if(stack->head == NULL){
        stack->tail = NULL;
    }
    int rsl = temp->num;
    free(temp);
    return rsl;
}

void destroyStack(){
    while(stack->head != NULL){
        Stack *temp = stack->head;
        stack->head = stack->head->next;
        free(temp);
    }
    stack->head = stack->tail = NULL;
    free(stack);
    stack = NULL;
}

int main()
{
    int temp = 0;
    push(1);
    push(2);
    push(3);
    temp = pop();
    printf("%d\n",temp);
    push(4);
    push(5);
    while (temp != NULL)
    {
        temp = pop();
        printf("%d\n",temp);
    }
    
    destroyStack();
    return 0;   
}