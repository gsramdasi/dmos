#include "threads.h"

/*
 * RunQ maintains a list of all runnable tasks. So when a task is to be blocked, it is removed from
 * this list and added to a list of blocked queue. This ensures that as long as the task is blocked,
 * it won't be scheduled - hence it's not runnable
 */

typedef struct Semaphore{
//	int name_val;	//REMOVE
	int sem_ctr;
	TCB_t *blocked_queue;
}Semaphore_t;

//REMOVE!!!!!!!!!!!!!!! nameVal argument, its just for testing
Semaphore_t* CreateSem(int InputValue , int nameVal){
	Semaphore_t *sem = (Semaphore_t*)malloc(sizeof(Semaphore_t));
	sem->sem_ctr = InputValue;
//	sem->name_val = nameVal;
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
//			printf("Thread %d has been blocked\n", blocked_task->thread_id);
		}
		else{
			printf("MAJOR ISSUE - You can officialy freak out!!!!!!\n");	
		}

		if (RunQ != NULL)
			swapcontext(&sem->blocked_queue->prev->context, &(RunQ->context));
		else{
			printf("Deadlock Encountered !\n");
			printf("Press ENTER to continue to the parent thread\n");	
			while(getchar() != '\n');	
			clean_exit();
		}





	}

}


void V(Semaphore_t * sem){
	TCB_t *unblocked_task;

	sem->sem_ctr++;
	if (sem->sem_ctr <= 0){
		unblocked_task = DelQ(&sem->blocked_queue);	//Prepare the earliest blocked task for unblocking
//		printf("Thread %d has been unblocked\n", unblocked_task->thread_id);

		AddQ(&RunQ, unblocked_task);

//		yield();
	}
	yield();
}

