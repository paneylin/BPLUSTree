#include"./graph.h"

int *get_source_p_for_key(PointRelation *pRelation){
    return &pRelation->sourceP;
}

int hash_source_p_for_key(int *sourceP){
    return *sourceP;
}

int compare_npde_vlink_Graph(NodeVlinkGraph * a, NodeVlinkGraph * b){
    return a->u == b->u;
}

NodeVlinkGraph *create_node_vlink_graph(int vIndex , int w){
    NodeVlinkGraph * node = (NodeVlinkGraph *)malloc(sizeof(NodeVlinkGraph));
    node->u = vIndex;
    node->w = w;
    return node;
}

void destroy_node_vlink_graph(NodeVlinkGraph * node){
    free(node);
    node = NULL;
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
        printf("index out of range , insertEdgeVLinkDirectGraph failed\n");
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
        graph->e++;
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
        graph->e++;
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
    for(int i = 0 ; i < distance->pathNum ; i++){
        destroyPathGraph(distance->path[i]);
    }
    free(distance->path);
    distance->path = NULL;
    free(distance);
    distance = NULL;
}

PointRelation * ceate_point_relation_Graph(int sourceP , int targetP){
    PointRelation *rsl = (PointRelation*)malloc(sizeof(PointRelation));
    rsl->sourceP = sourceP;
    rsl->targetP = targetP;
    return rsl;
}

void gen_point_relation_map(int sourceP , int targetP , HashTable * table){
    PointRelation * pRelation = ceate_point_relation_Graph(sourceP , targetP);
    insertElementHTable(pRelation , table);
}

int get_target_point_relation_Graph(int sourceP , HashTable * table){
    PointRelation * pRelation =  getElementsHTable(&sourceP , table);
    if(pRelation == NULL){
        return -1;
    }else{
        return pRelation->targetP;
    }

}

ArrayList *getSubGraphsGraph(VLinkGraph *graph){
    ArrayList *subGraphs = createArrayListAList(NULL);
    HashTable *map = createHashTable(graph->v , get_source_p_for_key , NULL , hash_source_p_for_key);
    setHashFactorHTable(1 , map);
    setBlockSizeHTable(graph->v , map);
    TreeLRTree *nodeTree = createTreeLRTree(NULL , NULL);
    for(int i = 0 ; i < graph->v ; i ++){
        insertDataLRTree(&i , nodeTree);
    }
    CircleQueue *queue = createCircleQueue(NULL);
    while(!isEmptyLRTree(nodeTree)){
        VLinkGraph * subGraph = createVLinkGraph(0);
        insertElementAList(subGraph , subGraphs);
        int newTargetP = 0;
        int *souceP = (int *)popMinDataLRTree(nodeTree);
        gen_point_relation_map(*souceP , newTargetP++ , map);
        pushCircleQueue(souceP , queue);
        while(!isEmptyCircleQueue(queue)){
            souceP = popCircleQueue(queue);
            int targetStartP = get_target_point_relation_Graph(souceP , map);
            add_point_VLinkGraph(subGraph);
            int relationNodeSize = getSizeAList(graph->adj[*souceP]);
            for(int i = 0 ; i < relationNodeSize ; i ++){
                NodeVlinkGraph * vlinkNode = getElementByIndexAList(i , graph->adj[*souceP]);
                int targetP = get_target_point_relation_Graph(vlinkNode->u , map);
                if(targetP == -1){
                    gen_point_relation_map(vlinkNode->u , newTargetP ++ ,map);
                    targetP = newTargetP - 1;
                }
                insert_edge_VLinkGraph(targetStartP , targetP , vlinkNode->w,subGraph);
                if(getDataFromTreeLRTree(&vlinkNode->u , nodeTree)){
                    pushCircleQueue(vlinkNode->u , queue);
                    deleteElementLRTree(&vlinkNode->u , nodeTree);
                }
            }
        }
    }
    return subGraphs;
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
        graph->adj = NULL;
    }
    free(graph);
    graph = NULL;
    
}