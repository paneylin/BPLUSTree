#include"./pathTree.h"

void destroy_node_PathTree(NodePathTree *node){
    if(node == NULL){
        return;
    }
    node->parent = node->next = node->pre = node->child = NULL;
    free(node);
}

void destroyPathTree(PathTree *tree){
    if(tree == NULL){
        return;
    }
    setFreeDataFuncAList(destroy_node_PathTree , tree->nodeList);
    if(tree->nodeList != NULL){
        destroyAList(tree->nodeList);
    }
    tree->root = NULL;
    free(tree);
    tree = NULL;
}

NodePathTree *create_node_PathTree(int u){
    NodePathTree *node = (NodePathTree *)malloc(sizeof(NodePathTree));
    node->u = u;
    node->height = 0;
    node->distance = UNREACHABLE_GRAPH;
    node->parent = node->next = node->pre = node->child = NULL;
    return node;
}

NodePathTree *get_node_from_tree_PathTree(int u , PathTree *tree){
    if(getSizeAList(tree->nodeList) < u){
        printf("Error: u is out of range in get_node_from_tree_PathTree\n");
        return NULL;
    }
    return getElementByIndexAList(u , tree->nodeList);
}

void set_node_child_PathTree(NodePathTree *node , NodePathTree *parent){
    node->parent = parent;
    if(parent->child == NULL){
        parent->child = node;
        node->next = node;
        node->pre = node;
    }else{
        NodePathTree *child = parent->child;
        child->next->pre = node;
        node->next = child->next;
        child->next = node;
        node->pre = child;
    }
}

void cut_node_relation_PathTree(NodePathTree *node){
    if(node->next == node){
        node->parent->child = NULL;
    }else{
        node->next->pre = node->pre;
        node->pre->next = node->next;
        if(node->parent->child == node){
            node->parent->child = node->next;
        }
        node->next = node->pre = NULL;
    }
    node->parent = NULL;
}

void short_distance_node(NodePathTree * node , NodePathTree *pNode , int shortDIstance){
    node->distance = node->distance - shortDIstance;
    node->height = pNode->height + 1;
    if(node->child != NULL){
        NodePathTree *child = node->child;
        do{
            short_distance_node(child , node , shortDIstance);
            child = child->next;
        }while (child != node->child);
    }
}

NodePathTree *reGen_node_tree_PathTree(int v , int u , int w , PathTree *tree){
    NodePathTree *node = get_node_from_tree_PathTree(u , tree);
    NodePathTree *parent = get_node_from_tree_PathTree(v , tree);
    if(node->distance == UNREACHABLE_GRAPH){
        set_node_child_PathTree(node , parent);
        node->distance = w + parent->distance;
        node->height = parent->height + 1;
    }else if(node->distance > w + parent->distance){
        cut_node_relation_PathTree(node);
        set_node_child_PathTree(node , parent);
        int shortDIstance = node->distance - w - parent->distance;
        short_distance_node(node , parent ,shortDIstance);
    }
}

PathTree *create_path_tree_PathTree(int start ,int  vNum){
    if(start < 0 || start >= vNum || vNum <= 0){
        printf("Error: start is out of range in create_path_tree_PathTree\n");
        return NULL;
    }
    PathTree *tree = (PathTree *)malloc(sizeof(PathTree));
    tree->nodeList = createArrayListAList(NULL);
    NodePathTree *node = NULL;
    for(int i = 0 ; i < vNum ; i++){
        node = create_node_PathTree(i);
        insertElementAList(node , tree->nodeList);
    }
    node = getElementByIndexAList(start , tree->nodeList);
    if(node == NULL){
        printf("Error: start node is null in create_path_tree_PathTree\n");
        return NULL;
    }
    node->distance = 0;
    tree->root = node;
}

PathTree *gen_pathtree_PathTree(int start , VLinkGraph *graph){
    if(start < 0 || start >= graph->v || graph->v <= 0){
        printf("Error: start is out of range in gen_pathtree_PathTree\n");
        return NULL;
    }
    PathTree * tree = create_path_tree_PathTree(start , graph->v);
    CircleQueue *queue = createCircleQueue();
    pushCircleQueue(&start , queue);
    TreeLRTree *lrtree = createTreeLRTree(NULL , NULL);
    insertDataLRTree(&start , lrtree);
    while (!isEmptyCircleQueue(queue)){
        int v = *(int *)popCircleQueue(queue);
        int adjNum = getSizeAList(graph->adj[v]);
        for(int i = 0 ; i <adjNum ; i ++){
            NodeVlinkGraph *node = getElementByIndexAList(i , graph->adj[v]);
            //printf("v = %d , u = %d, w = %d\n",v , node->u , node->w);
            reGen_node_tree_PathTree(v , node->u , node->w , tree);
            if(getDataFromTreeLRTree(&node->u , lrtree) == NULL){
                insertDataLRTree(&node->u , lrtree);
                pushCircleQueue(&node->u , queue);
            }        }
    }
    destroyCircleQueue(queue);
    return tree;
}

DistanceGraph *create_distance_by_node_PathTree(NodePathTree *node){
    if(node == NULL){
        printf("Error: node is null in create_distance_by_node_PathTree\n");
        return NULL;
    }
    DistanceGraph *distance = createDistanceGraph(node->height ,node->distance);
    int i = node->height - 1;
    while(node->parent != NULL){
        PathGraph *path = createPathGraph(node->parent->u ,node->u , node->distance - node->parent->distance);
        distance->path[i] = path;
        i--;
        node = node->parent;
    }
    return distance;
}

DistanceGraph* getShortestDistancePathTree(int start , int end , VLinkGraph *graph){
    if(start < 0 || start >= graph->v || end < 0 || end >= graph->v){
        printf("Error: start or end is out of range in getShortestDistancePathTree\n");
        return NULL;
    }
    PathTree *pathTree = gen_pathtree_PathTree(start , graph);
    NodePathTree * endNode = get_node_from_tree_PathTree(end , pathTree);
    if(endNode->distance == UNREACHABLE_GRAPH){
        return NULL;
    }
    DistanceGraph *distance = create_distance_by_node_PathTree(endNode);
    destroyPathTree(pathTree);
    return distance;
}

ArrayList *get_all_leaf_node_PathTree(PathTree *tree){
    ArrayList *leafList = createArrayListAList(NULL);
    ArrayList *nodeList = tree->nodeList;
    int size = getSizeAList(nodeList);
    for(int i = 0 ; i < size ; i++){
        NodePathTree *node = getElementByIndexAList(i , nodeList);
        if(node->child == NULL){
            insertElementAList(node , leafList);
        }
    }
    return leafList;
}

DistanceGraph * get_longest_path_PathTree(PathTree *pathTree){
    if(pathTree == NULL){
        printf("pathtree is none , no logest path in get_longest_path_PathTree\n");
        return NULL;
    }
    ArrayList *leafList = get_all_leaf_node_PathTree(pathTree);
    int size = getSizeAList(leafList);
    int maxDistance = 0;
    int maxIndex = 0;
    for(int i = 0 ; i < size ; i++){
        NodePathTree *node = getElementByIndexAList(i , leafList);
        if(node->distance > maxDistance){
            maxDistance = node->distance;
            maxIndex = i;
        }
    }
    NodePathTree *node = getElementByIndexAList(maxIndex , leafList);
    DistanceGraph *distance = create_distance_by_node_PathTree(node);
    destroyAList(leafList);
    return distance;
}

DistanceGraph *get_Sub_Graph_Diameter_PathTree(VLinkGraph *subGraph){
    if(subGraph == NULL || subGraph->v <= 0){
        printf("Error: subGraph is null in get_Sub_Graph_Diameter_PathTree\n");
        return NULL;
    }
    PathTree *pathTree = gen_pathtree_PathTree(0 , subGraph);
    ArrayList *leafList = get_all_leaf_node_PathTree(pathTree);
    DistanceGraph * rsl = get_longest_path_PathTree(pathTree);
    int size = getSizeAList(leafList);
    for(int i = 0 ; i < size ; i++){
        NodePathTree *node = getElementByIndexAList(i , leafList);
        PathTree *leafPathTree = gen_pathtree_PathTree(node->u , subGraph);
        DistanceGraph * tempRsl = get_longest_path_PathTree(leafPathTree);
        if(tempRsl->distance > rsl->distance){
            destroyDistanceGraph(rsl);
            rsl = tempRsl;
        }else{
            destroyDistanceGraph(tempRsl);
        }
        destroyPathTree(leafPathTree);
    }
    destroyPathTree(pathTree);
    return rsl;
}

DistanceGraph *getDiameterPathTreeUndirectPathTree(VLinkGraph * graph){
    ArrayList * subGraphs = getSubGraphsUnDirectGraph(graph);
    DistanceGraph * rsl = NULL;
    int size = getSizeAList(subGraphs);
    for(int i = 0 ; i < size ; i ++){
        printf("i = %d , size = %d\n",i , size);
        VLinkGraph *subGraph = getElementByIndexAList(i , subGraphs);
        DistanceGraph *tempRsl = get_Sub_Graph_Diameter_PathTree(subGraph);
        if(rsl == NULL || tempRsl->distance > rsl->distance){
            destroyDistanceGraph(rsl);
            rsl = tempRsl;
        }else{
            destroyDistanceGraph(tempRsl);
        }
    }
    setFreeDataFuncAList(destroyVlinkGraph , subGraphs);
    destroyAList(subGraphs);
    return rsl;
}

