#ifndef STACK_H
#define STACK_H

#include"./memory.h"
#include"./arrayList.h"

typedef struct StackNode{
    void *data;
    struct StackNode *next;
}StackNode;

typedef struct Stack{
    int dataSize;
    void (*freeDataFunc)(void *data);
    StackNode *head;
}Stack;
//设置stack的析构数据函数
void setFreeDataFuncStack(void (*freeDataFunc)(void *data) , Stack *stack);
//创建stack对象
Stack *createStackStack();
//向stack中压入数据
void pushStack(void *data , Stack *stack);
//判断stack是否为空
int isEmptyStack(Stack *stack);
//从stack中弹出数据
void *popStack(Stack *stack);
//销毁stack对象
void destroyStack(Stack *stack);
//判断stack中是否存在某个数据,compare值为0表示数值相同
int validDataExistStack(void *data , int (*compareFunc)(void * data1 , void *data2) , Stack *stack);
//获取栈顶数据但是不弹出
void *getTopStack(Stack *stack);
//获取data数据及其到栈顶数据之间的所有数据，但是不弹出，data数据不存在返回null
ArrayList *getAfterDataStack(void *data , int (*compareFunc)(void * data1 , void *data2) , Stack *stack);

#include"./stack.c"

#endif