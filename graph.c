int *get_source_p_for_key(PointRelation *pRelation){
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

PointRelation * create_point_relation_Graph(int sourceP , ArrayList *targetPoiint){
    PointRelation *rsl = (PointRelation*)mallocMemory(sizeof(PointRelation));
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

void destory_point_relation_Graph(PointRelation * pRelation){
    if(deleteElementAList(pRelation ,pRelation->targetPointList)!= NULL){
        for(int i = pRelation->targetP ; i < getSizeAList(pRelation->targetPointList) ; i++){
            PointRelation * p = (PointRelation *)getElementByIndexAList(i , pRelation->targetPointList);
            p->targetP--;
        }
    }
    if(getSizeAList(pRelation->targetPointList) == 0){
        destroyAList(pRelation->targetPointList);
    }
    freeMemory(pRelation);
}

PointRelation *gen_point_relation_map(int sourceP , ArrayList * targetPointList , HashTable * table){
    PointRelation * pRelation = create_point_relation_Graph(sourceP , targetPointList);
    insertElementHTable(pRelation , table);
    return pRelation;
}

PointRelation *get_target_point_relation_Graph(int sourceP , HashTable * table){
    PointRelation * pRelation =  getElementsHTable(&sourceP , table);
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
        PointRelation * pRelation = (PointRelation *)getElementByIndexAList(i , graph2);
        pRelation->sourceP += siez1;
        pRelation->targetPointList = graph1;
        insertElementAList(pRelation , graph1);
    }
    destroyAList(graph2);
}

ArrayList *getSubGraphGraph(VLinkGraph * graph){
    ArrayList *subGraphs = createArrayListAList(NULL);
    HashTable *map = createHashTable(graph->v , get_source_p_for_key , NULL , hash_source_p_for_key);
    setHashFactorHTable(1 , map);
    setBlockSizeHTable(graph->v , map);
    TreeLRTree *nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)mallocMemory(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    CircleQueue *queue = createCircleQueue(NULL);
    ArrayList *pointsList = createArrayListAList(NULL);
    while(!isEmptyLRTree(nodeTree)){
        int newTargetP = 0;
        int *souceP = (int *)popMinDataLRTree(nodeTree);
        PointRelation * point = gen_point_relation_map(*souceP , NULL , map);
        insertElementAList(point->targetPointList , pointsList);
        freeInteger(souceP);
        pushCircleQueue(point , queue);
        while(!isEmptyCircleQueue(queue)){
            point = popCircleQueue(queue);
            //add_point_VLinkGraph(subGraph);
            int relationNodeSize = getSizeAList(graph->adj[point->sourceP]);
            for(int i = 0 ; i < relationNodeSize ; i ++){
                NodeVlinkGraph * vlinkNode = getElementByIndexAList(i , graph->adj[point->sourceP]);
                PointRelation *pointU = get_target_point_relation_Graph(vlinkNode->u , map);
                if(pointU == NULL){
                    pointU = gen_point_relation_map(vlinkNode->u , point->targetPointList ,map);
                    pushCircleQueue(pointU , queue);
                    int * freeData = (int *)deleteElementLRTree(&vlinkNode->u , nodeTree);
                    freeMemory(freeData);
                }else{
                    if(pointU->targetPointList != point->targetPointList){
                        deleteElementAList(pointU->targetPointList , pointsList);
                        merge_target_pointList_Graph(point->targetPointList , pointU->targetPointList);
                    }
                }
            }
        }
    }
    int graphSize = getSizeAList(pointsList);
    for(int i = 0 ; i < graphSize ; i ++){
        int v = getSizeAList(getElementByIndexAList(i , pointsList));
        VLinkGraph *subGraph = createVLinkGraph(v);
        for(int j = 0 ; j < v ; j ++){
            PointRelation *point = getElementByIndexAList(j , getElementByIndexAList(i , pointsList));
            for(int k = 0 ; k < getSizeAList(graph->adj[point->sourceP]) ; k ++){
                NodeVlinkGraph *vlinkNode = getElementByIndexAList(k , graph->adj[point->sourceP]);
                PointRelation *pointU = get_target_point_relation_Graph(vlinkNode->u , map);
                insertEdgeVLinkDirectGraph(point->targetP , pointU->targetP , vlinkNode->w , subGraph);
            }
        }
        insertElementAList(subGraph , subGraphs);
    }
    setFreeDataHTable(destory_point_relation_Graph , map);
    destroyHTable(map);
    destroyCircleQueue(queue);
    destroyTreeLRTree(nodeTree);
    destroyAList(pointsList);
    return subGraphs;
}

ArrayList *getSubGraphsUnDirectGraph(VLinkGraph *graph){
    ArrayList *subGraphs = createArrayListAList(NULL);
    HashTable *map = createHashTable(graph->v , get_source_p_for_key , NULL , hash_source_p_for_key);
    setHashFactorHTable(1 , map);
    setBlockSizeHTable(graph->v , map);
    TreeLRTree *nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)mallocMemory(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    CircleQueue *queue = createCircleQueue(NULL);
    int graphNo = 0;
    while(!isEmptyLRTree(nodeTree)){
        VLinkGraph * subGraph = createVLinkGraph(0);
        insertElementAList(subGraph , subGraphs);
        int newTargetP = 0;
        int *souceP = (int *)popMinDataLRTree(nodeTree);
        PointRelation * point = gen_point_relation_map(*souceP , newTargetP++ , graphNo , map);
        freeInteger(souceP);
        pushCircleQueue(point , queue);
        while(!isEmptyCircleQueue(queue)){
            point = popCircleQueue(queue);
            add_point_VLinkGraph(subGraph);
            int relationNodeSize = getSizeAList(graph->adj[point->sourceP]);
            for(int i = 0 ; i < relationNodeSize ; i ++){
                NodeVlinkGraph * vlinkNode = getElementByIndexAList(i , graph->adj[point->sourceP]);
                PointRelation *pointU = get_target_point_relation_Graph(vlinkNode->u , map);
                if(pointU == NULL){
                    pointU = gen_point_relation_map(vlinkNode->u , newTargetP ++, graphNo ,map);
                    pushCircleQueue(pointU , queue);
                    int * freeData = (int *)deleteElementLRTree(&vlinkNode->u , nodeTree);
                    freeMemory(freeData);
                }
                insert_edge_VLinkGraph(point->targetP , pointU->targetP , vlinkNode->w , subGraph);
            }
        }
    }
    setFreeDataHTable(destory_point_relation_Graph , map);
    destroyHTable(map);
    destroyCircleQueue(queue);
    destroyTreeLRTree(nodeTree);
    return subGraphs;
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

int isCircleUndirectGraph(VLinkGraph * graph){
    TreeLRTree *nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)mallocMemory(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    CircleQueue *queue = createCircleQueue(NULL);
    while(!isEmptyLRTree(nodeTree)){
        int *startP = (int *)popMinDataLRTree(nodeTree);
        PathGraph * path = createPathGraph(-1 ,*startP ,UNREACHABLE_GRAPH);
        freeMemory(startP);
        pushCircleQueue(path , queue);
        while(!isEmptyCircleQueue(queue)){
            path = popCircleQueue(queue);
            int relationNodeSize = getSizeAList(graph->adj[path->u]);
            for(int i = 0 ; i < relationNodeSize ; i ++){
                NodeVlinkGraph * vlinkNode = getElementByIndexAList(i , graph->adj[path->u]);
                if(getDataFromTreeLRTree(&vlinkNode->u , nodeTree)){
                    PathGraph *newPath = createPathGraph(path->u ,vlinkNode->u ,vlinkNode->w);
                    pushCircleQueue(newPath , queue);
                    int * freeData = (int *)deleteElementLRTree(&vlinkNode->u , nodeTree);
                    freeMemory(freeData);
                }else if(vlinkNode->u != path->v){
                    setFreeDataFuncCircleQueue(destroyPathGraph , queue);
                    destroyCircleQueue(queue);
                    setFreeDataFuncLRTree(freeInteger , nodeTree);
                    destroyTreeLRTree(nodeTree);
                    return 1;
                }
            }
            freeMemory(path);
        }
    }
    return 0;
}

int compare_path_Graph(PathGraph *data1 , PathGraph * data2){
    return data1->u - data2->u;
}

int isCircleGraph(VLinkGraph *graph){
    Stack *nodeStack = createStackStack();
    TreeLRTree * nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)mallocMemory(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    while(!isEmptyLRTree(nodeTree)){
        int startP =  parsePointToCommon(popMinDataLRTree(nodeTree));
        PathGraph * path = createPathGraph(-1 ,startP ,-1);
        do{
            int relationNodeSize = getSizeAList(graph->adj[path->u]);
            if(path->w < relationNodeSize - 1){
                path->w ++;
                pushStack(path , nodeStack);
                path = createPathGraph(path->u ,((NodeVlinkGraph *)getElementByIndexAList(path->w , graph->adj[path->u]))->u ,-1);
                //printf("path->u = %d path->v = %d" , path->u , path->v);
                if(validDataExistStack(path , compare_path_Graph ,nodeStack)){
                    destroyPathGraph(path);
                    setFreeDataFuncStack(destroyPathGraph , nodeStack);
                    destroyStack(nodeStack);
                    setFreeDataFuncLRTree(freeInteger , nodeTree);
                    destroyTreeLRTree(nodeTree);
                    return 1;
                }
                int *data = deleteElementLRTree(&path->u , nodeTree);
                if(data != NULL){
                    freeInteger(data);
                }
            }else{
                destroyPathGraph(path);
                path = popStack(nodeStack);
            }
        }while(path != NULL);
    }
    return 0;
}

int isConnectedGraph(VLinkGraph *graph){
    ArrayList * subGraphs = getSubGraphGraph(graph);
    return getSizeAList(subGraphs) == 1;
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

void destroy_strongly_connected_Graph(StronglyConnectedGraph *stronglyConnectedGraph){
    if(stronglyConnectedGraph == NULL){
        return ;
    }
    setFreeDataFuncAList(destroyVlinkGraph , stronglyConnectedGraph->nodeList);
    destroyArrayListAList(stronglyConnectedGraph->nodeList);
    if(stronglyConnectedGraph->adj != NULL){
        for(int i = 0 ; i < stronglyConnectedGraph->nodeNum ; i ++){
            setFreeDataFuncAList(destroyPathGraph , stronglyConnectedGraph->adj[i]);
            destroyArrayListAList(stronglyConnectedGraph->adj[i]);
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

void add_path_to_strongly_connected_Graph(PathGraph *path , int nodeIndex ,StronglyConnectedGraph *stronglyConnectedGraph){
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
        PointRelation *sourcePoint =  gen_point_relation_map(startP , NULL , map);
        insertElementAList(sourcePoint->targetPointList , strongConnectList);
        PathGraph * path = createPathGraph(-1 ,startP ,-1);
        do{
            int relationNodeSize = getSizeAList(graph->adj[path->u]);
            if(path->w < relationNodeSize - 1){
                path->w ++;
                pushStack(path , nodeStack);
                path = createPathGraph(path->u ,((NodeVlinkGraph *)getElementByIndexAList(path->w , graph->adj[path->u]))->u ,-1);
                PointRelation * targetPoint = get_target_point_relation_Graph(path->u , map);
                if(targetPoint == NULL){
                    targetPoint = gen_point_relation_map(path->u , NULL , map);
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
                            PointRelation * circlePoint = get_target_point_relation_Graph(circlePath->u , map);
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
            PointRelation * curPoint = (PointRelation *)getElementByIndexAList(j , strongConnect);
            int nodeSize = getSizeAList(graph->adj[curPoint->sourceP]);
            for(int k =  0 ; k < nodeSize ; k ++){
                NodeVlinkGraph *vLinkNode = (NodeVlinkGraph *)getElementByIndexAList(k , graph->adj[curPoint->sourceP]);
                PointRelation * targetPoint = get_target_point_relation_Graph(vLinkNode->u , map);
                if(targetPoint->targetPointList == curPoint->targetPointList){
                    insertEdgeVLinkDirectGraph(curPoint->targetP , targetPoint->targetP , vLinkNode->w , node);
                }else{
                    PathGraph * path = createPathGraph(curPoint->targetP , targetPoint->targetP , vLinkNode->w);
                    int nodeIndex = getIndexAList(targetPoint->targetPointList , strongConnectList);
                    add_path_to_strongly_connected_Graph(path , nodeIndex , rsl);
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

