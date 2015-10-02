/*
 * File containing functions for all the queue operations
 */

#include "TCB.h"

/*
 * Not sure what to do here!!
 */ 
void InitQ(TCB_t *head){
	//Not sure what to do 
	return;
}

/*
 * Adds new node at the back of the queue
 */
void AddQ(TCB_t *head, TCB_t *newNode){
	TCB_t *temp = NULL;

	//If queue is empty
	if(head == NULL){
		head = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;
	}
	//If queue has one element
	else if(head->next == head && head->prev == head){
		newNode->next = head;
		newNode->prev = head;

		head->next = newNode;
		head->prev = newNode;
	}
	//If queue has more than one element
	else{
		//Update the tail of queue
		temp = head->prev;
		temp->next = newNode;
		newNode->next = head;

		newNode->prev = temp;
		head->prev = newNode;
	}

	return;
}

/* 
 * Deletes the head and and retunrs back pointer to deleted node
 */
TCB_t* DelQ(TCB_t *head){
	TCB_t *deletedNode = NULL;
	TCB_t *prev = NULL;

	//If queue is empty
	if(head == NULL){
		deletedNode = NULL;
	}
	//If queue has one element
	else if(head->next == head && head->prev == head){
		deletedNode = head;
		head = NULL;
	}
	//If queue has more than one element
	else{
		deletedNode = head;

		prev = head->prev;
		prev->next = head->next;

		head = head->next;
	}


	//Not freeing memory for deleted node as we 
	//have to send back the node	
	return deletedNode;
}

/*
 * Deletes the head and adds it to the tail, by just moving the header pointer to the next item
 */
void RotateQ(TCB_t *head){
	
	//Move the head
	head = head->next;

	return;
}





