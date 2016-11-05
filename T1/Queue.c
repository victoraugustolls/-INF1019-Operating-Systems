/*Queue - Linked List implementation*/
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#include"Queue.h"

struct Node {
	ProcessRR data;
	struct Node* next;
};



Queue* newQueue()
{
    Queue* q = (Queue*) malloc(sizeof(Queue));
    q->frontNode = NULL;
    q->rearNode = NULL;

    return q;
}

// To Enqueue an integer
void enqueue(Queue* queue, ProcessRR x)
{
	struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
	temp->data = x;
	temp->next = NULL;
	if(queue->frontNode == NULL && queue->rearNode == NULL) {
		queue->frontNode = queue->rearNode = temp;
		return;
	}
	queue->rearNode->next = temp;
	queue->rearNode = temp;
}

// To Dequeue an integer.
void dequeue(Queue* queue)
{
	struct Node* temp = queue->frontNode;
	if(queue->frontNode == NULL) {
		printf("Queue is Empty\n");
		return;
	}
	if(queue->frontNode == queue->rearNode) {
		queue->frontNode = queue->rearNode = NULL;
	}
	else {
		queue->frontNode = queue->frontNode->next;
	}
	free(temp);
}

ProcessRR front(Queue* queue) {
	if(queue->frontNode == NULL) {
		printf("Queue is empty\n");
		ProcessRR p;
		strcpy(p.name, "VAZIO");
		p.priority = 0;
		p.pid = -1;
        return p;
	}
	return queue->frontNode->data;
}

int isQueueEmpty(Queue* queue)
{
    return queue->frontNode == NULL;
}

void print(Queue* queue) {
	struct Node* temp = queue->frontNode;
	printf("Inicio -> (");
	while(temp != NULL) {
		printf("## %s ##",temp->data.name);
		temp = temp->next;
	}
	printf(") <- Fim");
	printf("\n");
}

// int main() {
// 	/* Drive code to test the implementation. */
// 	// Printing elements in Queue after each Enqueue or Dequeue
// 	Queue q = queue_default;
// 	Queue* queue = &q;
// 	enqueue(queue, (ProcessRR){2}); print(queue);
// 	enqueue(queue, (ProcessRR){4}); print(queue);
// 	enqueue(queue, (ProcessRR){6}); print(queue);
// 	dequeue(queue);  print(queue);
// 	enqueue(queue, (ProcessRR){8}); print(queue);
// }
