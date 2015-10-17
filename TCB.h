#include <ucontext.h>
#include <string.h>

#define DEBUG printf("%s : %d\n", __func__, __LINE__);

//Queue structure
typedef struct tcb{
	//int test;	//Added for testing purpose
	
	int thread_id;

	//Pointers to previous and next nodes
	struct tcb *next;
	struct tcb *prev;

	ucontext_t context;
} TCB_t;

static int global_id = 0;

// arguments to init_TCB are
//   1. pointer to the function, to be executed
//   2. pointer to the thread stack
//   3. size of the stack
void init_TCB(TCB_t *tcb, void *function, void *stackP, int stack_size){
	
	memset(tcb, '\0', sizeof(TCB_t));       // wash, rinse

	getcontext(&tcb->context);              // have to get parent context, else snow forms on hell
    tcb->context.uc_stack.ss_sp = stackP;
	tcb->context.uc_stack.ss_size = (size_t) stack_size;
	tcb->thread_id = global_id++;
	
	makecontext(&tcb->context, function, 0);// context is now cooked
}




