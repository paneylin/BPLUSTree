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

int testNodeDFSTree(NodeDFSTreeGraph *node, NodeDFSTreeGraph *pNode , VLinkGraph *graph){
    if(node->leftIndex >= node->rightIndex){
        printf("node index error!\n");
        return 0;
    }
    if(node->parent != pNode || (pNode != NULL  && (node->leftIndex <= pNode->leftIndex || node->rightIndex >= pNode->rightIndex))){
        printf("node is not a child of pNode\n");
        return 0;
    }
    TreeLRTree * tree = createTreeLRTree(NULL , NULL);
    int relationNodeSize = getSizeAList(graph->adj[node->v]);
    for(int i = 0 ; i < relationNodeSize ; i ++){
        NodeVlinkGraph *vLinkNode = getElementByIndexAList(i , graph->adj[node->v]);
        insertDataLRTree(&vLinkNode->u , tree);
    }
    int size = getSizeAList(node->adjNodeList);
    for(int i = 0 ; i < size ; i ++){
        NodeDFSTreeGraph *preNode = getElementByIndexAList(i , node->adjNodeList);
        if(preNode->leftIndex >= node->leftIndex ){
            printf("preNode is not a preNode of node , node is %d , preNode index is %d , node->left %d , node->right %d , preNode->left %d , preNode->right %d\n" 
            , node->v ,preNode->v , node->leftIndex , node->rightIndex , preNode->leftIndex , preNode->rightIndex);
            return 0;
        }
        if(!getDataFromTreeLRTree(&preNode->v , tree)){
            printf("node->v is not related to tree , preNode index is %d\n" , preNode->v);
            return 0;
        }else{
            deleteElementLRTree(&preNode->v , tree);
        }
    }
    //printf("test node %d\n");
    NodeDFSTreeGraph * child = node->child;
    if(child == NULL && isEmptyLRTree(tree)){
        destroyTreeLRTree(tree);
        return 1;
    }
    if(isEmptyLRTree(tree)){
        printf("DFSTree child is not in graph\n");
        destroyTreeLRTree(tree);
        return 0;
    }
    int startIndex = node->leftIndex;
    do{
        if(!testNodeDFSTree(child , node , graph)){
            destroyTreeLRTree(tree);
            return 0;
        }
        if(!getDataFromTreeLRTree(&child->v , tree)){
            printf("DFSTree child is not in graph\n");
            destroyTreeLRTree(tree);
            return 0;
        }else{
            deleteElementLRTree(&child->v , tree);
        }
        if(child->leftIndex <= startIndex){
            printf("brother index not corrected\n");
            destroyTreeLRTree(tree);
            return 0;
        }
        startIndex = child->rightIndex;
        child = child->next;
    }while(child != NULL);
    if(!isEmptyLRTree(tree)){
        while(!isEmptyLRTree(tree)){
            int *v = popMinDataLRTree(tree);
            freeInteger(v);
        }
    }
    destroyTreeLRTree(tree);
    return 1;
}

int testDFSTree(DFSTreeGraph *tree , VLinkGraph *graph){
    int nodeSize = getSizeAList(tree->nodeList);
    if(nodeSize != graph->v){
        printf("nodeSize is not equal to graph->v, nodeSize is %d , graph->v is %d" , nodeSize , graph->v);
        return 0;
    }
    int fSize = 0;
    for(int i = 0 ; i < nodeSize ; i ++){
        NodeDFSTreeGraph *node = getElementByIndexAList(i , tree->nodeList);
        if(node->v != i){
            printf("node->u is not equal to i, node->u is %d , i is %d" , node->v , i);
            return 0;
        }
        if(node->leftIndex == 0 || node->rightIndex == 0 || node->rightIndex <= node->leftIndex){
            printf("node->leftIndex is not equal to i, node->leftIndex is %d , node->rightIndex is %d" , node->leftIndex , node->rightIndex);
            return 0;
        }
        if(node->parent == NULL){
            fSize ++;
        }
    }
    int forestSize = getSizeAList(tree->treeRoots);
    printf("DFSTree forestSize is %d , total node is %d\n" , forestSize , graph->v);
    for(int i = 0 ; i < forestSize ; i ++){
        NodeDFSTreeGraph * node = getElementByIndexAList(i , tree->treeRoots);
        if(!testNodeDFSTree(node , NULL , graph)){
            return 0;
        }
    }
    return 1;
}

int testTopLogicSort(VLinkGraph * graph){
    ArrayList *topLogicSort = getTopLogicalSortGraph(graph);
    if(topLogicSort == NULL){
        printf("topLogicSort is NULL\n");
        return 0;
    }
    VLinkGraph *reverseGraph = getRevertVLinkGraph(graph);
    TreeLRTree * tree = createTreeLRTree(NULL , NULL);
    for( int i  = 0 ; i < getSizeAList(topLogicSort) ; i ++){
        TopLogicNodegraph * node = getElementByIndexAList(i , topLogicSort);
        if(node->calNumber != 9){
            printf("node->calNumber is not 9 , node->v is %d , node->calNumber is %d\n" , node->v , node->calNumber);
        }
        insertDataLRTree(&node->v , tree);
        int size = getSizeAList(reverseGraph->adj[node->v]);
        int preNodeIsze = getSizeAList(node->preNode);
        if(size != preNodeIsze){
            printf("node->v is %d , size is %d , preNodeIsze is %d\n" , node->v , size , preNodeIsze);
        }
        for(int j = 0 ; j < preNodeIsze ; j ++){
            TopLogicNodegraph *preNode = getElementByIndexAList(j , node->preNode);
            if(!getDataFromTreeLRTree(&preNode->v , tree)){
                printf("node->v is %d , pre->v is %d preNode is not preIndex\n" , node->v , preNode->v);
                return 0;
            }
            int flag = 0;
            for(int k = 0 ; k < size ; k ++){
                NodeVlinkGraph *edge = getElementByIndexAList(k , reverseGraph->adj[node->v]);
                if(edge->u == preNode->v){
                    if(preNode->minTime + edge->w > node->minTime){
                        printf("minTime error , minTime is %d , pre minTime is %d , edge is %d\n" , node->minTime , preNode->minTime , edge->w);
                        return 0;
                    }else if(preNode->minTime + edge->w == node->minTime){
                        flag = 1;
                    }
                    break;
                }
                if(k == size - 1){
                    printf("node->v is %d , pre->v is %d preNode is not preIndex\n" , node->v , preNode->v);
                    return 0;
                }
            }
            if(flag == 0){
                printf("minTime uncorrect ,no equal minTime %d\n" , node->v);
                return 0;
            }
        }
    }
    printf("topLogicSort test success\n");
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
    testTopLogicSort(vGraph);
    
    /*printf("graph create success\n");
    int connectPoint = validConnectedDirectGraph(vGraph);
    printf("connected Point is %d\n" , connectPoint);*/

    /*ArrayList *subGraph = getSubGraphGraph(vGraph);
    printf("subFraphs create success\n");
    testSubGraph(vGraph , subGraph);*/

    /*int rsl = isCircleGraph(vGraph);
    printf("isCircle is %d\n" , rsl);*/
    //showVLinkGraph(vGraph);

    /*DFSTreeGraph *tree = getDFSTreeGraph(0,vGraph);
    printf("DFSTree create success\n");
    if(!testDFSTree(tree , vGraph)){
        exit(0);
    }
    printf("DFSTree test success\n");*/

    /*StronglyConnectedGraph * sGraph = getStronglyConnectedGraph(vGraph);
    printf("stronglyConnectedGraph create success\n");
    testStronglyConnectedGraph(sGraph , vGraph);*/
    printf("end\n");
}
