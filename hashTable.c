#include"./linkList.c"
#include <stdint.h>

typedef struct HashTable{
    LinkList * datas;
    int tableSize;
    int curDataSize;
    void * (*getKeyFunc)(void *);
    int (*compareFunc)(void * , void *);
    int (*hashFunc)(void *);
}HashTable;

typedef struct HashTableNodeHTable{
    void ** data;
}HashTableNodeHTable;
//创建hashtable，传入hash函数（key），比较key函数，获取key函数
HashTable *createHashTable(int dataSize , void *(*getKeyFunc)(void *), int (*compareFunc)(void *, void *), int (*hashFunc)(void *));
//插入数据,如果key已经存在，插入失败
void insertElementHTable(void * data , HashTable * table);
//获取key对应的data
void *getElementsHTable(void * key , HashTable * table);
//销毁hashtable
void destroyHTable(HashTable * table);

const int TABLE_SIZE_HASH_TABLE_DEFAULT = 100;


int compare_default_HTable(void * key1, void * key2){
    return *(int *)key1  - *(int *)key2;
}

void * get_key_default_HTable(void * data){
    return data;
}

int hash_default_HTable(void * key){
    return key;
}

HashTableNodeHTable * create_hash_table_node_HTable(){
    HashTableNodeHTable * node = (HashTableNodeHTable *)malloc(sizeof(HashTableNodeHTable));
    node->data = (void **)malloc(sizeof(void *)*TABLE_SIZE_HASH_TABLE_DEFAULT);
    for(int i = 0 ;  i < TABLE_SIZE_HASH_TABLE_DEFAULT ; i++){
        node->data[i] = NULL;
    }
    return node;
}

HashTable *createHashTable(int dataSize , void *(*getKeyFunc)(void *), int (*compareFunc)(void *, void *), int (*hashFunc)(void *)){
    HashTable * table = (HashTable *)malloc(sizeof(HashTable));
    table->datas = createLinkListLList(NULL , NULL, NULL);
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
    return table;
}

int get_hash_position_data_HTable(void * data , HashTable * table){
    return table->hashFunc(table->getKeyFunc(data)) % table->tableSize;
}

int get_hash_position_key_HTable(void * key , HashTable * table){
    return table->hashFunc(key) % table->tableSize;
}

void * get_data_from_position_HTable(int position , HashTable * table){
    int tableIndex = position / TABLE_SIZE_HASH_TABLE_DEFAULT;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , table->datas);
    return node->data[position % TABLE_SIZE_HASH_TABLE_DEFAULT];
}

void * get_data_from_position_fromlist_HTable(int position , LinkList * list){
    int tableIndex = position / TABLE_SIZE_HASH_TABLE_DEFAULT;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , list);
    return node->data[position % TABLE_SIZE_HASH_TABLE_DEFAULT];
}


int is_empty_position_HTable(int position , HashTable * table){
    void * data = get_data_from_position_HTable(position , table);
    return data == NULL;
}

void set_data_by_position_HTable(int position , void * data , HashTable * table){
    int tableIndex = position / TABLE_SIZE_HASH_TABLE_DEFAULT;
    HashTableNodeHTable * node = getElementByIndexLList(tableIndex , table->datas);
    node->data[position % TABLE_SIZE_HASH_TABLE_DEFAULT] = data;
}

void destroy_link_list_HTable(LinkList * list , int tableSize){
    int i = 0;
    while(tableSize > 0){
        HashTableNodeHTable * node = getElementByIndexLList(i , list);
        free(node->data);
        free(node);
        tableSize -= TABLE_SIZE_HASH_TABLE_DEFAULT;
        i ++;
    }
    destroyLList(list);
}

void insert_element_to_table_HTable(void * data , HashTable * table){
    int position = get_hash_position_data_HTable(data , table);
    while(!is_empty_position_HTable(position , table)){
        void * positionData = get_data_from_position_HTable(position , table);
        if(table->compareFunc(table->getKeyFunc(data) , table->getKeyFunc(positionData)) == 0){
            return;
        }
        position++;
        if(position == table->tableSize){
            position = 0;
        }
    }
    set_data_by_position_HTable(position , data , table);
}

void re_hash_HTable(HashTable * table){
    LinkList * oldDatas = table->datas;
    table->tableSize *= 2;
    int newTableSize = table->tableSize;
    table->datas = createLinkListLList(NULL , NULL,NULL);
    while (newTableSize > 0){
        HashTableNodeHTable * node = create_hash_table_node_HTable();
        insertElementLList(node , table->datas);
        newTableSize -= TABLE_SIZE_HASH_TABLE_DEFAULT;
    }
    for(int i = 0 ; i < table->tableSize/2 ; i++){
        void * data = get_data_from_position_fromlist_HTable(i , oldDatas);
        if(data != NULL){
            insert_element_to_table_HTable(data , table);
        }
    }
    destroy_link_list_HTable(oldDatas , table->tableSize/2);
}

void insertElementHTable(void * data , HashTable * table){
    if(table->curDataSize >= table->tableSize * 0.5){
        re_hash_HTable(table);
    }
    insert_element_to_table_HTable(data , table);
    table->curDataSize++;
}

void * getElementsHTable(void * key , HashTable * table){
    int position = get_hash_position_key_HTable(key , table);
    while(!is_empty_position_HTable(position , table)){
        void * data = get_data_from_position_HTable(position , table);
        if(table->compareFunc(table->getKeyFunc(data) , key) == 0){
            return data;
        }
        if(position == table->tableSize){
            position = 0;
        }
        position++;
    }
    return NULL;
}

void destroyHTable(HashTable * table){
    destroy_link_list_HTable(table->datas , table->tableSize);
    free(table);
}