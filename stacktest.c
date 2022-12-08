#include"./stack.h"

int main(){
    Stack * stack = createStackStack();
    for(int i = 0 ; i < 10 ; i ++){
        int * j = (int *)malloc(sizeof(int));
        *j = i;
        pushStack(j , stack);
    }
    while(!isEmptyStack(stack)){
        int * j = (int *)popStack(stack);
        printf("%d " , *j);
    }
    destroyStack(stack);
    return 0;
}