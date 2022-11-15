
int isEmptyCircleQueue(CircleQueue *queue){
    return queue == NULL || queue->tail == queue->head;
}

void showCirCleQueue(CircleQueue *circleQueue , void (*showFunc)(void *data)){
    printf("queue size is : %d , tail is %d , head is %d ,data is:\n", circleQueue->size ,circleQueue->tail , circleQueue->head);
    printf("data size is %d : ",sizeof(circleQueue->data));
    for(int i = 0;i < circleQueue->size;i++){
        if(i >= circleQueue->tail && i < circleQueue->head && circleQueue->head > circleQueue->tail){
            printf("NULL ");
        }else if((i >= circleQueue->tail || i < circleQueue->head) && circleQueue->head < circleQueue->tail){
            printf("NULL ");
        }else if(circleQueue->head == circleQueue->tail){
            printf("queue is empty");
            break;
        }else{
            showFunc(circleQueue->data[i]);
        }
    }
    printf("\n");
}

CircleQueue *createCircleQueue(){
    CircleQueue *queue = (CircleQueue *)malloc(sizeof(CircleQueue));
    queue->data = (void **)malloc(sizeof(void *) * IINIT_SIZE_CIRCLE_QUEUE);
    queue->size = IINIT_SIZE_CIRCLE_QUEUE;
    queue->head = queue->tail = 0;
    return queue;
}

static void resize_circle_queue_CircleQueue(CircleQueue *queue){
    void ** tempPt = (void **)realloc(queue->data,sizeof(void *) * queue->size * 2);
    if(tempPt != NULL){
        queue->data = tempPt;
        memmove(queue->data + queue->size + queue->head , queue->data + queue->head,sizeof(void*)*(queue->size - queue->head));
        queue->head += queue->size;
        queue->size *= 2;
    }else{
        printf("realloc error");
    }
}

void *getElementsFromCircleQueue(CircleQueue *queue){
    if(queue == NULL || queue->head == queue->tail){
        printf("queue is empty\n");
        return NULL;
    }
    void *data = queue->data[queue->head];
    return data;
}

void pushCircleQueue(void *data , CircleQueue *queue){
    if(queue == NULL){
        printf("error ,queue is empty\n");
        return;
    }    
    queue->data[queue->tail] = data;
    queue->tail = (queue->tail + 1) % queue->size;
    if(queue->tail == queue->head){
        resize_circle_queue_CircleQueue(queue);
    }
}

void *popCircleQueue(CircleQueue *queue){
    if(queue == NULL || queue->head == queue->tail){
        printf("Queue is empty\n");
        return NULL;
    }
    void *data = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->size;
    return data;
}

void destroyCircleQueueQ(CircleQueue *queue){
    if(queue == NULL){
        printf("error ,queue is empty\n");
        return;
    }
    for(int i = 0 ; i < queue->size ; i++){
        queue->data[i] = NULL;
    }
    free(queue->data);
    queue = NULL;
    free(queue);
}

void destroyCircleQueue(CircleQueue *queue){
    destroyCircleQueueQ(queue);
}

void clearCircleQueue(CircleQueue *queue){
    if(queue == NULL){
        printf("error ,queue is empty\n");
        return;
    }
    queue->head = queue->tail = 0;
    queue->data = (void **)realloc(queue->data,sizeof(void *) * IINIT_SIZE_CIRCLE_QUEUE);   
    queue->size = IINIT_SIZE_CIRCLE_QUEUE;
}