#include"./graph.h"

int compareEDges(ArrayList * edges1 , ArrayList *edges2){
    int size = getSizeAList(edges1);
    if(size != getSizeAList(edges2)){
        //printf("size is not equal\n");
        return 0;
    }
    for(int i = 0 ; i < size ; i ++){
        NodeVlinkGraph * node1 = getElementByIndexAList(i , edges1);
        NodeVlinkGraph * node2 = getElementByIndexAList(i , edges2);
        if(node1->w != node2->w){
            //printf("weight is not equal %d , %d ,%d , %d , size is %d\n" , node1->w , node1->u , node2->w ,node2->u, size);
            return 0;
        }
    }
    return 1;
}

int testGraphjoin(VLinkGraph * graph){
    TreeLRTree * tree = createTreeLRTree(NULL , NULL);
    CircleQueue *queue = createCircleQueue(NULL);
    int min = -1;
    insertDataLRTree(&min , tree);
    int start = 0;
    do{
        insertDataLRTree(&start , tree);
        pushCircleQueue(&start , queue);
        while(!isEmptyCircleQueue(queue)){
            int i = *(int *)popCircleQueue(queue);
            int size = getSizeAList(graph->adj[i]);
            //printf("v = %d , graph v is %d , size is %d\n" , i , graph->v , size);
            for(int j = 0 ; j < size ; j ++){
                NodeVlinkGraph * node = getElementByIndexAList(j , graph->adj[i]);
                if(!getDataFromTreeLRTree(&node->u , tree)){
                    insertDataLRTree(&node->u , tree);
                    pushCircleQueue(&node->u , queue);
                }
            }
        }
        int *joinPoint = (int *)popMinDataLRTree(tree);
        if(*joinPoint >= start){
            printf("graph is not a connected graph , second Graph not connected to firstGraph %d , %d\n" , *joinPoint , start);
            return 0;
        }
        for(int i = start ; i < *(int*)getMaxDataLRTree(tree) ; i ++){
            if(!getDataFromTreeLRTree(&i , tree)){
                printf("graph is not a connected graph , has node unconnnected , %d , %d\n" , i , graph->v);
                return 0;
            }
        }
        start = *(int*)getMaxDataLRTree(tree) + 1;
        while(!isEmptyLRTree(tree)){
            popMinDataLRTree(tree);
        }
    }while(start < graph->v);
    destroyCircleQueue(queue);
    destroyTreeLRTree(tree);
    return 1;
}

int testSubGraph(VLinkGraph * graph , ArrayList * subGraphs){
    int size = getSizeAList(subGraphs);
    printf("subGraph num is %d\n" , size);
    int v = 0;
    int e = 0;
    for(int i = 0 ; i < size ; i ++){
        VLinkGraph * subgraph = getElementByIndexAList(i , subGraphs);
        v += subgraph->v;
        e += subgraph->e;
        if(!testGraphjoin(subgraph)){
            printf("subgraph %d is not a connected graph\n" , i);
            return 0;
        }
        printf("test subgraph %d\n" , i);
        for(int j = 0 ; j < subgraph->v ; j ++){
            int flag = 0;
            for(int k = 0 ; k < graph->v ; k ++){
                if(compareEDges(subgraph->adj[j] , graph->adj[k])){
                    flag = 1;
                    break;  
                }
            }
            if(!flag){
                printf("subgraph is not a subgraph of graph\n");
                return 0;
            }
        }
    }
    if(graph->v != v || graph->e != e){
        printf("subgraph is not a subgraph of graph , v is %d , %d , e is %d , %d\n", graph->v , v , graph->e , e);
        return 0;
    }
    printf("subgraph is a subgraph of graph\n");
    return 1;
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
        insertEdgeVLinkDirectGraph(v , u , weight , vGraph);
    }
    //showVLinkGraph(vGraph);
    printf("graph create success\n");
    ArrayList *subGraph = getSubGraphGraph(vGraph);
    //showVLinkGraph(vGraph);
    printf("subFraphs create success\n");
    testSubGraph(vGraph , subGraph);
    int rsl = isCircleGraph(vGraph);
    printf("isCircle is %d\n" , rsl);
    printf("end\n");
}
