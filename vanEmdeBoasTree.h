#ifndef VANEMDEBOASTREE_H
#define VANEMDEBOASTREE_H

#include<stdio.h>
#include"./linklist.h"

typedef struct SummaryVEBTree{
    int summaryPosition;
    int position;
    struct SummaryVEBTree * pre;
    struct SummaryVEBTree * next;
}SummaryVEBTree;

 typedef struct NodeVanEmdeBoasTree{
    int u;
    int nextU;
    int lgNumber;
    int min;
    int max;
    int maxSize;
    int clusterSize;
    SummaryVEBTree **summary;
    struct NodeVanEmdeBoasTree ** cluster;
}NodeVanEmdeBoasTree;

typedef struct DataVanEmdeBoasTree{
    LinkList * dataList;
}DataVanEmdeBoasTree;

typedef struct vanEmdeBoasTree{
    NodeVanEmdeBoasTree * root;
    int dataSize;
    DataVanEmdeBoasTree * data;
    int (*getKeyFunc)(void *);
}vanEmdeBoasTree;

const int DEFAULT_DIVICE_NUM = 2;
const int MIN_MEMORRY_SIZE = 16;
const int DATA_BLOCK_SIZE = 4096;

//创建一个vanEmdeBoasTree，传入u值大小范围，传入获取key的函数，key为整数
vanEmdeBoasTree *createVanEmdeBoasTree(int number , int (*getKeyFunc)(void *));
//找到key对应的数据，没有返回null
void *findDataInVEBTree(int key , vanEmdeBoasTree * tree);
//插入数据，如果数据已经存在，返回，数据key不能冲突
void insertDataVEBTree(void *data , vanEmdeBoasTree *tree);
//获取当前比输入key大的第一个数据，key所代表数据可以不存在
void * getNextDataVEBTree(int key , vanEmdeBoasTree * tree);
//获取输入key小的第一个数据，key所代表数据可以不存在
void * getPreDataVEBTree(int key , vanEmdeBoasTree * tree);
//删除key所代表数据，返回删除的数据，如果不存在，返回null
void *deleteDataVEBTree(int key , vanEmdeBoasTree * tree);

#include"./vanEmdeBoasTree.c"
#endif