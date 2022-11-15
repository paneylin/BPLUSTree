#include"./hashTable.c"

int getHashValue(int *data){
    return *(int *)data;
}

void show(int *data){
    printf("%d " , *data);
}

int * getKeyForList(void *data){
    int *key = malloc(sizeof(int));
    *key = *(int *)data;
    return key;
}

void freeKeyForList(void *key){
    free(key);
}

void testPosition(HashTable *table){
    int tableSize = table->tableSize;
    printf("tableSize = %d\n" , tableSize);
    for(int i = 0 ; i < tableSize ; i ++){
        int *data = (int *)get_data_from_position_HTable(i , table);
        int dataPosition = i;
        if(data != NULL){
            int hashValue = getHashValue(data);
            int rightPosition = hashValue % tableSize;
            LinkList * list = createLinkListLList(getKeyForList , NULL , NULL);
            while(dataPosition != rightPosition){
                insertElementLList(&dataPosition , list);
                dataPosition = rightPosition;
                data = (int *)get_data_from_position_HTable(dataPosition , table);
                if(data == NULL){
                    printf("error in position1\n");
                    break;
                }
                hashValue = getHashValue(data);
                rightPosition = hashValue % tableSize;
                if(getElementLList(&rightPosition , list) != NULL){
                    printf("error in position2 %d %d\n" , dataPosition , rightPosition ,*data);
                    showLList(list , show);
                    break;
                }
            }
            destroyLList(list);
        }
    }
}

void testHashTable(HashTable * table , int ** datas , int num){
    for(int i = 0 ; i < num ; i ++){
        int * data = getElementsHTable(datas[i] , table);
        if(data == NULL || data != datas[i]){
            printf("error data not exist %d\n", datas[i]);
        }
    }
    testPosition(table);
    printf("test success\n");
}

int main(){
    HashTable * table = createHashTable(10 , NULL , NULL , getHashValue);
    printf("tableSize = %d %d\n" , sizeof(table) , sizeof(*table));
    int num;
    scanf("%d" , &num);
    int **datas = (int **)malloc(sizeof(int *) * num);
    for(int i = 0 ; i < num ; ){
        int *data = (int *)malloc(sizeof(int));
        *data = rand()%1000;
        if(getElementsHTable(data , table) == NULL){
            printf("%d  %d\n" , i , *data);
            insertElementHTable(data , table);
            datas[i] = data;
            i++;
        }else{
            free(data);
        }
    }
    printf("insert success\n");
    testHashTable(table , datas , num); 
}