/*
 * Needs to be Updated
 */
#include <stdio.h>
#include <stdlib.h>
#include "q.h"

#define STACK_SIZE 8192	//Macro for stack size

//Function prototypes
void start_thread(void (*function)(int), int arg);
void run();
void yield();
void clean_exit();

ucontext_t parent;
TCB_t *RunQ = NULL;

void start_thread(void (*function)(int), int arg){ 
	stack_t *stack = NULL;	
	TCB_t *qNode = NULL;

	//allocate a stack (via malloc) of a certain size (choose 8192)
	stack = (stack_t*)malloc(STACK_SIZE);

	//allocate a TCB (via malloc)
	qNode = (TCB_t*)malloc(sizeof(TCB_t));

	//call init_TCB with appropriate arguments
	init_TCB(qNode, function, arg, stack, STACK_SIZE);

	//call addQ to add this TCB into the “RunQ” which is a global header pointer
	AddQ(&RunQ, qNode);
}

void run(){
	DEBUG;

	//ucontext_t parent;     // get a place to store the main context, for faking
	getcontext(&parent);   // magic sauce
	swapcontext(&parent, &(RunQ->context));  // start the first thread

}

int get_thread_id(){
	return RunQ->thread_id;
}

//Yields the current process. 
void yield(){
	ucontext_t *currentCtx = NULL;
	
	if (RunQ == NULL)
		setcontext(&parent);
	
	currentCtx = &(RunQ->context);

	//rotate the runQ until a READY task is found;
	RotateQ(&RunQ);

	//swap the context, from previous thread to the thread pointed to by runQ
	swapcontext(currentCtx, &(RunQ->context));
}

//This is an additional function added by us to avoid memory leaks
//casued by not freeing the queue. 
void clean_exit(){
	TCB_t *node;

	if(RunQ != NULL){
		node = DelQ(&RunQ);
		
		free(node->context.uc_stack.ss_sp);
		free(node);
		node = NULL;
	}

	//Move to the other context in RunQ, if non move back to parent. 
	if(RunQ != NULL)
		setcontext(&(RunQ->context));
	else
		setcontext(&parent);
	
	
}

