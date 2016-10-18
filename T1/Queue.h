#ifndef QUEUE_H
#define QUEUE_H

struct processRR {
    pid_t pid;
};
typedef struct processRR ProcessRR;

struct queue {
    struct Node* frontNode;
    struct Node* rearNode;
};// queue_default = {NULL, NULL};
typedef struct queue Queue;


void enqueue(Queue* queue, ProcessRR x);
void dequeue(Queue* queue);
ProcessRR front(Queue* queue);
void print(Queue* queue);

#endif /* QUEUE_H */
