#include "./optimalSearchTree.c"
typedef struct data{
    int key;
    int value;
}Data;

int compare(void *data1,void *data2){
    return ((Data *)data1)->key - ((Data *)data2)->key;
}

int getPrior(void *data){
    return ((Data *)data)->value;
}

void show(void *data){
    printf("(%d  %d)",((Data *)data)->key , ((Data *)data)->value);
}
int main(){
    int num;
    scanf("%d",&num);
    Data **datas = (Data **)malloc(sizeof(Data *) * num);
    for(int i = 0;i < num;i++){
        Data *data = (Data *)malloc(sizeof(Data));
        data->key = rand() % 100;
        data->value = rand() % 100;
        datas[i] = data;
    }
    OSTTree *tree = createOSTTree(datas , num , getPrior ,compare);
    showTreeDataOST(tree , show);
    Data *data = (Data *)malloc(sizeof(Data));
    data->key = 27;
    data->value = 36;
    insertDataOST(tree , data , getPrior , compare);
    showTreeDataOST(tree , show);
}
