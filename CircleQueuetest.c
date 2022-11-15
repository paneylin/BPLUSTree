#include "./CircleQueue.h"

void showFunc(void *data){
    printf("%d ",*(int *)data);
}

int main()
{
    CircleQueue *queue = createCircleQueue();
    int num;
    scanf("%d",&num);
    for(int i = 0;i < num;i++){
        int *data = (int *)malloc(sizeof(int));
        *data = rand() % 100;
        printf("%d ",*data);
        pushCircleQueue(data,queue);
    }   
    printf("\n");
    /*while (!isEmptyCircleQueue(queue)){
        showFunc(popCircleQueue(queue));
    }*/
    showCirCleQueue(queue,showFunc);
    destroyCircleQueueQ(queue);
    printf("end");
    //showCirCleQueue(queue , showFunc);
}