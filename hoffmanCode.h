#ifndef HOFFMANCODE_H
#define HOFFMANCODE_H
#include"./arrayList.h"
#include"./heap.h"
#include"./hashTable.h"
#include <math.h>

typedef struct HoffManNode{
    char charactor;
    int times;
    struct HoffManNode * left;
    struct HoffManNode * right;
    struct HoffManNode * parent;
    int code;
    int bitSize;
}HoffManNode;

typedef struct HoffManTree{
    HoffManNode *root;
    int *code;
    int codeBitSize;
}HoffManTree;

const int BIT_SIZE_HOFFMAN = sizeof(int)*8 ;
const unsigned int MAX_INTERGER_HOFFMAN = (1 << (BIT_SIZE_HOFFMAN - 1)) - 1;
const unsigned int MIN_INTERGER_HOFFMAN = 1 << (BIT_SIZE_HOFFMAN - 1);
//编码
HoffManTree *enCodeHoffMan(char * str);
//解码
char * deCodeHoffMan(HoffManTree * tree);

#include "./hoffmanCode.c"

#endif