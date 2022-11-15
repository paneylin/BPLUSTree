#include "./sort.h"

int rand_position_sort(int start , int end){
    return rand() % (end - start) + start;
}

void select_sort(void ** data , int start , int end  , int (*comparefunc)(void * , void *)){
    for(int i = start;i < end;i ++){
        int min = i;
        for(int j = i + 1;j <= end;j ++){
            if(comparefunc(data[min] , data[j]) > 0){
                min = j;
            }
        }
        void *temp = data[i];
        data[i] = data[min];
        data[min] = temp;
    }
}

int partition_sort(void ** data , int start , int end , int position , int (*compareFunc)(void * , void *)){
    void *standardData = data[position];
    int pos;
    while(start <= end){
        int value = compareFunc(data[start] , standardData);
        if(value > 0){
            void *temp = data[end];
            data[end] = data[start];
            data[start] = temp;
            end --;
        }else if(value == 0){
            pos = start;
            start ++;
        }else{
            start ++;
        }
    }
    start --;
    void *temp = data[start];
    data[start] = data[pos];
    data[pos] = temp;
    return start;
}

void quick_sort(void ** data , int start , int end , int (*compareFunc)(void * , void *)){
    if(end - start < 8){
        select_sort(data , start , end ,compareFunc);
        return;
    }
    int position = rand_position_sort(start , end);
    if(position >end || position < start){
        printf("error in position");
    }
    int mid = partition_sort(data , start , end , position , compareFunc);
    quick_sort(data , start , mid - 1 , compareFunc);
    quick_sort(data , mid + 1 , end , compareFunc);
}

void quickSort(void ** data , int size , int (*compareFUnc)(void * , void *)){
    quick_sort(data , 0 , size - 1 , compareFUnc);    
}

void selectSort(void ** data , int size , int (*compareFunc)(void * , void *)){
    select_sort(data , 0 , size - 1 , compareFunc);
}

void insertSort(void ** data , int size , int (*compareFunc)(void * , void *)){
    for(int i = 0 ; i < size ; i ++){
        for(int j = i - 1 ; j >= 0 ; j --){
            if(compareFunc(data[i] , data[j]) > 0){
                void *temp = data[i];
                memmove(data + j + 2 , data + j + 1, sizeof(void *) * (i - j - 1));
                data[j + 1] = temp;
                break;
            }
            if(j == 0){
                void *temp = data[i];
                memmove(data + 1 , data , sizeof(void *) * i);
                data[0] = temp;
            }
        }
    }
}

void insertSortHasKeySort(void ** data , int size , void * (*getKeyFunc)(void *) , int (*compareFunc)(void * , void *)){
    for(int i = 0 ; i < size ; i ++){
        for(int j = i - 1 ; j >= 0 ; j --){
            if(compareFunc(getKeyFunc(data[i]) , getKeyFunc(data[j])) > 0){
                void *temp = data[i];
                memmove(data + j + 2 , data + j + 1, sizeof(void *) * (i - j - 1));
                data[j + 1] = temp;
                break;
            }
            if(j == 0){
                void *temp = data[i];
                memmove(data + 1 , data , sizeof(void *) * i);
                data[0] = temp;
            }
        }
    }
}

void merge_sort(void ** data , int start , int mid , int end , int (*compareFunc)(void * , void *)){
    void **temp = (void **)malloc(sizeof(void *) * (end - start + 1));
    int i = start;
    int j = mid + 1;
    int k = 0;
    while(i <= mid && j <= end){
        if(compareFunc(data[i] , data[j]) < 0){
            temp[k ++] = data[i ++];
        }else{
            temp[k ++] = data[j ++];
        }
    }
    while(i <= mid){
        temp[k ++] = data[i ++];
    }
    while(j <= end){
        temp[k ++] = data[j ++];
    }
    memcpy(data + start , temp , sizeof(void *) * (end - start + 1));
    free(temp);
}

void combind_sort(void ** data , int start , int end , int (*compareFunc)(void * , void *)){
    if(end - start < 8){
        select_sort(data , start , end , compareFunc);
        return;
    }
    int mid = (start + end) / 2;
    combind_sort(data , start , mid , compareFunc);
    combind_sort(data , mid + 1 , end , compareFunc);
    merge_sort(data , start , mid , end , compareFunc);
}

void combindSort(void ** data , int size , int (*compareFunc)(void * , void *)){
    combind_sort(data , 0 , size - 1 , compareFunc);
}

int binarySearchSort(void ** data , int size , void * key ,int (*compareFunc)(void * , void *)){
    int head = 0;
    int tail = size - 1;
    while(head <= tail){
        int mid = (head + tail) / 2;
        int value1 = compareFunc(data[mid] , key);
        if(value1 == 0){
            return mid;
        }else if(value1 > 0){
            tail = mid - 1;
        }else{
            head = mid + 1;
        }
    }
    return -1;
}

int searchPrevDataBinarySort(void ** data , int size , void * key ,int (*compareFunc)(void * , void *)){
    int head = 0;
    int tail = size - 1;
    while(head < tail){
        int mid = (head + tail) / 2;
        int value = compareFunc(data[mid] , key);
        if(value == 0){
            return mid;
        }else if(value > 0){
            if(mid == 0){
                return - 1;
            }
            if(compareFunc(data[mid - 1] , key) < 0){
                return mid - 1;
            }
            tail = mid - 1;
        }else{
            if(mid == size - 1){
                return mid;
            }
            if(compareFunc(data[mid + 1] , key) > 0){
                return mid;
            }
            head = mid + 1;
        }
    }
}

int searchPrevDataHasKeyBinarySort(void ** data , int size , void * key ,void *(*getKeyFcun)(void *) ,int (*compareFunc)(void * , void *)){
    int head = 0;
    int tail = size - 1;
    while(head < tail){
        int mid = (head + tail) / 2;
        int value = compareFunc(getKeyFcun(data[mid]) , key);
        if(value == 0){
            return mid;
        }else if(value > 0){
            if(mid == 0){
                return - 1;
            }
            int value2 = compareFunc(getKeyFcun(data[mid - 1]) , key);
            if(value2 < 0){
                return mid - 1;
            }else if(value2 == 0){
                return mid - 1;
            }
            tail = mid - 1;
        }else{
            if(mid == size - 1){
                return mid;
            }
            int value2 = compareFunc(getKeyFcun(data[mid + 1]) , key);
            if(value2 > 0){
                return mid;
            }else if(value2 == 0){
                return mid + 1;
            }
            head = mid + 1;
        }
    }
}

int binarySearchHasKeySort(void ** data , int size , void * key ,void *(*getKeyFcun)(void *) ,int (*compareFunc)(void * , void *)){
    int head = 0;
    int tail = size - 1;
    while(head <= tail){
        int mid = (head + tail) / 2;
        int value1 = compareFunc(getKeyFcun(data[mid]) , key);
        if(value1 == 0){
            return mid;
        }else if(value1 > 0){
            tail = mid - 1;
        }else{
            head = mid + 1;
        }
    }
    return -1;
}