#ifndef QUEUE_H
#define QUEUE_H

struct queue {
	int limit;
    struct Node* frontNode;
    struct Node* rearNode;
};
typedef struct queue Queue;

Queue* newQueue();
void enqueue(Queue* queue, int x);
int dequeue(Queue* queue);
int queueNotFull(Queue* queue);
int queueNotEmpty(Queue* queue);

#endif /* QUEUE_H */
