#include "./arrayList.h"

void testListDataCorrect(ArrayList *list, int **datas, int length) {
    for(int i = 0 ; i < length ; i ++){
        void * data = getElementByIndexAList(i , list);
        if(data != datas[i]){
            printf("error\n");
        }
    }
}

void testLinkList(ArrayList *list , int ** datas , int num){
    testListDataCorrect(list , datas , num);
    for(int i = 0 ; i < num ; i++){
        int p = rand()%(num - i);
        deleteElementAList(datas[p], list);
        memmove(datas + p , datas + p + 1, sizeof(int *) * (num - i - p - 1));
        testListDataCorrect(list , datas , num - i - 1);
    }
    printf("test success\n");
}

int main(){
    ArrayList * list = createArrayListAList(NULL);
    int num;
    scanf("%d" , &num);
    int **datas = (int **)malloc(sizeof(int *) * num);
    for(int i = 0 ; i < num ; i ++){
        int *data = (int *)malloc(sizeof(int));
        *data = rand()%1000;
        datas[i] = data;
        insertElementAList(data , list);
    }
    testLinkList(list , datas , num);
}