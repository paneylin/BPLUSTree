#include "./hoffmanCode.h"

HoffManTree *create_HoffManTree_HoffMan(HoffManNode * root , int * code , int codeBitSize){
    HoffManTree * tree = (HoffManTree*)malloc(sizeof(HoffManTree));
    tree->root = root;
    tree->code = code;
    tree->codeBitSize = codeBitSize;
    return tree;
}

HoffManNode * create_HoffMan_node_HoffMan(char charactor , int times){
    HoffManNode * node = (HoffManNode *)malloc(sizeof(HoffManNode));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->times = times;
    node->charactor = charactor;
    node->code = -1;
    node->bitSize = 0;
}

int compare_data_HoffMan(char * charactor1 , char * charactor2){
    return *charactor1 - *charactor2;
}

int compare_data_heap_HoffMan(HoffManNode* node1 , HoffManNode * node2){
    return node2->times - node1->times;
}

char *get_key_HoffMan(HoffManNode *node){
    return &node->charactor;
}

int hash_key_func_HoffMan(char *key){
    unsigned char keyValue = *key;
    return keyValue;
}

void  gen_charactor_identify_HoffMan(ArrayList * listData){
    for(int i = 0 ; i < getSizeAList(listData) ; i ++){
        HoffManNode * node = (HoffManNode *)getElementByIndexAList(i , listData);
        HoffManNode * temp = node;
        int code = -1;
        int bitSize = 0;
        int lastCode = 1;
        while(temp != NULL){
            if(temp->parent != NULL){
                code = code >> 1;
                if(temp->parent->left == temp){
                    if(lastCode == 1){
                        code = code & MAX_INTERGER_HOFFMAN;
                        lastCode = 0;
                    }
                }else{
                    if(lastCode == 0){
                        code = code | MIN_INTERGER_HOFFMAN;
                        lastCode = 1;
                    }
                }
                bitSize++;
            }
            temp = temp->parent;
        }
        node->code = code;
        node->bitSize = bitSize;
        //printf("bitSize is %d code is %x charactor is %c\n" , bitSize , code , node->charactor);
    }
}

void gen_tree_hoffman_node_HoffMan(HoffManNode * parent , HoffManNode * left , HoffManNode * right){
    parent->left = left;
    parent->right = right;
    left->parent = parent;
    right->parent = parent;
}

int set_bit_value_operation_HoffMan(int *codeBeSet , int codeForSet , int setCodeSize ,int startPosition){
    if(codeForSet < 0){
        codeForSet = codeForSet >> startPosition;
    }else if(startPosition > 0){
        codeForSet = codeForSet >> 1;
        codeForSet = codeForSet | MIN_INTERGER_HOFFMAN;
        codeForSet = codeForSet >> (startPosition -1);
    }
    *codeBeSet = *codeBeSet & codeForSet;
    return BIT_SIZE_HOFFMAN - startPosition - setCodeSize < 0 ? -(BIT_SIZE_HOFFMAN - startPosition - setCodeSize) : 0;
}

void set_left_bit_value_operation_HoffMan(int *codeBeSet , int codeForSet , int setCodeSize ,int leftSize){
    codeForSet = codeForSet << (setCodeSize - leftSize);
    int temp = MAX_INTERGER_HOFFMAN >> (BIT_SIZE_HOFFMAN - 1 + leftSize - setCodeSize);
    codeForSet = codeForSet | temp;
    *codeBeSet = *codeBeSet & codeForSet;
}

HoffManTree *gen_HoffManTree_HoffMan(HoffManNode * root , HashTable * table , char * str , int codeSize){
    int charSize = codeSize/BIT_SIZE_HOFFMAN == 0 ? codeSize/BIT_SIZE_HOFFMAN : codeSize/BIT_SIZE_HOFFMAN + 1;
    int * codeRsl = (int *)malloc(sizeof(int)*charSize);
    memset(codeRsl, -1, sizeof(int )*charSize);
    HoffManTree * tree = create_HoffManTree_HoffMan(root , codeRsl , codeSize);
    int i = 0;
    int startPosition = 0 , leftSIze = 0;
    while(str[i] != '\0'){
        HoffManNode * node = (HoffManNode *)getElementsHTable(&str[i] , table);
        leftSIze = set_bit_value_operation_HoffMan(codeRsl , node->code , node->bitSize , startPosition);
        //printf("%x code is %x bitSize is %d startPosition is %d leftSize is %d , codeRsl is %x \n" ,node->charactor , node->code , node->bitSize , startPosition , leftSIze , *codeRsl);
        if(leftSIze > 0){
            codeRsl++;
            set_left_bit_value_operation_HoffMan(codeRsl , node->code , node->bitSize , leftSIze);
            //printf("%x code is %x bitSize is %d  leftSize is %d , codeRsl is %x \n" ,node->charactor , node->code , node->bitSize  , leftSIze , *codeRsl);
            startPosition = leftSIze;
        }else{
            startPosition += node->bitSize;
            if(startPosition == BIT_SIZE_HOFFMAN){
                startPosition = 0;
                codeRsl++;
            }
        }
        i++;
    }
    return tree;
}

HoffManTree *enCodeHoffMan(char * str){
    HashTable *table = createHashTable(100 , get_key_HoffMan , compare_data_HoffMan , hash_key_func_HoffMan);
    int i = 0 ;
    while(str[i] != '\0'){
        HoffManNode * charNode = getElementsHTable(&str[i] , table);
        if(charNode == NULL){
            charNode = create_HoffMan_node_HoffMan(str[i] , 1);
            insertElementHTable(charNode , table);
        }else{
            charNode->times++;
        }
        i++;
    }
    ArrayList *list = getAllDatasInTableHTable(table);
    HeapInfo * heap = createHeapInfoHeap(100,compare_data_heap_HoffMan);
    for(int i = 0 ; i < getSizeAList(list) ; i ++){
        insertElementHeap(getElementByIndexAList(i , list),heap);
    }
    while(getHeapSizeHeap(heap) > 1){
        HoffManNode * node1 = popElementHeap(heap);
        HoffManNode * node2 = popElementHeap(heap);
        HoffManNode * node3 = create_HoffMan_node_HoffMan('\0' , node1->times + node2->times);
        //printf("node1->times is %d %c node2->times is %d %c\n" , node1->times , node1->charactor , node2->times , node2->charactor);
        gen_tree_hoffman_node_HoffMan(node3 , node1 , node2);
        insertElementHeap(node3,heap);
    }
    gen_charactor_identify_HoffMan(list);
    int codeSize = 0;
    for(int i = 0 ; i < getSizeAList(list) ; i ++){
        HoffManNode * node = (HoffManNode *)getElementByIndexAList(i , list);
        codeSize += node->times * node->bitSize;
    }
    destroyAList(list);
    HoffManTree * hoffTree = gen_HoffManTree_HoffMan(popElementHeap(heap) , table , str ,codeSize);  
    destroyHTable(table);
    destroyHeapInfoHeap(heap);
    return hoffTree;
}



char * deCodeHoffMan(HoffManTree * tree){
    int totalCodeSize = tree->codeBitSize;
    CircleQueue * queue = createCircleQueue();
    int * code = tree->code;
    HoffManNode * node = tree->root;
    int charactorSize = 0;
    //printf("code is %x\n" , *code);
    for(int i = 0 ; i < totalCodeSize ; i ++){
        if(i > 0 && i % BIT_SIZE_HOFFMAN == 0){
            code++;
            //printf("code is %x\n" , *code);
        }
        if(*code >= 0){
            node = node->left;
            //printf("0 ");
        }else{
            node = node->right;
            //printf("1 ");
        }
        *code = *code << 1;
        if(node->charactor != '\0'){
            //printf("charactor is %x index is %d\n" , node->charactor , i);
            pushCircleQueue(&node->charactor , queue);
            charactorSize ++;
            node = tree->root;
        }
    }
    char * rsl = malloc(sizeof(char)*(charactorSize + 1));
    int i = 0;
    while(!isEmptyCircleQueue(queue)){
        rsl[i] = *(char *)popCircleQueue(queue);
        i++;
    }
    destroyCircleQueue(queue);
    rsl[i] = '\0';
    return rsl;
}