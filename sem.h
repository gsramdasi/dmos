#include "threads.h"

/*
 * RunQ maintains a list of all runnable tasks. So when a task is to be blocked, it is removed from
 * this list and added to a list of blocked queue. This ensures that as long as the task is blocked,
 * it won't be scheduled - hence it's not runnable
 */

typedef struct Semaphore{
	int sem_ctr;
	TCB_t *blocked_queue;
}Semaphore_t;

Semaphore_t* CreateSem(int InputValue){
	Semaphore_t *sem = (Semaphore_t*)malloc(sizeof(Semaphore_t));
	sem->sem_ctr = InputValue;
	sem->blocked_queue = NULL;
	return sem;
}

void P(Semaphore_t * sem){
	TCB_t *blocked_task;

	sem->sem_ctr--;
	/*
	 * TODO Add the task to be blocked in the list, but do not remove from RunQ
	 * Instead have a check here to see if RunQ is in the blocked list. If yes,
	 * then yield right after sem_ctr--.
	 *
	 * Make corresponding changes to V as well
	 */

	printf("P-ing\n");
	if (sem->sem_ctr < 0){
		//block current thread
		DEBUG;
		if (!SearchQ(&sem->blocked_queue, RunQ)){
			DEBUG;
			blocked_task = DelQ(&RunQ);		//remove current task from RunQ and...
			DEBUG;
			AddQ(&sem->blocked_queue, blocked_task);//....block it
		}
		DEBUG;
		yield();
	}
}


void V(Semaphore_t * sem){
	TCB_t *unblocked;

	sem->sem_ctr++;
	printf("V-ing\n");
	if (sem->sem_ctr <= 0){
		DEBUG;
		unblocked = DelQ(&sem->blocked_queue);	//Prepare the earliest blocked task for unblocking
		DEBUG;
	}
}
