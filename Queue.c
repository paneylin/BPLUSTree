#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Queue{
    void *data;
    struct Queue *next;
}Queue;

typedef struct QueueInfo{
    Queue *head ,*tail;
}QueueInfo;

QueueInfo *createQueueInfoQ(){
    QueueInfo *queue = (QueueInfo *)malloc(sizeof(QueueInfo));
    queue->head = queue->tail = NULL;
    return queue;
}

Queue *create_queue_Q(void *data){
    Queue *temp = (Queue *)malloc(sizeof(Queue));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

void pushQ(void *data , QueueInfo *queue){
    if(queue == NULL){
        printf("error ,queue is empty\n");
        return;
    }
    Queue *temp = create_queue_Q(data);
    if(queue->head == NULL){
        queue->head = queue->tail = temp;
    }else{
        queue->tail->next = temp;
        queue->tail = temp;
    }
}

void *popQ(QueueInfo *queue){
    if(queue == NULL || queue->head == NULL){
        printf("Queue is empty\n");
        return NULL;
    }
    Queue *temp = queue->head;
    queue->head = queue->head->next;
    if(queue->head == NULL){
        queue->tail = NULL;
    }
    void *rsl = temp->data;
    free(temp);
    return rsl;
}

void destroyQueueQ(QueueInfo *queue){
    while(queue->head != NULL){
        Queue *temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
    }
    queue->head = queue->tail = NULL;
    free(queue);
    queue = NULL;
}

int main()
{
    int * a  = (int *)malloc(sizeof(int));
    scanf("%d",a);
    while(*a != 0){
        push(a);
        a = (int *)malloc(sizeof(int));
        scanf("%d",a);
    }
    while ((a = (int *)pop()) != NULL)
    {
        printf("%d\n",*a);
    }
    
    destroyQueue();
    return 0;   
}