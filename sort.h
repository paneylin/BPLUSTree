#ifndef SORT_H
#define SORT_H

#include<stdio.h>
#include<stdlib.h>
#include<String.h>
#include<math.h>
//快速排序
void quickSort(void ** data , int size , int (*compareFUnc)(void * , void *));
//选择排序
void selectSort(void ** data , int size , int (*compareFUnc)(void * , void *));
//插入排序
void insertSort(void ** data , int size , int (*compareFunc)(void * , void *));
//归并排序
void combindSort(void ** data , int size , int (*compareFunc)(void * , void *));
//二分搜索，返回key的位置，如果key不存在，返回key所在区间的前一个位置，如果key在第一个元素之前，返回-1
int searchPrevDataBinarySort(void ** data , int size , void * key ,int (*compareFunc)(void * , void *));
//二分查找，返回key的位置，如果key不存在，返回-1
int binarySearchSort(void ** data , int size , void * key ,int (*compareFunc)(void * , void *));
//二分查找，返回key的位置，如果key不存在，返回-1
int binarySearchHasKeySort(void ** data , int size , void * key ,void *(*getKeyFcun)(void *) ,int (*compareFunc)(void * , void *));
//二分查找，返回key的位置，如果key不存在，key前面的一个元素位置
int searchPrevDataHasKeyBinarySort(void ** data , int size , void * key ,void *(*getKeyFcun)(void *) ,int (*compareFunc)(void * , void *));

#include "./sort.c"

#endif