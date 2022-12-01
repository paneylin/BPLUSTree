#include"./vanEmdeBoasTree.h"

void destory_summary_VEBTree(SummaryVEBTree * summary){
    summary->next = summary->pre = NULL;
    free(summary);
    summary = NULL;
}

void destory_node_VEBTree(NodeVanEmdeBoasTree * node){
    if(node == NULL){
        return;
    }
    if(node->lgNumber == 1){
        free(node);
        node = NULL;
        return;
    }
    for(int i = 0 ; i < node->nextU ; i ++){
        destory_summary_VEBTree(node->summary[i]);
    }
    for(int i = 0 ; i < node->clusterSize ; i ++){
        destory_node_VEBTree(node->cluster[i]);
    }
    free(node->summary);
    node->summary = NULL;
    free(node->cluster);
    node->cluster = NULL;
    free(node);
    node = NULL;
}

int get_lgNumber_VEBTree(int number){
    int lgNumber = 0;
    int temp = 1;
    while(temp < number){
        temp = temp << 1;
        lgNumber ++;
    }
    return lgNumber;
}

int get_ceilLogNumber_VEBTree(int logNumber){
    return logNumber/2;
}

int get_next_u_VEBTree(int currentLgNumber){
    int nexrLgNumber = get_ceilLogNumber_VEBTree(currentLgNumber);
    int nexrU = 1;
    while(nexrLgNumber > 0){
        nexrU = nexrU << 1;
        nexrLgNumber -- ;
    }
    return nexrU;
}

int get_summary_index_VEBTree(int key , NodeVanEmdeBoasTree * node){
    return key /(node->u/node->nextU);
}

int get_low_index_VEBTree(int key , NodeVanEmdeBoasTree * node){
    return key % (node->u/node->nextU);
}

int cal_key_VEBTree(int summaryIndex , int lowIndex , NodeVanEmdeBoasTree * node){
    return summaryIndex * (node->u/node->nextU) + lowIndex;
}

DataVanEmdeBoasTree * create_data_VEBTree(int dataSize){
    DataVanEmdeBoasTree * data = (DataVanEmdeBoasTree *)malloc(sizeof(DataVanEmdeBoasTree));
    data->dataList = createLinkListLList(NULL , NULL);
    while(dataSize > 0){
        void ** dataBlock = (void **)malloc(sizeof(void *) * DATA_BLOCK_SIZE_VEBTREE);
        for(int i = 0 ;i < DATA_BLOCK_SIZE_VEBTREE ; i ++){
            dataBlock[i] = NULL;
        }
        insertElementLList(dataBlock , data->dataList);
        dataSize -= DATA_BLOCK_SIZE_VEBTREE;
    }
    return data;
}

SummaryVEBTree * create_summary_VEBTree(int summaryPosition){
    SummaryVEBTree * summary = (SummaryVEBTree *)malloc(sizeof(SummaryVEBTree));
    summary->position = -1;
    summary->summaryPosition = summaryPosition;
    summary->pre = NULL;
    summary->next = NULL;
}

NodeVanEmdeBoasTree * create_tree_node_VEBTree(int lgNumber){
    NodeVanEmdeBoasTree * node = (NodeVanEmdeBoasTree *)malloc(sizeof(NodeVanEmdeBoasTree));
    node->lgNumber = lgNumber;
    node->u = 1 << lgNumber;
    node->min = -1;
    node->max = -1;
    node->nextU = get_next_u_VEBTree(lgNumber);
    node->maxSize = node->nextU / DEFAULT_DIVICE_NUM <= MIN_MEMORRY_SIZE ? node->nextU : node->nextU / DEFAULT_DIVICE_NUM;
    if(lgNumber > 1){
        node->summary = (SummaryVEBTree **)malloc(sizeof(SummaryVEBTree *) * node->nextU);
        for(int i = 0 ; i < node->nextU ; i ++){
            node->summary[i] = create_summary_VEBTree(i);
        }
        node->cluster = (NodeVanEmdeBoasTree **)malloc(sizeof(NodeVanEmdeBoasTree *) * node->maxSize);
        for(int i = 0 ; i < node->maxSize ; i ++){
            node->cluster[i] = NULL;
        }
    }else{
        node->summary = NULL;
        node->cluster = NULL;
    }
    node->clusterSize = 0;
    return node;
}

VanEmdeBoasTree *createVanEmdeBoasTree(int number , int (*getKeyFunc)(void *)){
    VanEmdeBoasTree * tree = (VanEmdeBoasTree *)malloc(sizeof(VanEmdeBoasTree));
    int lgNumber = get_lgNumber_VEBTree(number);
    tree->root = create_tree_node_VEBTree(lgNumber);
    tree->dataSize = number;
    tree->data = create_data_VEBTree(number);
    tree->getKeyFunc = getKeyFunc;
    return tree;
} 

void *delete_data_from_dataBlock_VEBTree(int key , VanEmdeBoasTree * tree){
    int blockIndex = key/DATA_BLOCK_SIZE_VEBTREE;
    void ** dataBlock =  getElementByIndexLList(blockIndex, tree->data->dataList);
    int lowIndex = key % DATA_BLOCK_SIZE_VEBTREE;
    void * data = dataBlock[lowIndex];
    dataBlock[lowIndex] = NULL;
    return data;
}

void *findDataInVEBTree(int key , VanEmdeBoasTree * tree){
    if(key > tree->dataSize || key < 0){
        printf("key is out of range\n",key);
        return NULL;
    }
    int blockIndex = key/DATA_BLOCK_SIZE_VEBTREE;
    void ** dataBlock =  getElementByIndexLList(blockIndex, tree->data->dataList);
    int lowIndex = key % DATA_BLOCK_SIZE_VEBTREE;
    return dataBlock[lowIndex];
}

void insert_data_to_dataBlock_VEBTree(void * data , int key , VanEmdeBoasTree * tree){
    int blockIndex = key/DATA_BLOCK_SIZE_VEBTREE;
    void ** dataBlock =  getElementByIndexLList(blockIndex, tree->data->dataList);
    int lowIndex = key % DATA_BLOCK_SIZE_VEBTREE;
    dataBlock[lowIndex] = data;
}

void resize_cluster_VEBTree(NodeVanEmdeBoasTree * node){
    int newSize = node->maxSize * 2;
    NodeVanEmdeBoasTree ** tempPt = (NodeVanEmdeBoasTree **)realloc(node->cluster , sizeof(NodeVanEmdeBoasTree *) * newSize);
    if(tempPt != NULL){
        node->cluster = tempPt;
        for(int i = node->maxSize ; i < newSize ; i ++){
            node->cluster[i] = NULL;
        }
        node->maxSize = newSize;
    }
}

int cluster_Exist_VEBTree(NodeVanEmdeBoasTree * node , int summaryIndex){
    return node->summary[summaryIndex]->position != -1 && node->cluster[node->summary[summaryIndex]->position] != NULL;
}

void insert_data_to_tree_VEBTree(int key , NodeVanEmdeBoasTree *node){
    printf("insert key %d, lg is %d , nextU is %d  , u is %d\n" , key , node->lgNumber , node->nextU , node->u);
    if(node->min == -1){
        node->min = key;
        node->max = key;
    }
    if(key < node->min){
        node->min = key;
    }
    if(key > node->max){
        node->max = key;
    }
    if(node->lgNumber == 1){
        node->cluster ++;
        return ;
    }
    int summaryIndex = get_summary_index_VEBTree(key , node);
    int lowIndex = get_low_index_VEBTree(key , node);
    if(!cluster_Exist_VEBTree(node , summaryIndex)){
        if(node->summary[summaryIndex]->next == NULL){
            for(int i = 0 ; i < node->nextU ; i ++){
                node->summary[i]->next = node->summary[summaryIndex];
                node->summary[i]->pre = node->summary[summaryIndex];
            }
        }else{
            if(node->clusterSize == node->maxSize){
                resize_cluster_VEBTree(node);
            }
            node->summary[summaryIndex]->pre->next = node->summary[summaryIndex];
            node->summary[summaryIndex]->next->pre = node->summary[summaryIndex];
            int lowPosition = node->summary[summaryIndex]->pre->summaryPosition;
            for(int i = lowPosition + 1 ; i < summaryIndex ; i ++){
                node->summary[i]->next = node->summary[summaryIndex];
            }
            if(lowPosition > summaryIndex){
                for(int i = 0 ; i < summaryIndex ; i ++){
                    node->summary[i]->next = node->summary[summaryIndex];
                }
                for(int i = lowPosition + 1 ; i < node->nextU ; i ++){
                    node->summary[i]->next = node->summary[summaryIndex];
                }
            }
            int highPosition = node->summary[summaryIndex]->next->summaryPosition;
            for(int i = summaryIndex + 1 ; i < highPosition ; i ++){
                node->summary[i]->pre = node->summary[summaryIndex];
            }
            if(highPosition < summaryIndex){
                for(int i = summaryIndex + 1 ; i < node->nextU ; i ++){
                    node->summary[i]->pre = node->summary[summaryIndex];
                }
                for(int i = 0 ; i < highPosition ; i ++){
                    node->summary[i]->pre = node->summary[summaryIndex];
                }
            }
        } 
        if(node->summary[summaryIndex]->position == -1){
            node->summary[summaryIndex]->position = node->clusterSize;
            node->clusterSize ++;
        }
        node->cluster[node->summary[summaryIndex]->position] = create_tree_node_VEBTree(node->lgNumber - get_ceilLogNumber_VEBTree(node->lgNumber));
    }
    insert_data_to_tree_VEBTree(lowIndex , node->cluster[node->summary[summaryIndex]->position]);
}

void insertDataVEBTree(void *data , VanEmdeBoasTree *tree){
    int key = tree->getKeyFunc(data);
    if(key < 0 || key >= tree->dataSize){
        printf("key out of index , insert failed!\n");
        return;
    }
    void * existData = findDataInVEBTree(key , tree);
    if(existData != NULL){
        printf("key already exist , insert failed!\n");
        return;
    }
    insert_data_to_dataBlock_VEBTree(data , key , tree);
    insert_data_to_tree_VEBTree(key , tree->root);
}

int get_max_VEBTree(NodeVanEmdeBoasTree * node){
    return node->max;
}

int get_min_VEBTree(NodeVanEmdeBoasTree * node){
    return node->min;
}

int find_next_key_VEBTree(int key , NodeVanEmdeBoasTree * node){
    if(key >= node->max){
        return -1;
    }
    if(node->lgNumber == 1){
        return 1;
    }
    int lowNumber = 0;
    int highKey = get_summary_index_VEBTree(key , node);
    int lowKey = get_low_index_VEBTree(key , node);
    if(cluster_Exist_VEBTree(node , highKey)){
        if(node->cluster[node->summary[highKey]->position]->max > lowKey){
            lowNumber = find_next_key_VEBTree(lowKey , node->cluster[node->summary[highKey]->position]);
        }else{
            lowNumber = get_min_VEBTree(node->cluster[node->summary[highKey]->next->position]);
            highKey  = node->summary[highKey]->next->summaryPosition;
        }
    }else{
        lowNumber = get_min_VEBTree(node->cluster[node->summary[highKey]->next->position]);
        highKey  = node->summary[highKey]->next->summaryPosition;
    }
    return cal_key_VEBTree(highKey , lowNumber, node);
}

void * getNextDataVEBTree(int key , VanEmdeBoasTree * tree){
    int newKey = find_next_key_VEBTree(key , tree->root);
    if(newKey == -1){
        return NULL;
    }else{
        return findDataInVEBTree(newKey , tree);
    }
}

int find_pre_key_VEBTree(int key , NodeVanEmdeBoasTree * node){
    if(key <= node->min){
        return -1;
    }
    if(node->lgNumber == 1){
        return 0;
    }
    int lowNumber = 0;
    int highKey = get_summary_index_VEBTree(key , node);
    int lowKey = get_low_index_VEBTree(key , node);
    if(cluster_Exist_VEBTree(node , highKey)){
        if(node->cluster[node->summary[highKey]->position]->min < lowKey){
            lowNumber = find_pre_key_VEBTree(lowKey , node->cluster[node->summary[highKey]->position]);
        }else{
            lowNumber = get_max_VEBTree(node->cluster[node->summary[highKey]->pre->position]);
            highKey  = node->summary[highKey]->pre->summaryPosition;
        }
    }else{
        lowNumber = get_max_VEBTree(node->cluster[node->summary[highKey]->pre->position]);
        highKey  = node->summary[highKey]->pre->summaryPosition;
    }
    return cal_key_VEBTree(highKey , lowNumber, node);
}

void * getPreDataVEBTree(int key , VanEmdeBoasTree * tree){
    int newKey = find_pre_key_VEBTree(key , tree->root);
    if(newKey == -1){
        return NULL;
    }else{
        return findDataInVEBTree(newKey , tree);
    }
}

int delete_data_from_tree_VEBTree(int key , NodeVanEmdeBoasTree *node){
    if(node->lgNumber == 1){
        if(node->max == node->min){
            node->max = -1;
            node->min = -1;
            return -1;
        }else{
            if(key == node->max){
                node->max = node->min;
            }else{
                node->min = node->max;
            }
            return 1;
        }
    }
    int summaryIndex = get_summary_index_VEBTree(key , node);
    int lowIndex = get_low_index_VEBTree(key , node);
    if(!cluster_Exist_VEBTree(node , summaryIndex)){
        printf("key not exist , delete failed!\n");
        return 1;
    }
    NodeVanEmdeBoasTree * childNode = node->cluster[node->summary[summaryIndex]->position];
    int deleteFlag = delete_data_from_tree_VEBTree(lowIndex , childNode);
    printf("%d %d %d %d\n" , deleteFlag , key ,node->min , node->max);
    if(deleteFlag == -1){
        if(node->min == node->max){
            node->min = -1;
            node->max = -1;
            destory_node_VEBTree(childNode);
            node->cluster[node->summary[summaryIndex]->position] = NULL;
            return -1;
        }else{
            if(key == node->min){
                int lowNumber = get_min_VEBTree(node->cluster[node->summary[summaryIndex]->next->position]);
                node->min = cal_key_VEBTree(node->summary[summaryIndex]->next->summaryPosition , lowNumber , node);
            }
            if(key == node->max){
                int lowNumber = get_max_VEBTree(node->cluster[node->summary[summaryIndex]->pre->position]);
                node->max = cal_key_VEBTree(node->summary[summaryIndex]->pre->summaryPosition , lowNumber , node);
            }
            int highPosition = node->summary[summaryIndex]->next->summaryPosition;
            for(int i = summaryIndex + 1 ; i <= highPosition ; i ++){
                node->summary[i]->pre = node->summary[summaryIndex]->pre;
            }
            if(highPosition < summaryIndex){
                for(int i = summaryIndex + 1; i < node->nextU ; i ++){
                    node->summary[i]->pre = node->summary[summaryIndex]->pre;
                }
                for(int i = 0 ; i <= highPosition ; i ++){
                    node->summary[i]->pre = node->summary[summaryIndex]->pre;
                }
            }
            int lowPosition = node->summary[summaryIndex]->pre->summaryPosition;
            for(int i = lowPosition ; i < summaryIndex ; i ++){
                node->summary[i]->next = node->summary[summaryIndex]->next;
            }
            if(lowPosition > summaryIndex){
                for(int i =  0 ; i < summaryIndex ; i ++){
                    node->summary[i]->next = node->summary[summaryIndex]->next;
                }
                for(int i = lowPosition ; i < node->nextU ; i ++){
                    node->summary[i]->next = node->summary[summaryIndex]->next;
                }
            }
            destory_node_VEBTree(childNode);
            node->cluster[node->summary[summaryIndex]->position] = NULL;
            return 1;
        }
    }else{
        if(key == node->max){
            int lowNumber = find_pre_key_VEBTree(lowIndex , childNode);
            node->max = cal_key_VEBTree(summaryIndex , lowNumber , node);
        }
        if(key == node->min){
            int lowNumber = find_next_key_VEBTree(lowIndex , childNode);
            node->min = cal_key_VEBTree(summaryIndex , lowNumber , node);
        }
    }
    return 1;
}

void *deleteDataVEBTree(int key , VanEmdeBoasTree * tree){
    if(key < 0 || key >= tree->dataSize){
        printf("key out of index , delete failed!\n");
        return NULL;
    }
    void * existData = findDataInVEBTree(key , tree);
    if(existData == NULL){
        printf("key not exist , delete failed!\n");
        return NULL;
    }
    delete_data_from_tree_VEBTree(key , tree->root);
    return delete_data_from_dataBlock_VEBTree(key , tree);
}