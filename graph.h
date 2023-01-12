#ifndef GRAPH_H
#define GRAPH_H

#include"./arrayList.h"
#include"./leftRightTree.h"
#include"./hashTable.h"
#include"./CircleQueue.h"
#include"./stack.h"
#include"./memory.h"

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

typedef struct PathStronglyConnectedGraph{
    int v;
    int u;
    int w;
    int nodeIndex;
}PathStronglyConnectedGraph;

typedef struct DistanceGraph{
    PathGraph **path;
    int pathNum;
    int distance;
}DistanceGraph;

typedef struct PointRelationGragh{
    int sourceP;
    int targetP;
    //记录同组point信息，每个元素是一个PointRelation格式
    ArrayList *targetPointList;
}PointRelationGragh;

typedef struct PointSubGragh{
   HashTable *map;
   //记录图中点被分几类，每个类是一个arraylist，与PointRelationGragh中的list对应
   ArrayList *goupPoints;
}PointSubGragh;


typedef struct  StronglyConnectedGraph{
    //强连通图数量
    int nodeNum;
    //强连通图之间的连接路径数量
    int pathNum;
    //强连通图列表，每个元素是一个VLINkGraph图
    ArrayList *nodeList;
    //强连通图之间路径矩阵，类似VLINKGraph图的adj格式，记录了两个连通图之间连接点信息及权重
    ArrayList **adj;
}StronglyConnectedGraph;

typedef struct NodeDFSTreeGraph{
    int v;
    int height;
    int leftIndex;
    int rightIndex;
    int visitIndex;
    ArrayList *adjNodeList;
    struct NodeDFSTreeGraph *parent;
    struct NodeDFSTreeGraph *next;
    struct NodeDFSTreeGraph *pre;
    struct NodeDFSTreeGraph *child;
}NodeDFSTreeGraph;

typedef struct DFSTreeGraph{
    //森林的根节点，NodeDFSTreeGraph格式
    ArrayList *treeRoots;
    //根据传入的VLINkGraph图，生成的DNodeDFSTreeGraph节点
    ArrayList *nodeList;
}DFSTreeGraph;

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
//获取图的转置图
VLinkGraph *getRevertVLinkGraph(VLinkGraph * graph);
//判断邻接图是否无向图
int validUnDirectVLinkGraph(VLinkGraph * graph);
//判断矩阵图是否无向图
int validUnDirectVMutrixGraph(VMutrixGraph * graph);
//获取图的DFS访问树,starP为首先访问节点
DFSTreeGraph *getDFSTreeGraph(int startP , VLinkGraph * graph);
//销毁DFSTree访问树并释放内存
void destroyDFSTreeGraph(DFSTreeGraph * tree);
//判断无向图是否连通
int validConnectedUndirectGraph(VLinkGraph *graph);
//判断有向图连通,不能返回-1 ，如果能返回首访问节点
int validConnectedDirectGraph(VLinkGraph *graph);
//判断图是否有环
int isCircleGraph(VLinkGraph *graph);
//将一个图分割成几个子图，子图是连通图,子图以VLinkGraph格式返回
ArrayList *getSubGraphGraph(VLinkGraph *graph);
//将图分割成一个个强连通图，返回对应数据结构
StronglyConnectedGraph *getStronglyConnectedGraph(VLinkGraph *graph);
//销毁强连通子图并释放内存
void destroyStronglyConnectedGraph(StronglyConnectedGraph *stronglyConnectedGraph);
//销毁图，并释放内存
void destroyVlinkGraph(VLinkGraph *graph);
//销毁图，并释放内存
void destroyVMutrixGraph(VMutrixGraph *graph);



#include"./graph.c"

#endif  