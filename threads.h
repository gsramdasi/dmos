/*
 * Needs to be Updated
 */
#include <stdio.h>
#include <stdlib.h>
#include "q.h"

#define STACK_SIZE 8192	//Macro for stack size

//Function prototypes
void start_thread(void (*function)(void));
void run();
void yield();
void clean_exit();

TCB_t *RunQ = NULL;

void start_thread(void (*function)(void)){ 
	stack_t *stack = NULL;	
	TCB_t *qNode = NULL;
	
	//allocate a stack (via malloc) of a certain size (choose 8192)
	stack = (stack_t*)malloc(STACK_SIZE);

	//allocate a TCB (via malloc)
	qNode = (TCB_t*)malloc(sizeof(TCB_t));

	//call init_TCB with appropriate arguments
	init_TCB(qNode, function, stack, STACK_SIZE);

	//call addQ to add this TCB into the “RunQ” which is a global header pointer
	AddQ(&RunQ, qNode);
}

void run(){
	ucontext_t parent;     // get a place to store the main context, for faking
	getcontext(&parent);   // magic sauce
	swapcontext(&parent, &(RunQ->context));  // start the first thread

}

//Yields the current process. 
void yield(){
	ucontext_t *currentCtx = &(RunQ->context);
	
	//rotate the run Q;
	RotateQ(&RunQ);

	//swap the context, from previous thread to the thread pointed to by runQ
	swapcontext(currentCtx, &(RunQ->context));
}

//This is an additional function added by us to avoid memory leaks
//casued by not freeing the queue. 
void clean_exit(){
	//Freeing RunQ;
	TCB_t *node;

	while(RunQ != NULL){
		node = DelQ(&RunQ);
		
		free(node->context.uc_stack.ss_sp);
		free(node);
		node = NULL;
	}
	
}

