int *get_source_p_for_key(PointRelationGragh *pRelation){
    return &pRelation->sourceP;
}

int hash_source_p_for_key(int *sourceP){
    return *sourceP;
}

int compare_npde_vlink_Graph(NodeVlinkGraph * a, NodeVlinkGraph * b){
    return a->u == b->u;
}

int compare_integer_Graph(int * a, int * b){
    return *a == *b;
}

NodeVlinkGraph *create_node_vlink_graph(int vIndex , int w){
    NodeVlinkGraph * node = (NodeVlinkGraph *)mallocMemory(sizeof(NodeVlinkGraph));
    node->u = vIndex;
    node->w = w;
    return node;
}

void freeInteger(int * p){
    freeMemory(p);
    p = NULL;
}

int parsePointToCommon(int * p){
    if(p == NULL){
        return -1;
    }
    int rsl = *p;
    freeMemory(p);
    return rsl;
}

void destroy_node_vlink_graph(NodeVlinkGraph * node){
    freeMemory(node);
    node = NULL;
}

void destroyVlinkGraph(VLinkGraph * graph){
    if(graph == NULL){
        return;
    }
    if(graph->adj != NULL){
        for(int i = 0 ; i < graph->v ; i ++){
            if(graph->adj[i] != NULL){
                setFreeDataFuncAList(destroy_node_vlink_graph,graph->adj[i]);
                destroyAList(graph->adj[i]);
                graph->adj[i] = NULL;
            }
        }
        freeMemory(graph->adj);
        graph->adj = NULL;
    }
    freeMemory(graph);
    graph = NULL;   
}

VLinkGraph * createVLinkGraph(int v){
    VLinkGraph * graph = (VLinkGraph *)mallocMemory(sizeof(VLinkGraph));
    graph->v = v;
    graph->e = 0;
    graph->adj = (ArrayList **)mallocMemory(sizeof(ArrayList *) * v);
    for(int i = 0 ; i < v ; i++){
        graph->adj[i] = createArrayListAList(compare_npde_vlink_Graph);
    }
    return graph;
}

void add_point_VLinkGraph(VLinkGraph *graph){
    graph->v++;
    ArrayList ** temp = (ArrayList **)realloc(graph->adj , sizeof(ArrayList *)*graph->v);
    if(temp != NULL){
        graph->adj = temp;
    }else{
        printf("realloc error!\n");
    }
    graph->adj[graph->v - 1] = createArrayListAList(compare_npde_vlink_Graph);
}

void insert_edge_VLinkGraph(int v , int u , int w , VLinkGraph * graph){
    NodeVlinkGraph * node = create_node_vlink_graph(u , w);
    if(getElementAList(node , graph->adj[v]) == NULL){
        insertElementAList(node , graph->adj[v]);
        graph->e++;
    }
}

void insertEdgeVLinkDirectGraph(int v , int u , int w , VLinkGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return;
    }
    if(v < 0 || v >= graph->v || u < 0 || u >= graph->v){
        printf("index out of range , insertEdgeVLinkDirectGraph failed %d %d %d\n" , v , u , graph->v);
        return;
    }
    insert_edge_VLinkGraph(v , u , w , graph);
}

void insertEdgeVLinkUnDirectGraph(int v , int u , int w , VLinkGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return;
    }
    if(v < 0 || v >= graph->v || u < 0 || u >= graph->v){
        printf("index out of range , insertEdgeVLinkUnDirectGraph failed\n");
        return;
    }
    NodeVlinkGraph * node = create_node_vlink_graph(u , w);
    if(getElementAList(node , graph->adj[v]) == NULL){
        insertElementAList(node , graph->adj[v]);
        insertElementAList(create_node_vlink_graph(v , w) , graph->adj[u]);
        graph->e += 2;
    }else{
        ((NodeVlinkGraph *)getElementAList(node , graph->adj[v]))->w = w;
        node->u = v;
        ((NodeVlinkGraph *)getElementAList(node , graph->adj[u]))->w = w;
        destroy_node_vlink_graph(node);
    }
}

VMutrixGraph * createVMutrixGraph(int v){
    VMutrixGraph * graph = (VMutrixGraph *)mallocMemory(sizeof(VMutrixGraph));
    graph->v = v;
    graph->e = 0;
    graph->adj = (int **)mallocMemory(sizeof(int *) * v);
    for(int i = 0 ; i < v ; i++){
        graph->adj[i] = (int *)mallocMemory(sizeof(int) * v);
        for(int j = 0 ; j < v ; j++){
            if(i == j){
                graph->adj[i][j] = 0;
            }else{
                graph->adj[i][j] = PATH_NOT_EXSIT_GRAPH;
            }
        }
    }
    return graph;
}

void destroyVMutrixGraph(VMutrixGraph * graph){
    if(graph == NULL){
        return;
    }
    if(graph->adj != NULL){
        for(int i = 0 ; i < graph->v ; i ++){
            if(graph->adj[i] != NULL){
                freeMemory(graph->adj[i]);
                graph->adj[i] = NULL;
            }
            freeMemory(graph->adj);
        }
        graph->adj = NULL;
    }
    freeMemory(graph);
    graph = NULL;
}

void insertEdgeVMutrixDirectGraph(int v , int u , int w , VMutrixGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return;
    }
    if(v < 0 || v >= graph->v || u < 0 || u >= graph->v){
        printf("index out of range , insertEdgeVMutrixDirectGraph failed\n");
        return;
    }
    if(graph->adj[v][u] == PATH_NOT_EXSIT_GRAPH){  
        graph->e++;
    }
    graph->adj[v][u] = w;
}

void insertEdgeVMutrixUnDirectGraph(int v , int u , int w , VMutrixGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return;
    }
    if(v < 0 || v >= graph->v || u < 0 || u >= graph->v){
        printf("index out of range , insertEdgeVMutrixDirectGraph failed\n");
        return;
    }
    if(graph->adj[v][u] == PATH_NOT_EXSIT_GRAPH){  
        graph->e += 2;
    }
    graph->adj[v][u] = w;
    graph->adj[u][v] = w;
}

VLinkGraph *getRevertVLinkGraph(VLinkGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return NULL;
    }
    VLinkGraph * newGraph = createVLinkGraph(graph->v);
    for(int i = 0 ; i < graph->v ; i++){
        for(int j = 0 ; j < getSizeAList(graph->adj[i]) ; j++){
            NodeVlinkGraph * node = (NodeVlinkGraph *)getElementByIndexAList(j , graph->adj[i]);
            insert_edge_VLinkGraph(node->u , i , node->w , newGraph);
        }
    }
    return newGraph;
}

VMutrixGraph *changeLinktoMutrixGraph(VLinkGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return NULL;
    }
    VMutrixGraph * newGraph = createVMutrixGraph(graph->v);
    for(int i = 0 ; i < graph->v ; i++){
        for(int j = 0 ; j < getSizeAList(graph->adj[i]) ; j++){
            NodeVlinkGraph * node = (NodeVlinkGraph *)getElementByIndexAList(j , graph->adj[i]);
            newGraph->adj[i][node->u] = node->w;
        }
    }
    return newGraph;
}

VLinkGraph * changeMutrixtoLinkGraph(VMutrixGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return NULL;
    }
    VLinkGraph * newGraph = createVLinkGraph(graph->v);
    for(int i = 0 ; i < graph->v ; i++){
        for(int j = 0 ; j < graph->v ; j++){
            if(graph->adj[i][j] != PATH_NOT_EXSIT_GRAPH){
                insertEdgeVLinkUnDirectGraph(i , j , graph->adj[i][j] , newGraph);
            }
        }
    }
    return newGraph;
}

int validUnDirectVMutrixGraph(VMutrixGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return 0;
    }
    for(int i = 0 ; i < graph->v ; i++){
        for(int j = i ; j < graph->v ; j++){
            if(graph->adj[i][j] != graph->adj[j][i]){
                return 0;
            }
        }
    }
    return 1;
}

int validUnDirectVLinkGraph(VLinkGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
    }
    VMutrixGraph * newGraph = changeLinktoMutrixGraph(graph);
    int rsl = validUnDirectVMutrixGraph(newGraph);
    destroyVMutrixGraph(newGraph);
    return rsl;
}

NodeDFSTreeGraph *create_node_DFS_tree_Graph(int v){
    NodeDFSTreeGraph * node = (NodeDFSTreeGraph *)mallocMemory(sizeof(NodeDFSTreeGraph));
    node->v = v;
    node->height = node->leftIndex = node->rightIndex = node->visitIndex = 0;
    node->adjNodeList = createArrayListAList(NULL);
    node->child = node->parent = node->next = node->pre = NULL;
    return node;
}

void destroy_node_DFS_tree_Graph(NodeDFSTreeGraph * node){
    if(node == NULL){
        return;
    }
    if(node->adjNodeList != NULL){
        destroyAList(node->adjNodeList);
        node->adjNodeList = NULL;
    }
    node->child = node->parent = node->next = node->pre = NULL;
    freeMemory(node);
    node = NULL;
}

DFSTreeGraph *create_DFS_tree_Graph(int v){
    DFSTreeGraph * tree = (DFSTreeGraph *)mallocMemory(sizeof(DFSTreeGraph));
    tree->treeRoots = createArrayListAList(NULL);
    tree->nodeList = createArrayListAList(NULL);
    tree->toplogicList = createArrayListAList(NULL);
    for(int i = 0 ; i < v ; i ++){
        NodeDFSTreeGraph * node = create_node_DFS_tree_Graph(i);
        insertElementAList(node , tree->nodeList);
    }
    return tree;
}

NodeDFSTreeGraph *get_node_DFS_tree_Graph(int v , DFSTreeGraph * tree , VLinkGraph * graph){
    if(tree == NULL || graph == NULL){
        printf("tree is null or graph is null, failed\n");
        return NULL;
    }
    if(v < 0 || v >= getSizeAList(tree->nodeList)){
        printf("index out of range , get_node_DFS_tree_Graph failed v is %d\n" , v);
        return NULL;
    }
    NodeDFSTreeGraph * node = getElementByIndexAList(v , tree->nodeList);
    for(int i = 0 ; i < getSizeAList(graph->adj[v]) ; i ++){
        NodeVlinkGraph * nodeV = getElementByIndexAList(i , graph->adj[v]);
        insertElementAList(getElementByIndexAList(nodeV->u , tree->nodeList) , node->adjNodeList);
    }
    return node;
}

int get_next_visit_index_DFS_tree_Graph(NodeDFSTreeGraph * node , DFSTreeGraph *tree){
    if(node == NULL){
        printf("node is null or tree is null , failed\n");
        return 0;
    }
    int adjSize = getSizeAList(node->adjNodeList);
    if(node->visitIndex >= adjSize){
        return -1;
    }
    for(int i = node->visitIndex ; i < adjSize ; i ++){
        NodeDFSTreeGraph * searchNode = getElementByIndexAList(i , node->adjNodeList);
        if(searchNode->leftIndex == 0){
            deleteElementByIndexAList(i , node->adjNodeList);
            node->visitIndex = i;
            return searchNode->v;
        }else if(searchNode->rightIndex != 0 && searchNode->rightIndex > node->leftIndex){
            deleteElementByIndexAList(i , node->adjNodeList);
            i --;
            adjSize --;
        }else if(searchNode->rightIndex == 0){
            tree->isCicle = 1;
        }
    }
    node->visitIndex = getSizeAList(node->adjNodeList);
    return -1;
}

void set_node_parent_DFS_tree_Graph(NodeDFSTreeGraph * node , NodeDFSTreeGraph * parent){
    if(node == NULL || parent == NULL){
        printf("node or parent is null , failed\n");
        return;
    }
    node->parent = parent;
    if(parent->child == NULL){
        parent->child = node;
    }else{
        NodeDFSTreeGraph * searchNode = parent->child;
        while(searchNode->next != NULL){
            searchNode = searchNode->next;
        }
        searchNode->next = node;
        node->pre = searchNode;
    }
}

void delete_and_free_integer_from_tree_Graph(int * key , TreeLRTree * tree){
    int *data = deleteElementLRTree(key , tree);
    freeMemory(data);
}

DFSTreeGraph *getDFSTreeGraph(int startP ,VLinkGraph * graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return NULL;
    }
    DFSTreeGraph * tree = create_DFS_tree_Graph(graph->v);
    Stack *nodeStack = createStackStack();
    TreeLRTree * nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)mallocMemory(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    int treeIndex = 1;
    do{
        delete_and_free_integer_from_tree_Graph(&startP , nodeTree);
        NodeDFSTreeGraph * node = get_node_DFS_tree_Graph(startP , tree , graph);
        insertElementAList(node , tree->treeRoots);
        node->leftIndex = treeIndex ++;
        node->height = 1;
        do{
            int nextNodeIndex = get_next_visit_index_DFS_tree_Graph(node , tree);
            if(nextNodeIndex == -1){
                node->rightIndex = treeIndex ++;
                insertElementByIndexAList(0 , node , tree->toplogicList);
                node = popStack(nodeStack);
            }else{
                NodeDFSTreeGraph * nextNode = get_node_DFS_tree_Graph(nextNodeIndex , tree , graph);
                set_node_parent_DFS_tree_Graph(nextNode , node);
                delete_and_free_integer_from_tree_Graph(&nextNode->v , nodeTree);
                pushStack(node , nodeStack);
                node = nextNode;
                node->leftIndex = treeIndex ++;
                node->height = node->parent->height + 1;
            }
        }while(node != NULL);
        startP =  parsePointToCommon(getMinDataLRTree(nodeTree));
    }while(!isEmptyLRTree(nodeTree));
    destroyStack(nodeStack);
    destroyTreeLRTree(nodeTree);
    return tree;
}

void destroyDFSTreeGraph(DFSTreeGraph * tree){
    if(tree == NULL){
        return;
    }
    if(tree->nodeList != NULL){
        setFreeDataFuncAList(destroy_node_DFS_tree_Graph , tree->nodeList);
        destroyAList(tree->nodeList);
    }
    destroyAList(tree->treeRoots);
    freeMemory(tree);
    tree = NULL;
}

int validConnectedUndirectGraph(VLinkGraph *graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return 0;
    }
    if(!validUnDirectVLinkGraph(graph)){
        printf("graph is not undirect graph , failed\n");
        return 0;
    }
    DFSTreeGraph *tree = getDFSTreeGraph(0 , graph);
    return getSizeAList(tree->treeRoots) == 1;
}

PathGraph *createPathGraph(int v , int u  , int w){
    PathGraph * path = (PathGraph *)mallocMemory(sizeof(PathGraph));
    path->v = v;
    path->u = u;
    path->w = w;
    return path;
}

DistanceGraph *createDistanceGraph(int v , int distance){
    DistanceGraph * rsl = (DistanceGraph *)mallocMemory(sizeof(DistanceGraph));
    rsl->path = (PathGraph **)mallocMemory(sizeof(PathGraph *) * v);
    for(int i = 0 ;i < v ; i++){
        rsl->path[i] = NULL;
    }
    rsl->distance = distance;
    rsl->pathNum = v;
    return rsl;
}

void destroyPathGraph(PathGraph * path){
    if(path == NULL){
        return;
    }
    freeMemory(path);
    path = NULL;
}

void destroyDistanceGraph(DistanceGraph * distance){
    if(distance == NULL){
        return;
    }
    for(int i = 0 ; i < distance->pathNum ; i++){
        destroyPathGraph(distance->path[i]);
    }
    freeMemory(distance->path);
    distance->path = NULL;
    freeMemory(distance);
    distance = NULL;
}

PointRelationGragh * create_point_relation_Graph(int sourceP , ArrayList *targetPoiint){
    PointRelationGragh *rsl = (PointRelationGragh*)mallocMemory(sizeof(PointRelationGragh));
    rsl->sourceP = sourceP;
    if(targetPoiint == NULL){
        rsl->targetPointList = createArrayListAList(NULL);
    }else{
        rsl->targetPointList = targetPoiint;
    }
    rsl->targetP = getSizeAList(rsl->targetPointList);
    insertElementAList(rsl,rsl->targetPointList);
    return rsl;
}

void destory_point_relation_Graph(PointRelationGragh * pRelation){
    if(deleteElementAList(pRelation ,pRelation->targetPointList)!= NULL){
        for(int i = pRelation->targetP ; i < getSizeAList(pRelation->targetPointList) ; i++){
            PointRelationGragh * p = (PointRelationGragh *)getElementByIndexAList(i , pRelation->targetPointList);
            p->targetP--;
        }
    }
    if(getSizeAList(pRelation->targetPointList) == 0){
        destroyAList(pRelation->targetPointList);
    }
    freeMemory(pRelation);
}

PointRelationGragh *gen_point_relation_map_Graph(int sourceP , ArrayList * targetPointList , HashTable * table){
    //printf("gen_point_relation_map_Graph %d\n",sourceP);
    PointRelationGragh * pRelation = create_point_relation_Graph(sourceP , targetPointList);
    insertElementHTable(pRelation , table);
    return pRelation;
}

PointRelationGragh *get_target_point_relation_Graph(int sourceP , HashTable * table){
    PointRelationGragh * pRelation =  getElementsHTable(&sourceP , table);
    return pRelation;
}

void merge_target_pointList_Graph(ArrayList * graph1 , ArrayList * graph2){
    if(graph1 == NULL || graph2 == NULL){
        printf("graph is null , failed\n");
        return NULL;
    }
    int siez1 = getSizeAList(graph1);
    int size2 = getSizeAList(graph2);
    for(int i = 0 ; i < size2 ; i ++){
        PointRelationGragh * pRelation = (PointRelationGragh *)getElementByIndexAList(i , graph2);
        pRelation->targetP += siez1;
        pRelation->targetPointList = graph1;
        insertElementAList(pRelation , graph1);
    }
    destroyAList(graph2);
}

PointSubGragh *create_point_sub_graph_Graph(int v){
    PointSubGragh * subGraph = (PointSubGragh *)mallocMemory(sizeof(PointSubGragh));
    subGraph->map = createHashTable(v , get_source_p_for_key , NULL , hash_source_p_for_key);
    setHashFactorHTable(1 , subGraph->map);
    setBlockSizeHTable(v , subGraph->map);
    subGraph->goupPoints = createArrayListAList(NULL);
    return subGraph;
}

void destory_point_sub_graph_Graph(PointSubGragh * subGraph){
    if(subGraph == NULL){
        return;
    }
    setFreeDataHTable(destory_point_relation_Graph , subGraph->map);
    destroyHTable(subGraph->map);
    destroyAList(subGraph->goupPoints);
    freeMemory(subGraph);
    subGraph = NULL;
}


PointSubGragh *group_week_connected_points_Graph(VLinkGraph * graph){
    PointSubGragh * pointSubGraph = create_point_sub_graph_Graph(graph->v);
    TreeLRTree *nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)mallocMemory(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    CircleQueue *queue = createCircleQueue(NULL);
    while(!isEmptyLRTree(nodeTree)){
        int *souceP = (int *)popMinDataLRTree(nodeTree);
        PointRelationGragh * point = gen_point_relation_map_Graph(*souceP , NULL , pointSubGraph->map);
        insertElementAList(point->targetPointList , pointSubGraph->goupPoints);
        freeInteger(souceP);
        pushCircleQueue(point , queue);
        while(!isEmptyCircleQueue(queue)){
            point = popCircleQueue(queue);
            int relationNodeSize = getSizeAList(graph->adj[point->sourceP]);
            for(int i = 0 ; i < relationNodeSize ; i ++){
                NodeVlinkGraph * vlinkNode = getElementByIndexAList(i , graph->adj[point->sourceP]);
                PointRelationGragh *pointU = get_target_point_relation_Graph(vlinkNode->u , pointSubGraph->map);
                if(pointU == NULL){
                    pointU = gen_point_relation_map_Graph(vlinkNode->u , point->targetPointList ,pointSubGraph->map);
                    pushCircleQueue(pointU , queue);
                    delete_and_free_integer_from_tree_Graph(&vlinkNode->u , nodeTree);
                }else{
                    if(pointU->targetPointList != point->targetPointList){
                        deleteElementAList(pointU->targetPointList , pointSubGraph->goupPoints);
                        merge_target_pointList_Graph(point->targetPointList , pointU->targetPointList);
                    }
                }
            }
        }
    }
    destroyCircleQueue(queue);
    destroyTreeLRTree(nodeTree);
    return pointSubGraph;
}

ArrayList *getSubGraphGraph(VLinkGraph * graph){
    ArrayList *subGraphs = createArrayListAList(NULL);
    PointSubGragh *pointSubGraph = group_week_connected_points_Graph(graph);
    int graphSize = getSizeAList(pointSubGraph->goupPoints);
    //printf("subGraph size is %d\n" , graphSize);
    for(int i = 0 ; i < graphSize ; i ++){
        ArrayList *vList = getElementByIndexAList(i , pointSubGraph->goupPoints);
        int v = getSizeAList(vList);
        //printf("graph %d size is %d\n",i , v);
        VLinkGraph *subGraph = createVLinkGraph(v);
        for(int j = 0 ; j < v ; j ++){
            PointRelationGragh *point = getElementByIndexAList(j , vList);
            int adjSize = getSizeAList(graph->adj[point->sourceP]);
            for(int k = 0 ; k < adjSize; k ++){
                NodeVlinkGraph *vlinkNode = getElementByIndexAList(k , graph->adj[point->sourceP]);
                PointRelationGragh *pointU = get_target_point_relation_Graph(vlinkNode->u , pointSubGraph->map);
                insertEdgeVLinkDirectGraph(point->targetP , pointU->targetP , vlinkNode->w , subGraph);
            }
        }
        //showVLinkGraph(subGraph);
        insertElementAList(subGraph , subGraphs);
    }
    destory_point_sub_graph_Graph(pointSubGraph);
    return subGraphs;
}

int validConnectedDirectGraph(VLinkGraph *graph){
    if(graph == NULL){
        printf("graph is null , failed\n");
        return 0;
    }
    PointSubGragh *pointSubGraph = group_week_connected_points_Graph(graph);
    int graphSize = getSizeAList(pointSubGraph->goupPoints);
    //printf("pointSubGraph size is %d\n" , getSizeAList(pointSubGraph->goupPoints));
    if(graphSize > 1){
        destory_point_sub_graph_Graph(pointSubGraph);
        return -1;
    }
    ArrayList * list = getFirstElementAList(pointSubGraph->goupPoints);
    PointRelationGragh * point = getFirstElementAList(list);
    DFSTreeGraph *tree = getDFSTreeGraph(point->sourceP ,graph);
    if(getSizeAList(tree->treeRoots) != 1){
        destory_point_sub_graph_Graph(pointSubGraph);
        return -1;
    }
    int rslPoint = point->sourceP;
    destory_point_sub_graph_Graph(pointSubGraph);
    return rslPoint;
}

void showVLinkGraph(VLinkGraph *graph){
    if(graph == NULL){
        printf("graph is null , show nothing\n");
        return ;
    }
    printf("show graph\n");
    for(int i = 0 ; i < graph->v ; i ++){
        int size = getSizeAList(graph->adj[i]);
        for(int j = 0 ; j < size ; j ++){
            NodeVlinkGraph *node = getElementByIndexAList(j , graph->adj[i]);
            printf("v= %d d= %d w= %d\n" , i , node->u , node->w);
        }
    }
}

int compare_path_Graph(PathGraph *data1 , PathGraph * data2){
    return data1->u - data2->u;
}

/*int valid_circle_dfs_node_Graph(NodeDFSTreeGraph* node){
    if(node == NULL){
        printf("node is null or tree is null , failed\n");
        return 0;
    }
    int adjSize = getSizeAList(node->adjNodeList);
    for(int i = 0 ; i < adjSize ; i ++){
        NodeDFSTreeGraph * preNode = getElementByIndexAList(i , node->adjNodeList);
        if(preNode->rightIndex < node->rightIndex){
            return 1;
        }
    }
    return 0;
}*/

int isCircleGraph(VLinkGraph *graph){
    DFSTreeGraph *dfsTree = getDFSTreeGraph(0,graph);
    int validCircle = dfsTree->isCicle;
    destroyDFSTreeGraph(dfsTree);
    return validCircle;
}

PathStronglyConnectedGraph *create_path_strong_connected_Graph(int v , int u , int w , int nodeIndex){
    PathStronglyConnectedGraph *path = (PathStronglyConnectedGraph *)mallocMemory(sizeof(PathStronglyConnectedGraph));
    path->v = v;
    path->u = u;
    path->w = w;
    path->nodeIndex = nodeIndex;
    return path;
}

StronglyConnectedGraph * create_strongly_connected_Graph(int nodeNum){
    StronglyConnectedGraph *stronglyConnectedGraph = (StronglyConnectedGraph *)mallocMemory(sizeof(StronglyConnectedGraph));
    stronglyConnectedGraph->nodeNum = nodeNum;
    stronglyConnectedGraph->pathNum = 0;
    stronglyConnectedGraph->nodeList = createArrayListAList(NULL);
    stronglyConnectedGraph->adj = (ArrayList **)mallocMemory(sizeof(ArrayList *) * nodeNum);
    for(int i = 0 ; i < stronglyConnectedGraph->nodeNum ; i ++){
        insertElementAList(NULL ,stronglyConnectedGraph->nodeList);    
        stronglyConnectedGraph->adj[i] = createArrayListAList(NULL);
    }
    return stronglyConnectedGraph;
}

void destroyStronglyConnectedGraph(StronglyConnectedGraph *stronglyConnectedGraph){
    if(stronglyConnectedGraph == NULL){
        return ;
    }
    setFreeDataFuncAList(destroyVlinkGraph , stronglyConnectedGraph->nodeList);
    destroyAList(stronglyConnectedGraph->nodeList);
    if(stronglyConnectedGraph->adj != NULL){
        for(int i = 0 ; i < stronglyConnectedGraph->nodeNum ; i ++){
            setFreeDataFuncAList(destroyPathGraph , stronglyConnectedGraph->adj[i]);
            destroyAList(stronglyConnectedGraph->adj[i]);
        }
        freeMemory(stronglyConnectedGraph->adj);
        stronglyConnectedGraph->adj = NULL;
    }
    freeMemory(stronglyConnectedGraph);
    stronglyConnectedGraph = NULL;
}

void set_node_to_strongly_connected_Graph(VLinkGraph *node , int nodeIndex ,StronglyConnectedGraph *stronglyConnectedGraph){
    void * data = replaceElementByIndexAList(nodeIndex , node , stronglyConnectedGraph->nodeList);
    if(data != NULL){
        destroyVlinkGraph(data);
    }
}

void add_path_to_strongly_connected_Graph(PathStronglyConnectedGraph *path , int nodeIndex ,StronglyConnectedGraph *stronglyConnectedGraph){
    insertElementAList(path , stronglyConnectedGraph->adj[nodeIndex]);
    stronglyConnectedGraph->pathNum ++;
}

StronglyConnectedGraph *getStronglyConnectedGraph(VLinkGraph *graph){
    Stack *nodeStack = createStackStack();
    TreeLRTree * nodeTree = createTreeLRTree(NULL , NULL);
    HashTable *map = createHashTable(graph->v , get_source_p_for_key , NULL , hash_source_p_for_key);
    setHashFactorHTable(1 , map);
    setBlockSizeHTable(graph->v , map);
    ArrayList *strongConnectList = createArrayListAList(NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)mallocMemory(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    while(!isEmptyLRTree(nodeTree)){
        int startP =  parsePointToCommon(popMinDataLRTree(nodeTree));
        PointRelationGragh *sourcePoint =  gen_point_relation_map_Graph(startP , NULL , map);
        insertElementAList(sourcePoint->targetPointList , strongConnectList);
        PathGraph * path = createPathGraph(-1 ,startP ,-1);
        do{
            int relationNodeSize = getSizeAList(graph->adj[path->u]);
            if(path->w < relationNodeSize - 1){
                path->w ++;
                pushStack(path , nodeStack);
                path = createPathGraph(path->u ,((NodeVlinkGraph *)getElementByIndexAList(path->w , graph->adj[path->u]))->u ,-1);
                PointRelationGragh * targetPoint = get_target_point_relation_Graph(path->u , map);
                if(targetPoint == NULL){
                    targetPoint = gen_point_relation_map_Graph(path->u , NULL , map);
                    insertElementAList(targetPoint->targetPointList , strongConnectList);
                    int *data = deleteElementLRTree(&path->u , nodeTree);
                    if(data != NULL){
                        freeInteger(data);
                    }
                }else{
                    if(validDataExistStack(path , compare_path_Graph ,nodeStack)){
                        ArrayList * CircleDatas = getAfterDataStack(path , compare_path_Graph ,nodeStack);
                        int circleSize = getSizeAList(CircleDatas);
                        for(int i = 0 ; i < circleSize - 1 ; i ++){
                            PathGraph * circlePath = (PathGraph *)getElementByIndexAList(i , CircleDatas);
                            PointRelationGragh * circlePoint = get_target_point_relation_Graph(circlePath->u , map);
                            if(targetPoint->targetPointList != circlePoint->targetPointList){
                                deleteElementAList(circlePoint->targetPointList , strongConnectList);
                                merge_target_pointList_Graph(targetPoint->targetPointList , circlePoint->targetPointList);
                            }
                        }
                    }
                    destroyPathGraph(path);
                    path = popStack(nodeStack);
                }
            }else{
                destroyPathGraph(path);
                path = popStack(nodeStack);
            }
        }while(path != NULL);
    }
    int strongConnectSize = getSizeAList(strongConnectList);
    StronglyConnectedGraph *rsl = create_strongly_connected_Graph(strongConnectSize);
    for(int i = 0 ; i < strongConnectSize ; i ++){
        ArrayList *strongConnect = (ArrayList *)getElementByIndexAList(i , strongConnectList);
        int strongConnectNodeSize = getSizeAList(strongConnect);
        VLinkGraph *node = createVLinkGraph(strongConnectNodeSize);
        set_node_to_strongly_connected_Graph(node , i , rsl);
        for(int j = 0 ; j < strongConnectNodeSize ; j ++){
            PointRelationGragh * curPoint = (PointRelationGragh *)getElementByIndexAList(j , strongConnect);
            int nodeSize = getSizeAList(graph->adj[curPoint->sourceP]);
            for(int k =  0 ; k < nodeSize ; k ++){
                NodeVlinkGraph *vLinkNode = (NodeVlinkGraph *)getElementByIndexAList(k , graph->adj[curPoint->sourceP]);
                PointRelationGragh * targetPoint = get_target_point_relation_Graph(vLinkNode->u , map);
                if(targetPoint->targetPointList == curPoint->targetPointList){
                    insertEdgeVLinkDirectGraph(curPoint->targetP , targetPoint->targetP , vLinkNode->w , node);
                }else{
                    int nodeIndex = getDataIndexAList(targetPoint->targetPointList , strongConnectList);
                    PathStronglyConnectedGraph * path = create_path_strong_connected_Graph(curPoint->targetP , targetPoint->targetP , vLinkNode->w , nodeIndex);
                    add_path_to_strongly_connected_Graph(path , i , rsl);
                }
            }
        }
    }
    setFreeDataHTable(destory_point_relation_Graph , map);
    destroyHTable(map);
    destroyStack(nodeStack);
    destroyTreeLRTree(nodeTree);
    destroyAList(strongConnectList);
    return rsl;
}

int topLogic_compare_for_heap_Graph(TopLogicNodegraph *node1 , TopLogicNodegraph *node2){
    if(node1->calNumber == 0 && node2->calNumber == 0){
        return node2->minTime - node1->minTime;
    }
    return  node2->calNumber - node1->calNumber;
}

TopLogicNodegraph *create_top_logic_node_Graph(int v , int preNodeNumber){
    TopLogicNodegraph *node = (TopLogicNodegraph *)mallocMemory(sizeof(TopLogicNodegraph));
    node->v = v;
    node->preNode = createArrayListAList(NULL);
    node->calNumber = preNodeNumber;
    node->minTime = 0;
    return node;
}

TopLogicNodegraph * get_top_logic_node_from_heap_node(ArrayList * heapNodeList , int index){
    NodeHeapInfo * node = getElementByIndexAList(index , heapNodeList);
    return (TopLogicNodegraph *)node->data;
}

ArrayList *getTopLogicalSortGraph(VLinkGraph *graph){
    if(graph == NULL){
        printf("graph is null , can not toplogical sort\n");
        return NULL;
    }
    if(isCircleGraph(graph)){
        printf("circle graph can not toplogical sort\n ");
        return NULL;
    }
    ArrayList *rsl = createArrayListAList(NULL);
    VLinkGraph *invertGraph = getRevertVLinkGraph(graph);
    HeapInfo *heap = createHeapInfoHeap(graph->v , topLogic_compare_for_heap_Graph);
    ArrayList *heapNodeList = createArrayListAList(NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        TopLogicNodegraph *node = create_top_logic_node_Graph(i , getSizeAList(invertGraph->adj[i]));
        NodeHeapInfo * heapNode = insertElementHeap(node , heap);
        insertElementAList(heapNode , heapNodeList);
    }
    destroyVlinkGraph(invertGraph);
    while(!isEmptyHeap(heap)){
        TopLogicNodegraph * node = popElementHeap(heap);
        insertElementAList(node , rsl);
        int afterNodeSize = getSizeAList(graph->adj[node->v]);
        for(int i = 0 ; i < afterNodeSize ; i ++){
            int afterNodeIndex = ((NodeVlinkGraph *)getElementByIndexAList(i , graph->adj[node->v]))->u;
            int afetNodeWeight= ((NodeVlinkGraph *)getElementByIndexAList(i , graph->adj[node->v]))->w;
            TopLogicNodegraph *afterNode = get_top_logic_node_from_heap_node(heapNodeList , afterNodeIndex);
            afterNode->calNumber --;
            insertElementAList(node , afterNode->preNode);
            if(afterNode->minTime < node->minTime + afetNodeWeight){
                afterNode->minTime = node->minTime + afetNodeWeight;
            }
            decreseDataHeap(getElementByIndexAList(afterNodeIndex , heapNodeList) , heap);
        }
    }
    destroyHeapInfoHeap(heap);
    destroyAList(heapNodeList);
    return rsl;
}

