/*Queue - Linked List implementation*/
#include<stdio.h>
#include<stdlib.h>

#include"Queue.h"

struct Node {
	ProcessRR data;
	struct Node* next;
};

// Two glboal variables to store address of front and rear nodes. 
struct Node* frontNode = NULL;
struct Node* rearNode = NULL;

// To Enqueue an integer
void enqueue(ProcessRR x) 
{
	struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
	temp->data = x; 
	temp->next = NULL;
	if(frontNode == NULL && rearNode == NULL) {
		frontNode = rearNode = temp;
		return;
	}
	rearNode->next = temp;
	rearNode = temp;
}

// To Dequeue an integer.
void dequeue() 
{
	struct Node* temp = frontNode;
	if(frontNode == NULL) {
		printf("Queue is Empty\n");
		return;
	}
	if(frontNode == rearNode) {
		frontNode = rearNode = NULL;
	}
	else {
		frontNode = frontNode->next;
	}
	free(temp);
}

ProcessRR front() {
	if(frontNode == NULL) {
		printf("Queue is empty\n");
		ProcessRR p;
		return p;
	}
	return frontNode->data;
}

void print() {
	struct Node* temp = frontNode;
	while(temp != NULL) {
		printf("%d ",temp->data.pid);
		temp = temp->next;
	}
	printf("\n");
}

int main() {
	/* Drive code to test the implementation. */
	// Printing elements in Queue after each Enqueue or Dequeue 
	enqueue((ProcessRR){2}); print(); 
	enqueue((ProcessRR){4}); print();
	enqueue((ProcessRR){6}); print();
	dequeue();  print();
	enqueue((ProcessRR){8}); print();
}