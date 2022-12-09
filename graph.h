#ifndef GRAPH_H
#define GRAPH_H

#include"./arrayList.h"
#include"./leftRightTree.h"
#include"./hashTable.h"
#include"./CircleQueue.h"
#include"./stack.h"

typedef struct VLinkGraph{
    int v;
    int e;
    ArrayList **adj;
}VLinkGraph;

typedef struct NodeVlinkGraph{
    int u;
    int w;
}NodeVlinkGraph;

typedef struct VMutrixGraph{
    int v;
    int e;
    int ** adj;
}VMutrixGraph;

typedef struct PathGraph{
    int v;
    int u;
    int w;
}PathGraph;

typedef struct DistanceGraph{
    PathGraph **path;
    int pathNum;
    int distance;
}DistanceGraph;

typedef struct PointRelation{
    int sourceP;
    int targetP;
    int targetGraphNum;
}PointRelation;

const int PATH_NOT_EXSIT_GRAPH = -1;
const int UNREACHABLE_GRAPH = -1;
//创建邻接表类型的图，输入图的顶点
VLinkGraph * createVLinkGraph(int v);
//增加邻接表类型有向图的边，输入边起点v，终点u，以及边权重w
void insertEdgeVLinkDirectGraph(int v , int u , int w , VLinkGraph * graph);
//增加邻接表类型无向图的边，输入边起点v，终点u，以及边权重w
void insertEdgeVLinkUnDirectGraph(int v , int u , int w , VLinkGraph * graph);
//创建矩阵表类型的图，输入图的顶点
VMutrixGraph * createVMutrixGraph(int v);
//增加矩阵类型有向图的边，输入边起点v，终点u，以及边权重w
void insertEdgeVMutrixDirectGraph(int v , int u , int w , VMutrixGraph * graph);
//增加矩阵类型无向图的边，输入边起点v，终点u，以及边权重w
void insertEdgeVMutrixDirectGraph(int v , int u , int w , VMutrixGraph * graph);
//将邻接表类型的图转为矩阵类型的图；
VMutrixGraph *changeLinktoMutrixGraph(VLinkGraph * graph);
//将矩阵类型的图转为邻接表类型的图；
VLinkGraph * changeMutrixtoLinkGraph(VMutrixGraph * graph);
//将一个图分割成几个子图，子图之间互相不连通
ArrayList *getSubGraphGraph(VLinkGraph *graph);
//销毁图，并释放内存
void destroyVlinkGraph(VLinkGraph *graph);
//销毁图，并释放内存
void destroyVMutrixGraph(VMutrixGraph *graph);
//判断无向图是否循环
int isCircleUndirectGraph(VLinkGraph * graph);
//判断有向图是否循环
int isCircleGraph(VLinkGraph *graph);
//获取无向图连通子图
ArrayList *getSubGraphsUnDirectGraph(VLinkGraph *graph);
//获取有向图连通子图
ArrayList *getSubGraphGraph(VLinkGraph * graph);

#include"./graph.c"

#endif  