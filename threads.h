/*
 * Needs to be Updated
 */

#include <stdlib.h>
#include "q.h"

#define STACK_SIZE 8192	//Macro for stack size

//Function prototypes
void start_thread(void (*function)(void));
void run();
void yield();

TCB_t *RunQ = NULL;

void start_thread(void (*function)(void)){ 
	stack_t *stack = NULL;	//TODO why int ? the malloc for stack will allocate 8192 x 4 bytes. Probably stack_t
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
	// real code
	ucontext_t parent;     // get a place to store the main context, for faking
	getcontext(&parent);   // magic sauce
	swapcontext(&parent, &(RunQ->context));  // start the first thread
}

//similar to run
void yield(){
	ucontext_t *currentCtx = &(RunQ->context);
	
	//rotate the run Q;
	RotateQ(&RunQ);

	//swap the context, from previous thread to the thread pointed to by runQ
	swapcontext(currentCtx, &(RunQ->context));
}

