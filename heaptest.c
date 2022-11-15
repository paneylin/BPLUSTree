#include"./heap.c"

int main()
{
    HeapInfo *heapInfo = createHeapInfoHeap(100,NULL);
    int num;
    scanf("%d",&num);
    int ** datas = malloc(sizeof(int *) * num);
    for(int i = 0 ; i < num ; i++){
        int *data = (int *)malloc(sizeof(int));
        *data = rand() % 10000;
        datas[i] = data;
        //insertElement(data,heapInfo);
    }
    insertElementsHeap((void **)datas,num,heapInfo);
    //printf("%d\n",changeTime);
    for(int i = 0 ; i < num ; i ++){
        printf("%d ",*(int *)pushElementHeap(heapInfo));
    }
    while(1){
        scanf("%d",&num);
    }
}