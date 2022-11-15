#include "./hashTable.h"


int compare_default_HTable(void * key1, void * key2){
    return *(int *)key1  - *(int *)key2;
}

void * get_key_default_HTable(void * data){
    return data;
}

int hash_default_HTable(void * key){
    return key;
}

void setFreeKeyHTable(void (*freeKeyFunc)(void * key) , HashTable * table){
    table->freeKeyFunc = freeKeyFunc;
}

void setFreeDataHTable(void (*freeDataFunc)(void * data) , HashTable * table){
    table->freeDataFunc = freeDataFunc;
}

HashTableNodeHTable * create_hash_table_node_HTable(){
    HashTableNodeHTable * node = (HashTableNodeHTable *)malloc(sizeof(HashTableNodeHTable));
    node->data = (void **)malloc(sizeof(void *)*TABLE_SIZE_HASH_TABLE_DEFAULT);
    node->key = (void **)malloc(sizeof(void *)*TABLE_SIZE_HASH_TABLE_DEFAULT);
    for(int i = 0 ;  i < TABLE_SIZE_HASH_TABLE_DEFAULT ; i++){
        node->data[i] = NULL;
        node->key[i] = NULL;
    }
    return node;
}

HashTable *createHashTable(int dataSize , void *(*getKeyFunc)(void *), int (*compareFunc)(void *, void *), int (*hashFunc)(void *)){
    HashTable * table = (HashTable *)malloc(sizeof(HashTable));
    table->datas = createLinkListLList(NULL , NULL);
    if(dataSize == 0){
        dataSize =TABLE_SIZE_HASH_TABLE_DEFAULT;
    }
    table->tableSize = dataSize;
    while (dataSize > 0){
        HashTableNodeHTable * node = create_hash_table_node_HTable();
        insertElementLList(node , table->datas);
        dataSize -= TABLE_SIZE_HASH_TABLE_DEFAULT;
    }
    table->curDataSize = 0;
    table->getKeyFunc = getKeyFunc;
    if(getKeyFunc == NULL){
        table->getKeyFunc = get_key_default_HTable;
    }
    table->compareFunc = compareFunc;
    if(compareFunc == NULL){
        table->compareFunc = compare_default_HTable;
    }
    table->hashFunc = hashFunc;
    if(hashFunc == NULL){
        table->hashFunc = hash_default_HTable;
    }
    table->freeDataFunc = table->freeKeyFunc = NULL;
    table->blockSize = TABLE_SIZE_HASH_TABLE_DEFAULT;
    table->hashFactor = 0.5;
    return table;
}

int get_hash_position_key_HTable(void * key , HashTable * table){
    return table->hashFunc(key) % table->tableSize;
}

void * get_data_from_position_HTable(int position , HashTable * table){
    int tableIndex = position / table->blockSize;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , table->datas);
    return node->data[position % table->blockSize];
}

void * get_key_from_position_HTable(int position , HashTable * table){
    int tableIndex = position / table->blockSize;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , table->datas);
    return node->key[position % table->blockSize];
}

void * get_data_from_position_fromlist_HTable(int position , LinkList * list , int blockSize){
    int tableIndex = position / blockSize;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , list);
    return node->data[position % blockSize];
}

void* get_key_from_position_fromlist_HTable(int position , LinkList * list , int blockSize){
    int tableIndex = position / blockSize;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , list);
    return node->key[position % blockSize];
}

int is_empty_position_HTable(int position , HashTable * table){
    void * data = get_data_from_position_HTable(position , table);
    return data == NULL;
}

void set_data_by_position_HTable(int position ,void * key , void * data , HashTable * table){
    int tableIndex = position / table->blockSize;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , table->datas);
    node->data[position % table->blockSize] = data;
    node->key[position % table->blockSize] = key;
}

void destroy_node_HTable(HashTableNodeHTable * node , HashTable * table){
    if(table->freeDataFunc != NULL){
        for(int i = 0 ; i < table->blockSize ; i ++){
            table->freeDataFunc(node->data[i]);
            node->data[i] = NULL;
        } 
    }
    if(table->freeKeyFunc != NULL){
        for(int i = 0 ; i < table->blockSize ; i ++){
            table->freeKeyFunc(node->key[i]);
            node->key[i] = NULL;
        }
    }
    free(node->data);
    node->data = NULL;
    free(node->key);
    node->key = NULL;
    free(node);
    node = NULL;
}

void destroy_node_keep_data_HTable(HashTableNodeHTable * node){
    free(node->key);
    node->key = NULL;
    free(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
}

void destroy_node_list_HTable(LinkList * list){
    setFreeDataFuncLList(destroy_node_keep_data_HTable , list);
    destroyLList(list);
}

void insert_element_to_table_HTable(void * key ,void * data , HashTable * table){
    int position = get_hash_position_key_HTable(key , table);
    while(!is_empty_position_HTable(position , table)){
        void * positionKey = get_key_from_position_HTable(position , table);
        if(table->compareFunc(key , positionKey) == 0){
            printf("same key exist , insert fail\n");
            return;
        }
        position++;
        if(position == table->tableSize){
            position = 0;
        }
    }
    set_data_by_position_HTable(position , key , data , table);
}

void re_hash_factor_HTable(HashTable * table){
    LinkList * oldDatas = table->datas;
    table->tableSize *= 2;
    int newTableSize = table->tableSize;
    table->datas = createLinkListLList(NULL , NULL);
    while (newTableSize > 0){
        HashTableNodeHTable * node = create_hash_table_node_HTable();
        insertElementLList(node , table->datas);
        newTableSize -= table->blockSize;
    }
    for(int i = 0 ; i < table->tableSize/2 ; i++){
        void * data = get_data_from_position_fromlist_HTable(i , oldDatas , table->blockSize);
        void * key = get_key_from_position_fromlist_HTable(i , oldDatas , table->blockSize);
        if(data != NULL){
            insert_element_to_table_HTable(key ,data , table);
        }
    }
    destroy_node_list_HTable(oldDatas);
}

void re_hash_block_size_HTable(HashTable * table , int oldBlockSize){
    LinkList * oldDatas = table->datas;
    int newTableSize = table->tableSize;
    table->datas = createLinkListLList(NULL , NULL);
    while (newTableSize > 0){
        HashTableNodeHTable * node = create_hash_table_node_HTable();
        insertElementLList(node , table->datas);
        newTableSize -= table->blockSize;
    }
    for(int i = 0 ; i < table->tableSize/2 ; i++){
        void * data = get_data_from_position_fromlist_HTable(i , oldDatas , oldBlockSize);
        void * key = get_key_from_position_fromlist_HTable(i , oldDatas , oldBlockSize);
        if(data != NULL){
            insert_element_to_table_HTable(key ,data , table);
        }
    }
    destroy_node_list_HTable(oldDatas);
}

void setBlockSizeHTable(int blockSize , HashTable * table){
    int oldBlockSize = table->blockSize;
    table->blockSize = blockSize;
    re_hash_block_size_HTable(table , oldBlockSize);
}

void setHashFactorHTable(double hashFactor , HashTable * table){
    table->hashFactor = hashFactor;
    if(table->curDataSize >= table->tableSize * table->hashFactor){
        re_hash_factor_HTable(table);
    }
}

void insertElementHTable(void * data , HashTable * table){
    if(table->curDataSize >= table->tableSize * table->hashFactor){
        re_hash_factor_HTable(table);
    }
    void * key = table->getKeyFunc(data);
    insert_element_to_table_HTable(key ,data , table);
    table->curDataSize++;
}

void * getElementsHTable(void * key , HashTable * table){
    int position = get_hash_position_key_HTable(key , table);
    while(!is_empty_position_HTable(position , table)){
        void * positionkey = get_key_from_position_HTable(position , table);
        if(table->compareFunc(positionkey , key) == 0){
            return get_data_from_position_HTable(position , table);
        }
        position++;
        if(position == table->tableSize){
            position = 0;
        }
    }
    return NULL;
}

void destroyHTable(HashTable * table){
    int i = 0 ;
    while(table->tableSize > 0){
        HashTableNodeHTable * node = getElementByIndexLList(i , table->datas);
        destroy_node_HTable(node , table);
        i++;
        table->tableSize -= table->blockSize;
    }
    destroyLList(table->datas);
    free(table);
    table = NULL;
}