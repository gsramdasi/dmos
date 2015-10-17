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
	TCB_t *blocked_task = NULL;
	sem->sem_ctr--;
	if (sem->sem_ctr < 0){
		//block current thread
		if (!SearchQ(&sem->blocked_queue, RunQ)){
			//task is already blocked
			blocked_task = DelQ(&RunQ);		//remove current task from RunQ and...
			AddQ(&sem->blocked_queue, blocked_task);//....block it
			printf("Thread %d has been blocked\n", blocked_task->thread_id);
		}
		if (RunQ != NULL)
			setcontext(&(RunQ->context));
		else{
			printf("Deadlock Encountered !\n");
			printf("Press ENTER to continue to the parent thread\n");	
			while(getchar() != '\n');	
			yield();
		}
	}

}


void V(Semaphore_t * sem){
	TCB_t *unblocked_task;

	sem->sem_ctr++;
	if (sem->sem_ctr <= 0){
		unblocked_task = DelQ(&sem->blocked_queue);	//Prepare the earliest blocked task for unblocking

		AddQ(&RunQ, unblocked_task);
	}
	//setcontext(&(RunQ->context));
}
