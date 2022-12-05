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
    DistanceGraph *distance = createDistanceGraph(endNode->height ,endNode->distance);
    int i = endNode->height - 1;
    while(endNode->parent != NULL){
        PathGraph *path = createPathGraph(endNode->parent->u ,endNode->u , endNode->distance - endNode->parent->distance);
        distance->path[i] = path;
        i--;
        endNode = endNode->parent;
    }
    destroyPathTree(pathTree);
    return distance;
}

