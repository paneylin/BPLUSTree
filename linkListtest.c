#include "./linkList.c"

void testListDataCorrect(LinkList *list, int **datas, int length) {
    for(int i = 0 ; i < length ; i ++){
        void * data = getElementByIndexLList(i , list);
        if(data != datas[i]){
            printf("error\n");
        }
    }
}

void testLinkList(LinkList *list , int ** datas , int num){
    testListDataCorrect(list , datas , num);
    for(int i = 0 ; i < num ; i++){
        int p = rand()%(num - i);
        deleteElementLList(datas[p], list);
        memmove(datas + p , datas + p + 1, sizeof(int *) * (num - i - p - 1));
        testListDataCorrect(list , datas , num - i - 1);
    }
    printf("test success\n");
}

int main(){
    LinkList * list = createLinkListLList(NULL , NULL , NULL);
    int num;
    scanf("%d" , &num);
    int **datas = (int **)malloc(sizeof(int *) * num);
    for(int i = 0 ; i < num ; i ++){
        int *data = (int *)malloc(sizeof(int));
        *data = rand()%1000;
        datas[i] = data;
        insertElementLList(data , list);
    }
    testLinkList(list , datas , num);
}