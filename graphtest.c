#include"./graph.h"

int compareEDges(ArrayList * edges1 , ArrayList *edges2){
    int size = getSizeAList(edges1);
    if(size != getSizeAList(edges2)){
        return 0;
    }
    TreeLRTree *tree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < size ; i ++){
        NodeVlinkGraph * node1 = getElementByIndexAList(i , edges1);
        //printf("node1->w = %d\n",node1->w);
        insertDataLRTree(&node1->w , tree);
    }
    for(int i = 0 ; i < size ; i ++){
        NodeVlinkGraph * node2 = getElementByIndexAList(i , edges2);
        int *data = deleteElementLRTree(&node2->w , tree);
        if(data == NULL){
            printf("not found = %d\n",node2->w);
            destroyTreeLRTree(tree);
            return 0;
        }
    }
    destroyTreeLRTree(tree);
    return 1;
}

int testGraphjoin(VLinkGraph * graph){
    VLinkGraph *revertGraph = getRevertVLinkGraph(graph);
    TreeLRTree * tree = createTreeLRTree(NULL , NULL);
    CircleQueue *queue = createCircleQueue(NULL);
    int start = 0;
    insertDataLRTree(&start , tree);
    pushCircleQueue(&start , queue);
    while(!isEmptyCircleQueue(queue)){
        int i = *(int *)popCircleQueue(queue);
        int size = getSizeAList(graph->adj[i]);
        for(int j = 0 ; j < size ; j ++){
            NodeVlinkGraph * node = getElementByIndexAList(j , graph->adj[i]);
            if(!getDataFromTreeLRTree(&node->u , tree)){
                insertDataLRTree(&node->u , tree);
                pushCircleQueue(&node->u , queue);
            }
        }
        size = getSizeAList(revertGraph->adj[i]);
        for(int j = 0 ; j < size ; j ++){
            NodeVlinkGraph * node = getElementByIndexAList(j , revertGraph->adj[i]);
            if(!getDataFromTreeLRTree(&node->u , tree)){
                insertDataLRTree(&node->u , tree);
                pushCircleQueue(&node->u , queue);
            }
        }
    }
    if(getSizeLRTree(tree) != graph->v){
        printf("graph is not a connected graph\n");
        return 0;
    }
    destroyVlinkGraph(revertGraph);
    destroyCircleQueue(queue);
    setFreeDataFuncLRTree(freeInteger , tree);
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

int testGraphStrongConnected(VLinkGraph *graph){
    if(graph->v == 1){
        return 1;
    }
    VLinkGraph * revertGraph = getRevertVLinkGraph(graph);
    TreeLRTree *tree = createTreeLRTree(NULL , NULL);
    CircleQueue *queue = createCircleQueue(NULL);
    int start = 0;
    insertDataLRTree(&start , tree);
    pushCircleQueue(&start , queue);
    while(!isEmptyCircleQueue(queue)){
        int i = *(int *)popCircleQueue(queue);
        int size = getSizeAList(graph->adj[i]);
        for(int j = 0 ; j < size ; j ++){
            NodeVlinkGraph * node = getElementByIndexAList(j , graph->adj[i]);
            if(!getDataFromTreeLRTree(&node->u , tree)){
                insertDataLRTree(&node->u , tree);
                pushCircleQueue(&node->u , queue);
            }
        }
    }
    if(getSizeLRTree(tree) != graph->v){
        printf("some node is not connected to start node\n");
        return 0;
    }
    deleteElementLRTree(&start , tree);
    pushCircleQueue(&start , queue);
    while(!isEmptyCircleQueue(queue)){
        int i = *(int *)popCircleQueue(queue);
        int size = getSizeAList(revertGraph->adj[i]);
        for(int j = 0 ; j < size ; j ++){
            NodeVlinkGraph * node = getElementByIndexAList(j , revertGraph->adj[i]);
            if(getDataFromTreeLRTree(&node->u , tree)){
                deleteElementLRTree(&node->u , tree);
                pushCircleQueue(&node->u , queue);
            }
        }
    }
    if(getSizeLRTree(tree) != 0){
        printf("some node is not connected to end node\n");
        return 0;
    }
    return 1;
}

int testStronglyConnectedGraph(StronglyConnectedGraph * sGraph , VLinkGraph * graph){
    int size = sGraph->nodeNum;
    printf("stronglyConnectedGraph num is %d\n" , size);
    int v = 0;
    int e = sGraph->pathNum;
    for(int i = 0 ; i < size ; i ++){
        VLinkGraph * subgraph = getElementByIndexAList(i , sGraph->nodeList);
        //printf("subgraph %d %d\n" , graph->e , subgraph->e);
        //showVLinkGraph(graph);
        //showVLinkGraph(subgraph);
        v += subgraph->v;
        e += subgraph->e;
        if(!testGraphStrongConnected(subgraph)){
            printf("subgraph %d is not a strong connected graph\n" , i);
            return 0;
        }
        printf("test subgraph %d\n" , i);
        ArrayList * nodePathList = sGraph->adj[i];
        for(int j = 0 ; j < subgraph->v ; j ++){
            int flag = 0;
            ArrayList * newList = createArrayListAList(NULL);
            insertArrayListAList(subgraph->adj[j] , newList);
            printf("newList size is %d , %d”\n" , getSizeAList(newList) , getSizeAList(subgraph->adj[j]));
            for(int k = 0 ; k < getSizeAList(nodePathList) ; k ++){
                PathStronglyConnectedGraph * nodePath = getElementByIndexAList(k , nodePathList);
                //printf("nodePath  %d\n" , nodePath->v);
                if(nodePath->v == j){
                    NodeVlinkGraph * node = create_node_vlink_graph(nodePath->u , nodePath->w);
                    insertElementAList(node, newList);
                }
            }
            for(int k = 0 ; k < graph->v ; k ++){
                printf("finding is %d\n" , k);
                if(compareEDges(graph->adj[k] , newList)){
                    printf("find %d\n" , k);
                    flag = 1;
                    break;  
                }
            }
            if(!flag){
                printf("sGraph is not a strong connected Graph of graph\n");
                return 0;
            }
        }
    }
    if(graph->v != v || graph->e != e){
        printf("sGraph is not a strong connected Graph of graph , v is %d , %d , e is %d , %d\n", graph->v , v , graph->e , e);
        return 0;
    }
    printf("subgraph is a strong connected Graph of graph\n");
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
    /*printf("graph create success\n");
    ArrayList *subGraph = getSubGraphGraph(vGraph);
    printf("subFraphs create success\n");
    testSubGraph(vGraph , subGraph);
    int rsl = isCircleGraph(vGraph);
    printf("isCircle is %d\n" , rsl);*/
    //showVLinkGraph(vGraph);
    StronglyConnectedGraph * sGraph = getStronglyConnectedGraph(vGraph);
    printf("stronglyConnectedGraph create success\n");
    testStronglyConnectedGraph(sGraph , vGraph);
    printf("end\n");
}
