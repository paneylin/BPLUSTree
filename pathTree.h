#ifndef PATH_TREE_H
#define PATH_TREE_H

#include"./graph.h"
#include"./CircleQueue.h"

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
}PathTree;

#include "./pathTree.c"

#endif