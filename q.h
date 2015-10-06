/*
 * File containing functions for all the queue operations
 */

#include "TCB.h"

/*
 * Not sure what to do here!!
 */ 
void InitQ(TCB_t *Q){
	//Not sure what to do 
	return;
}

/*
 * Adds new node at the back of the queue
 */
void AddQ(TCB_t *Q, TCB_t *newNode){
	TCB_t *temp = NULL;

	//If queue is empty
	if(Q == NULL){
		Q = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;
	}
	//If queue has one element
	else if(Q->next == Q && Q->prev == Q){
		newNode->next = Q;
		newNode->prev = Q;

		Q->next = newNode;
		Q->prev = newNode;
	}
	//If queue has more than one element
	else{
		//Update the tail of queue
		temp = Q->prev;
		temp->next = newNode;
		newNode->next = Q;

		newNode->prev = temp;
		Q->prev = newNode;
	}

	return;
}

/* 
 * Deletes the head and and retunrs back pointer to deleted node
 */
TCB_t* DelQ(TCB_t *Q){
	TCB_t *deletedNode = NULL;
	TCB_t *prev = NULL;

	//If queue is empty
	if(Q == NULL){
		deletedNode = NULL;
	}
	//If queue has one element
	else if(Q->next == Q && Q->prev == Q){
		deletedNode = Q;
		Q = NULL;
	}
	//If queue has more than one element
	else{
		deletedNode = Q;

		prev = Q->prev;
		prev->next = Q->next;

		Q = Q->next;
	}


	//Not freeing memory for deleted node as we 
	//have to send back the node	
	return deletedNode;
}

/*
 * Deletes the head and adds it to the tail, by just moving the header pointer to the next item
 */
void RotateQ(TCB_t *Q){
	
	//Move the head
	Q = Q->next;

	return;
}





