#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include"./linkList.h"
#include"./arrayList.h"
#include <stdint.h>

typedef struct HashTable{
    LinkList * datas;
    int tableSize;
    int curDataSize;
    int blockSize;
    double hashFactor;
    void * (*getKeyFunc)(void *);
    int (*compareFunc)(void * , void *);
    int (*hashFunc)(void *);
    void (*freeDataFunc)(void *);
    void (*freeKeyFunc)(void *);
}HashTable;

typedef struct HashTableNodeHTable{
    void ** key;
    void ** data;
}HashTableNodeHTable;
//创建hashtable，传入hash函数（key），比较key函数，获取key函数
HashTable *createHashTable(int dataSize , void *(*getKeyFunc)(void *), int (*compareFunc)(void *, void *), int (*hashFunc)(void *));
//设置释放key函数，不设置不释放
void setFreeKeyHTable(void (*freeKeyFunc)(void * key) , HashTable * table);
//设置释放data函数，不设置不释放
void setFreeDataHTable(void (*freeDataFunc)(void * data) , HashTable * table);
//设置hashtable一块数据大小
void setBlockSizeHTable(int blockSize , HashTable * table);
//设置hashfactor，hash表的数据满载率
void setHashFactorHTable(double hashFactor , HashTable * table);
//插入数据,如果key已经存在，插入失败
void insertElementHTable(void * data , HashTable * table);
//获取key对应的data
void *getElementsHTable(void * key , HashTable * table);
//销毁hashtable
void destroyHTable(HashTable * table);
//获取hashtable中所有的数据，返回ArrayList
ArrayList * getAllDatasInTableHTable(HashTable * table);
//删除key对应的数据并返回该数据
void *deleteElementHTable(void * key , HashTable * table);

const int TABLE_SIZE_HASH_TABLE_DEFAULT = 100;

#include "./hashTable.c"

#endif