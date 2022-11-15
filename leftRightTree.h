#include"./arrayList.h"

#ifndef LeftRightTree_H
#define LeftRightTree_H

typedef struct TreeNodeLRTree
{
    void * key;
    void * data;
    int size;
    struct TreeNodeLRTree *parent;
    struct TreeNodeLRTree *left;
    struct TreeNodeLRTree *right;
}TreeNodeLRTree;

typedef struct TreeLRTree
{
    TreeNodeLRTree *root;
    double balanceFactor;
    int (*compareFunc)(void * , void *);
    void* (*getKeyFunc)(void *);
    void (*freeKeyFunc)(void *);
}TreeLRTree;

//创建二叉树，传入比较key函数，获取key函数，key对象释放函数，默认key就是data并且不释放
TreeLRTree *createTreeLRTree(int (*compareFunc)(void *key1 , void *key2) , void *(*getKeyFunc)(void* data) , void (*freeKeyFunc)(void * key));
//设置二叉树平衡因子函数
void setBalanceFactorLRTree(TreeLRTree *tree , double balanceFactor);
//插入数据
void insertDataLRTree(void * data , TreeLRTree * tree);
//根据key获取数据
void * getDataFromTreeLRTree(void * key , TreeLRTree * tree);
//中序遍历方式显示二叉树全部数据
void ShowTree(TreeLRTree *tree , void (*showNodeFunc)(void * key,void *data));
//ArrayList形式返回二叉树，数据按照key从小到大排序
ArrayList * getTreeAllDatasLeftToRightLRTree(TreeLRTree * tree);
//销毁二叉树释放内存
void destroyTreeLRTree(TreeLRTree * tree);

#include "./LeftRightTree.c"

#endif