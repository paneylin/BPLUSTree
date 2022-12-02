#ifndef Graph_h
#define Graph_h

#include"./arrayList.h"

typedef struct VLinkGraph{
    int v;
    int e;
    ArrayList **adj;
}VLinkGraph;

typedef struct NodeVlinkGraph{
    int vIndex;
    int w;
}NodeVListGraph;

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
    PathGraph *path;
    int distance;
}NodeVMutrixGraph;

const int PATH_NOT_EXSIT_GRAPH = -1;
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

#include"./graph.c"

#endif  