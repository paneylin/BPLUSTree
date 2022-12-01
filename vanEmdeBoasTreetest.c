#include"./vanEmdeBoasTree.h"
#include<math.h>
#include"./sort.h"

int getKeyFunc(int  *data){
    return *data;
}

int compareKeyFunc(int * key1 , int * key2){
    return *key1 - *key2;
}

int testDataVEBTree(VanEmdeBoasTree * tree , int ** datas , int size){
    for(int i = 0 ; i < size ; i ++){
        if(findDataInVEBTree(*datas[i] , tree) == NULL){
            printf("error , can't find data %d in tree\n" , *datas[i]);
            return 0;
        }
    }
    printf("test data in success\n");
    int i = 0;
    int count = 0 ;
    void ** treeDatas;
    while((treeDatas = getElementByIndexLList(i , tree->data->dataList)) != NULL){
        for(int j = 0 ; j < DATA_BLOCK_SIZE_VEBTREE ; j ++){
            if(treeDatas[j] != NULL){
                count ++;
            }
        }
        i ++;
    }
    if(count != size){
        printf("error , tree size is %d , but datas size is %d\n" , count , size);
        return 0;
    }
    printf("test data success\n");
    return 1;
}

int testKeyinIndex(VanEmdeBoasTree * tree , int **datas , int size){
    NodeVanEmdeBoasTree * node = tree->root;
    for(int i = 0 ; i < size ; i ++){
        int key = *datas[i];
        while(node->lgNumber > 1){
            //printf("node->lgNumber %d = key is %d\n" , node->lgNumber , key);
            int highIndex = key /(node->u/node->nextU);
            int lowIndex = key % (node->u/node->nextU);
            if(!clusterExist(node , highIndex)){
                printf("error , cluster not exists ,%d index error,  lgnumber is %d tree\n" , key , node->lgNumber);
                return 0;   
            }
            node = node->cluster[node->summary[highIndex]->position];
            key = lowIndex;
        }
        if(*datas[i] % 2 == 0 && node->min != 0){
            printf("error , %d index error,  lgnumber is %d tree\n" , *datas[i] , node->lgNumber);
            return 0;   
        }
        if(*datas[i] % 2 == 1 && node->max != 1){
            printf("error , %d index error,  lgnumber is %d tree\n" , *datas[i] , node->lgNumber);
            return 0;   
        }
        node = tree->root;
    }
    printf("test key in index success\n");
    return 1;
}

int clusterExist(NodeVanEmdeBoasTree * node , int summaryIndex){
    return node->summary[summaryIndex]->position != -1 && node->cluster[node->summary[summaryIndex]->position] != NULL;
}

int testSeqNode(NodeVanEmdeBoasTree *node){
    if(node->lgNumber == 1){
        if(node->min == -1 || node->max == -1){
            printf("error ,min node min or max is -1\n");
            return 0;
        }
        return 1;   
    }
    if(node->max < node->min){
        printf("error , max is %d , min is %d\n" , node->max , node->min);
        return 0;
    }
    int key = node->min;
    int summaryIndex = key / (node->u/node->nextU);
    int preSummaryIndex = node->summary[summaryIndex]->pre->summaryPosition;
    if(preSummaryIndex != node->max / (node->u/node->nextU)){
        printf("error , max is %d , min is %d , max index is %d max is not the biggest key !\n", node->max , node->min , preSummaryIndex);
        return 0;
    }
    int i = 0 ;
    while(i < node->nextU){
        if(i == summaryIndex){
            if(!clusterExist(node , summaryIndex)){
                printf("error , cluster not exists %d index error,  lgnumber is %d tree\n" , summaryIndex , node->lgNumber);
                return 0;   
            }
            if(!testSeqNode(node->cluster[node->summary[summaryIndex]->position])){
                return 0;
            }
            preSummaryIndex = summaryIndex;
            summaryIndex = node->summary[summaryIndex]->next->summaryPosition;
        }else{
            if(node->summary[i]->next->summaryPosition != summaryIndex){
                printf("error , NULL value next not correct , %d index error,  lgnumber is %d tree\n" , i , node->lgNumber);
                return 0;   
            }
            if(node->summary[i]->pre->summaryPosition != preSummaryIndex){
                printf("error , NULL value pre not correct , %d index error,  lgnumber is %d tree\n" , i , node->lgNumber);
                return 0;   
            }
            if(clusterExist(node , i)){
                printf("error , cluster exists %d index error,  lgnumber is %d tree\n" , i , node->lgNumber);
                return 0;   
            }
        }
        i ++;
    }
}

int testSeq(VanEmdeBoasTree *tree){
    NodeVanEmdeBoasTree * node = tree->root;
    if(!testSeqNode(node)){
        return 0; 
    }
    printf("test seq node success\n");
    return 1;
}

int testIndexVEBTree(VanEmdeBoasTree *tree , int **datas , int size){
    if(!testKeyinIndex(tree , datas , size)){
        return 0;
    }
    if(!testSeq(tree)){
        return 0;
    }
    printf("test index success\n");
    return 1;
}

int testPreNextFunction(VanEmdeBoasTree * tree , int ** data , int size){
    for(int i = 0 ; i < 100 ; i ++){
        int key = rand() % 10000;
        int * pre = getPreDataVEBTree(key , tree);
        int * next = getNextDataVEBTree(key , tree);
        int prePosition = searchPrevDataBinarySort(data , size , &key, compareKeyFunc);
        printf("key is %d , pre is %d , next is %d , prePosition is %d , prePosition data is %d\n" , key , pre == NULL ? -1 : *pre , next == NULL ? -1 : *next , prePosition , prePosition == -1 ? -1 : *data[prePosition]);
        if(prePosition != -1){
            if(prePosition == 0 && *data[prePosition] == key){
                if(pre != NULL){
                    printf("error , pre is not null , but key is the first data\n");
                    return 0;
                }
                if(*next != *data[prePosition + 1]){
                    printf("error , next is not correct ,key is the smallest\n");
                    return 0;
                }
            }else if(prePosition == size - 1){
                if(next != NULL){
                    printf("error , next is not null , but key is >= the last data\n");
                    return 0;
                }
                if(*data[prePosition] == key && *pre != *data[prePosition -1]){
                    printf("error , pre is not correct ,key is the biggest\n");
                    return 0;
                }
                if(*data[prePosition] != key && *pre != *data[prePosition]){
                    printf("error , pre is not correct ,key is > the biggest\n");
                    return 0;
                }
            }else if(*data[prePosition] == key){
                if(*pre != *data[prePosition - 1]){
                    printf("error , pre is not correct  , key is in datas\n");
                    return 0;
                }
                if(*next != *data[prePosition + 1]){
                    printf("error , next is not correct , key is in datas\n");
                    return 0;
                }
            }else{
                if(*pre != *data[prePosition]){
                    printf("error , pre is not correct ,key is not in data prePosition is %d ,pre is %d ,key is %d , preData is %d\n", prePosition , *pre , key ,*data[prePosition]);
                    return 0;
                }
                if(*next != *data[prePosition + 1]){
                    printf("error , next is not correct ,key is not in data\n");
                    return 0;
                }
            }
        }else{
            if(pre != NULL){
                printf("error , pre is not null , but key is < the first data\n");
                return 0;
            }
            if(*next != *data[0]){
                printf("error , next is not correct ,key is < the first data\n");
                return 0;
            }
        }
    }
    printf("test pre next function success\n");
    return 1;
}

int testVEBTree(VanEmdeBoasTree * tree , int ** data , int size){
    if(!testDataVEBTree(tree , data , size)){
        return 0;
    }
    if(!testIndexVEBTree(tree , data , size)){
        return 0;
    }
    if(!testPreNextFunction(tree , data , size)){
        return 0;
    }
    printf("test success\n");
    return 1;
}

int main(){
    int num = 0;
    scanf("%d",&num);
    VanEmdeBoasTree * tree = createVanEmdeBoasTree(10000 , getKeyFunc);
    int ** datas = (int **)malloc(sizeof(int*) * num);
    for(int i = 0;i < num ;){
        int *data = (int *)malloc(sizeof(int));
        *data = rand() % 10000;
        if(findDataInVEBTree(*data , tree) == NULL){
            insertDataVEBTree(data , tree);
            datas[i] = data;
            i++;
        }
    }
    printf("insert data success\n");
    quickSort(datas ,  num , compareKeyFunc);
    if(!testVEBTree(tree , datas , num)){
        printf("test failed\n");
        return;
    }
    for(int i = num - 1 ; i >= 0 ; i --){
        deleteDataVEBTree(*datas[i] , tree);
        free(datas[i]);
        if(!testVEBTree(tree , datas , i)){
            printf("test failed\n");
            return;
        }
    }
    printf("delete data success\n");
}