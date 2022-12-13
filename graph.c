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
    NodeVlinkGraph * node = (NodeVlinkGraph *)malloc(sizeof(NodeVlinkGraph));
    node->u = vIndex;
    node->w = w;
    return node;
}

void freeInteger(int * p){
    free(p);
    p = NULL;
}

int parsePointToCommon(int * p){
    int rsl = *p;
    free(p);
    return rsl;
}

void destroy_node_vlink_graph(NodeVlinkGraph * node){
    free(node);
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
        free(graph->adj);
        graph->adj = NULL;
    }
    free(graph);
    graph = NULL;   
}

VLinkGraph * createVLinkGraph(int v){
    VLinkGraph * graph = (VLinkGraph *)malloc(sizeof(VLinkGraph));
    graph->v = v;
    graph->e = 0;
    graph->adj = (ArrayList **)malloc(sizeof(ArrayList *) * v);
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
    VMutrixGraph * graph = (VMutrixGraph *)malloc(sizeof(VMutrixGraph));
    graph->v = v;
    graph->e = 0;
    graph->adj = (int **)malloc(sizeof(int *) * v);
    for(int i = 0 ; i < v ; i++){
        graph->adj[i] = (int *)malloc(sizeof(int) * v);
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
    PathGraph * path = (PathGraph *)malloc(sizeof(PathGraph));
    path->v = v;
    path->u = u;
    path->w = w;
    return path;
}

DistanceGraph *createDistanceGraph(int v , int distance){
    DistanceGraph * rsl = (DistanceGraph *)malloc(sizeof(DistanceGraph));
    rsl->path = (PathGraph **)malloc(sizeof(PathGraph *) * v);
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
    free(path);
    path = NULL;
}

void destroyDistanceGraph(DistanceGraph * distance){
    if(distance == NULL){
        return;
    }
    for(int i = 0 ; i < distance->pathNum ; i++){
        destroyPathGraph(distance->path[i]);
    }
    free(distance->path);
    distance->path = NULL;
    free(distance);
    distance = NULL;
}

PointRelation * ceate_point_relation_Graph(int sourceP , int targetP , int graphNo){
    PointRelation *rsl = (PointRelation*)malloc(sizeof(PointRelation));
    rsl->sourceP = sourceP;
    rsl->targetP = targetP;
    rsl->targetGraphNum = graphNo;
    return rsl;
}

void destory_point_relation_Graph(PointRelation * pRelation){
    free(pRelation);
    pRelation = NULL;
}

PointRelation *gen_point_relation_map(int sourceP , int targetP , int graphNo , HashTable * table){
    PointRelation * pRelation = ceate_point_relation_Graph(sourceP , targetP , graphNo);
    insertElementHTable(pRelation , table);
    return pRelation;
}

PointRelation *get_target_point_relation_Graph(int sourceP , HashTable * table){
    PointRelation * pRelation =  getElementsHTable(&sourceP , table);
    return pRelation;
}

VLinkGraph *mergeTwoGraph(VLinkGraph * graph1 , VLinkGraph * graph2 , int graphNo , HashTable * table){
    if(graph1 == NULL || graph2 == NULL){
        printf("graph is null , failed\n");
        return NULL;
    }
    VLinkGraph * newGraph = createVLinkGraph(graph1->v + graph2->v);
    for(int i = 0 ; i < graph1->v ; i++){
        for(int j = 0 ; j < getSizeAList(graph1->adj[i]) ; j++){
            NodeVlinkGraph * node = (NodeVlinkGraph *)getElementByIndexAList(j , graph1->adj[i]);
            if(node->u < 0){
                PointRelation * pRelation = get_target_point_relation_Graph(-node->u , table);
                if(pRelation != NULL && pRelation->targetGraphNum == graphNo){
                    printf("insert edge %d %d %d\n" , pRelation->targetP , graph1->v , node->w);
                    insertEdgeVLinkDirectGraph(i , pRelation->targetP , node->w , newGraph);
                }else{
                    insert_edge_VLinkGraph(i , node->u , node->w , newGraph);
                }
            }else{
                insertEdgeVLinkDirectGraph(i, node->u , node->w , newGraph);
            }
        }
    }
    for(int i = 0 ; i < graph2->v ; i++){
        for(int j = 0 ; j < getSizeAList(graph2->adj[i]) ; j++){
            NodeVlinkGraph * node = (NodeVlinkGraph *)getElementByIndexAList(j , graph2->adj[i]);
            insertEdgeVLinkDirectGraph(i + graph1->v , node->u + graph1->v , node->w , newGraph);
        }
    }
    newGraph->e = graph1->e + graph2->e;
    destroyVlinkGraph(graph1);
    destroyVlinkGraph(graph2);
    return newGraph;
}

void update_point_relation_targetP_Graph(int increaseNumber , int graphNo , HashTable * table){
    for(int i = 0 ; i < table->tableSize ; i++){
        PointRelation * pRelation = getElementsHTable(&i , table);
        if(pRelation != NULL && pRelation->targetGraphNum == graphNo){
            pRelation->targetP += increaseNumber;
        }
    }
}

void update_point_relation_graphNo_Graph(int sourceGraphNo , int targetGraphNo , HashTable * table){
    for(int i = 0 ; i < table->tableSize ; i++){
        PointRelation * pRelation = getElementsHTable(&i , table);
        if(pRelation != NULL && pRelation->targetGraphNum == sourceGraphNo){
            pRelation->targetGraphNum = targetGraphNo;
        }
    }
}

ArrayList *getSubGraphGraph(VLinkGraph * graph){
    ArrayList *subGraphs = createArrayListAList(NULL);
    HashTable *map = createHashTable(graph->v , get_source_p_for_key , NULL , hash_source_p_for_key);
    setHashFactorHTable(1 , map);
    setBlockSizeHTable(graph->v , map);
    TreeLRTree *nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)malloc(sizeof(int));
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
        ArrayList *joinList = createArrayListAList(compare_integer_Graph);
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
                    free(freeData);
                    insert_edge_VLinkGraph(point->targetP , pointU->targetP , vlinkNode->w , subGraph);
                }else{
                    if(pointU->targetGraphNum != graphNo){
                        if(!isExistElementAList(&pointU->targetGraphNum , joinList)){
                            insertElementAList(&pointU->targetGraphNum , joinList);
                        }
                        insert_edge_VLinkGraph(point->targetP , -pointU->sourceP , vlinkNode->w , subGraph);
                    }else{
                        insert_edge_VLinkGraph(point->targetP , pointU->targetP , vlinkNode->w , subGraph);
                    }
                }
            }
        }
        while(!isEmptyAList(joinList)){
            int *joinGraphNo = (int *)deleteElementByIndexAList(0 , joinList);
            VLinkGraph *joinGraph = replaceElementByIndexAList(*joinGraphNo , NULL , subGraphs);
            update_point_relation_targetP_Graph(subGraph->v , *joinGraphNo , map);
            subGraph = mergeTwoGraph( subGraph , joinGraph , *joinGraphNo , map);
            replaceElementByIndexAList(graphNo , subGraph , subGraphs);
            update_point_relation_graphNo_Graph(*joinGraphNo , graphNo , map);
        }
        destroyAList(joinList);
        graphNo ++;
    }
    setFreeDataHTable(destory_point_relation_Graph , map);
    destroyHTable(map);
    destroyCircleQueue(queue);
    destroyTreeLRTree(nodeTree);
    for(int i = getSizeAList(subGraphs) - 1 ; i >= 0 ; i --){
        if(getElementByIndexAList(i , subGraphs) == NULL){
            deleteElementByIndexAList(i , subGraphs);
        }
    }
    return subGraphs;
}

ArrayList *getSubGraphsUnDirectGraph(VLinkGraph *graph){
    ArrayList *subGraphs = createArrayListAList(NULL);
    HashTable *map = createHashTable(graph->v , get_source_p_for_key , NULL , hash_source_p_for_key);
    setHashFactorHTable(1 , map);
    setBlockSizeHTable(graph->v , map);
    TreeLRTree *nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        int *j = (int *)malloc(sizeof(int));
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
                    free(freeData);
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
                free(graph->adj[i]);
                graph->adj[i] = NULL;
            }
            free(graph->adj);
        }
        graph->adj = NULL;
    }
    free(graph);
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
        int *j = (int *)malloc(sizeof(int));
        *j = i;
        insertDataLRTree(j , nodeTree);
    }
    CircleQueue *queue = createCircleQueue(NULL);
    while(!isEmptyLRTree(nodeTree)){
        int *startP = (int *)popMinDataLRTree(nodeTree);
        PathGraph * path = createPathGraph(-1 ,*startP ,UNREACHABLE_GRAPH);
        free(startP);
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
                    free(freeData);
                }else if(vlinkNode->u != path->v){
                    setFreeDataFuncCircleQueue(destroyPathGraph , queue);
                    destroyCircleQueue(queue);
                    setFreeDataFuncLRTree(freeInteger , nodeTree);
                    destroyTreeLRTree(nodeTree);
                    return 1;
                }
            }
            free(path);
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
        int *j = (int *)malloc(sizeof(int));
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

