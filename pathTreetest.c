#include"./pathTree.h"

int count = 0;
int hasDistance(int v ,int u ,int curDistance , int distance , VLinkGraph *graph){
    printf("v:%d u:%d curDistance:%d distance:%d , %d\n",v,u,curDistance,distance,count++);
    if(curDistance >= distance){
        return 0;
    }
    if(v == u){
        return 1;
    }
    int num = getSizeAList(graph->adj[v]);
    for(int i = 0 ; i < num ; i++){
        NodeVlinkGraph * node = getElementByIndexAList(i , graph->adj[v]);
        if(hasDistance(node->u , u , curDistance + node->w , distance , graph)){
            return 1;
        }
    }
    return 0;
}

void testDIstance(int  distance , int v , int u , VLinkGraph * graph){
    if(hasDistance(v , u , 0 , distance , graph)){
        printf("test failed\n");
        return 0;
    }else{
        return 1;
    }
}

int main(){
    int num = 10;
    scanf("%d",&num);
    VLinkGraph *vGraph = createVLinkGraph(num);
    int e = 0;
    scanf("%d",&e);
    for(int i = 0 ;i < e ; i ++){
        int v , u , weight;
        v = rand()%num;
        u = rand()%num;
        weight = rand()%10000;
        if(v == u){
            continue;
        }
        insertEdgeVLinkUnDirectGraph(v , u , weight , vGraph);
    }
    printf("graph create success\n");
    int start = rand()%num;
    int end = rand()%num;
    //DistanceGraph * distance = getShortestDistancePathTree(start , end , vGraph);
    DistanceGraph * distance = getDiameterPathTreePathTree(vGraph);
    if(distance == NULL){
        printf("no path from %d to %d\n",start , end);
        return;
    }
    printf("start:%d end:%d , distance is %d\n",distance->path[0]->v , distance->path[distance->pathNum -1]->u , distance->distance);
    //testDIstance(distance->distance , start , end , vGraph);
    printf("test success\n");
}

