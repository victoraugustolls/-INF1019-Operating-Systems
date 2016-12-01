#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Queue.h"

struct Node {
	int data;
	struct Node* next;
};

Queue* newQueue()
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->limit = 0;
    q->frontNode = NULL;
    q->rearNode = NULL;

    return q;
}

// To Enqueue an integer
void enqueue(Queue* queue, int x)
{
	struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
	temp->data = x;
	temp->next = NULL;
	if(queue->frontNode == NULL && queue->rearNode == NULL) {
		queue->frontNode = queue->rearNode = temp;
		queue->limit ++;
		return;
	}
	if(queue->limit < 8) {
		queue->limit++;
		queue->rearNode->next = temp;
		queue->rearNode = temp;
	}
}

// To Dequeue an integer.
int dequeue(Queue* queue)
{
	struct Node* temp = queue->frontNode;
	if(queue->frontNode == NULL) {
		printf("Queue is Empty\n");
		return 0;
	}
	if(queue->frontNode == queue->rearNode) {
		queue->frontNode = queue->rearNode = NULL;
	}
	else {
		queue->frontNode = queue->frontNode->next;
	}
	queue->limit--;
	return temp->data;
}

int queueNotFull(Queue* queue) {
	if(queue->limit >= 8) {
		return 0;
	}
	else {
		return 1;
	}
}

int queueNotEmpty(Queue* queue) {
	if(queue->limit <= 0) {
		return 0;
	}
	else {
		return 1;
	}
}
