#ifndef PATH_TREE_H
#define PATH_TREE_H

#include"./graph.h"
#include"./leftRightTree.h"
#include"./CircleQueue.h"
#include"./heap.h"
#include"./hashTable.h"
#include"./memory.h"

typedef struct NodePathTree{
    int u;
    int height;
    int distance;
    struct NodePathTree *parent;
    struct NodePathTree *next;
    struct NodePathTree *pre;
    struct NodePathTree *child;
}NodePathTree;

typedef struct PathTree{
    NodePathTree *root;
    ArrayList *nodeList;
    int totalDistance;
}PathTree;

//销毁pathTree，并释放内存
void destroyPathTree(PathTree *tree);
//获取图上指定两点的最短路径，distanceGraph需自己销毁
DistanceGraph* getShortestDistancePathTree(int start , int end , VLinkGraph *graph);
//获取图上最长的最短路径
//DistanceGraph* getDiameterPathTreeUndirectPathTree(VLinkGraph *graph);
//生成最小生成树PRIM
PathTree *getGenericMSTPrimPathTree(int start , VLinkGraph *graph);

#include "./pathTree.c"

#endif