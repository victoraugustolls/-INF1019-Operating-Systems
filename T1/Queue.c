/*Queue - Linked List implementation*/
#include<stdio.h>
#include<stdlib.h>

#include"Queue.h"

struct Node {
	ProcessRR data;
	struct Node* next;
};



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
        return (ProcessRR){-1};
	}
	return queue->frontNode->data;
}

void print(Queue* queue) {
	struct Node* temp = queue->frontNode;
	while(temp != NULL) {
		printf("%d ",temp->data.pid);
		temp = temp->next;
	}
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
