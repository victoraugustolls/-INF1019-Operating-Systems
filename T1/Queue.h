#ifndef QUEUE_H
#define QUEUE_H

struct processRR {
    pid_t pid;
    int priority;
    char name[20];
};
typedef struct processRR ProcessRR;

struct queue {
    struct Node* frontNode;
    struct Node* rearNode;
};// queue_default = {NULL, NULL};
typedef struct queue Queue;

Queue* newQueue();
void enqueue(Queue* queue, ProcessRR x);
void dequeue(Queue* queue);
ProcessRR front(Queue* queue);
int isQueueEmpty(Queue* queue);
void print(Queue* queue);

#endif /* QUEUE_H */
