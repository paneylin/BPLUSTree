#include "./sort.h"

int compare(void *data1,void *data2){
    return *(int *)data1 - *(int *)data2;
}

void show_data(void **data , int dataSize){
    for(int i = 0 ; i < dataSize ; i ++){
        printf("%d ",*(int *)data[i]);
    }
    printf("\n");
}

void test_quick(){
    int num;
    scanf("%d",&num);
    int **datas = (int **)malloc(sizeof(int*) * num);
    for(int i = 0;i < num;i++){
        int *data = (int *)malloc(sizeof(int*));
        *data = rand() % 10000;
        datas[i] = data;
    }
    quickSort(datas , num , compare);
    for(int i = 0 ; i < num - 1; i ++){    
        if(compare(*(datas + i) , *(datas + i + 1)) > 0){
            printf("error\n");
        }
    }
    int *data = (int *)malloc(sizeof(int*));
    *data = 0;
    int position = searchPrevDataBinarySort(datas , num ,data , compare);
    printf("%d\n",position);
    printf("success\n");
}

void test_select(){
    int num;
    scanf("%d",&num);
    int **datas = (int **)malloc(sizeof(int*) * num);
    for(int i = 0;i < num;i++){
        int *data = (int *)malloc(sizeof(int*));
        *data = rand() % 10000;
        datas[i] = data;
    }
    selectSort(datas , num , compare);
    for(int i = 0 ; i < num - 1; i ++){    
        if(compare(*(datas + i) , *(datas + i + 1)) > 0){
            printf("error\n");
        }
    }
    printf("success\n");
}

void test_combind(){
    int num;
    scanf("%d",&num);
    int **datas = (int **)malloc(sizeof(int*) * num);
    for(int i = 0;i < num;i++){
        int *data = (int *)malloc(sizeof(int*));
        *data = rand() % 10000;
        datas[i] = data;
    }
    combindSort(datas , num , compare);
    for(int i = 0 ; i < num - 1; i ++){    
        if(compare(*(datas + i) , *(datas + i + 1)) > 0){
            printf("error\n");
        }
    }
    printf("success\n");
}

void test_insert(){
    int num;
    scanf("%d",&num);
    int **datas = (int **)malloc(sizeof(int*) * num);
    for(int i = 0;i < num;i++){
        int *data = (int *)malloc(sizeof(int*));
        *data = rand() % 10000;
        datas[i] = data;
    }
    insertSort(datas , num , compare);
    for(int i = 0 ; i < num - 1; i ++){    
        if(compare(*(datas + i) , *(datas + i + 1)) > 0){
            printf("error\n");
        }
    }
    printf("success\n");
}

int main(){
    test_quick();
    test_insert();
    test_combind();
    test_select();
}