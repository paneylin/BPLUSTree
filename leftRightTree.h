#ifndef LeftRightTree_H
#define LeftRightTree_H

#include"./arrayList.h"

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
    void (*freeDataFunc)(void *);
}TreeLRTree;

const double DEFAULT_BALANCE_FACTOR_LRTREE = 0.7;

//创建二叉树，传入比较key函数，获取key函数，key对象释放函数，默认key就是data并且不释放
TreeLRTree *createTreeLRTree(int (*compareFunc)(void *key1 , void *key2) , void *(*getKeyFunc)(void* data));
//设置释放key函数，用于节点释放时候key的释放，不设置则不释放
void setFreeKeyFuncLRTree(void (*freeKeyFunc)(void * key) , TreeLRTree * tree);
//设置释放data函数，用于节点释放时候key的释放，不设置则不释放
void setFreeDataFuncLRTree(void (*freeDataFunc)(void * data) , TreeLRTree * tree);
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
//获取二叉树的所有key，按照从小到大排列
ArrayList * getTreeAllKeysLeftToRightLRTree(TreeLRTree * tree);
//销毁二叉树释放内存
void destroyTreeLRTree(TreeLRTree * tree);
//删除二叉树key节点
void deleteElementLRTree(void * key , TreeLRTree * tree);
//返回树是否为空
int isEmptyLRTree(TreeLRTree * tree);
//返回树的最小节点并删除节点，不能设置freeKeyFunc和freeDataFunc
void *popMinDataLRTree(TreeLRTree * tree);
//返回树的最大节点数据并删除改节点，不能设置freeKeyFunc和freeDataFunc
void *popMaxDataLRTree(TreeLRTree * tree);

#include "./LeftRightTree.c"

#endif