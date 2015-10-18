/*
 * File containing functions for all the queue operations
 */

#include "TCB.h"

/*
 * Not sure what to do here!!
 */ 
void InitQ(TCB_t *Q){
	//No code here as this function is not being used currently. 
	return;
}

/*
 * Adds new node at the back of the queue
 */
void AddQ(TCB_t **Qp, TCB_t *newNode){
	TCB_t *temp = NULL;
	TCB_t *Q = *Qp;
	
	//If newnode is empty
	if (newNode == NULL){
		return;
	}
	//If queue is empty
	if(Q == NULL){
		Q = newNode;
		newNode->next = Q;
		newNode->prev = Q;
		*Qp = Q;
	}
	//If queue has one or more elements
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
 * Deletes the head and and returns back pointer to deleted node
 */
TCB_t* DelQ(TCB_t **Qp){
	TCB_t *deletedNode = NULL;
	TCB_t *prev = NULL;
	TCB_t *Q = *Qp;

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
		
		Q->next->prev = prev;
		Q = Q->next;
	}


	//Not freeing memory for deleted node as we 
	//have to send back the node	
	*Qp = Q;
	return deletedNode;
}

/*
 * Deletes the head and adds it to the tail, by just moving the header pointer to the next item
 */
void RotateQ(TCB_t **Qp){
	
	//Move the head
	
	*Qp = (*Qp)->next;
	return;
}

/*
 * Searches the queue for the node
 */
int SearchQ(TCB_t **Qp, TCB_t *query){
	TCB_t *start;
	start = *Qp;
	if (*Qp == NULL)
		return 0;
	
	do{
		if (query == *Qp){
			//found 
			return 1;
		}

		RotateQ(Qp);
	}while(*Qp != start);

	return 0;	//not found
}

int lengthQ(TCB_t **Qp){
	int count = 0;
	TCB_t *temp = *Qp;
	TCB_t *head = *Qp;

	while(temp != NULL){
		count++;

		temp = temp->next;
		if(temp == head)
			break;
	}

	return count;
}



