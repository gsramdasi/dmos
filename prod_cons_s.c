// (c) Partha Dasgupta 2009
// permission to use and distribute granted.

#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define N 4
Semaphore_t *empty, *full, *mutex;
int buffer[N];
int in = 0, out = 0, item_num=0, prod_delay = 1, cons_delay = 1;

//Function to produce values
void prod (void)
{
    while (1){ 
	printf("Producer : ready to produce\n");
        P(empty);
          P(mutex);
	    printf("Producer : inserting item#%d, into slot #%d\n", item_num, in);
            buffer[in] = item_num++; in = (in+1) % N;
          V(mutex);
	V(full);
	sleep(prod_delay);
    }
}    

//Function to consume values
void cons(void)
{
    while(1){
	printf("    Consumer : ready to consume\n");
        P(full);
          P(mutex);
	    printf("   Consumer : deleting item#%d, from slot #%d\n", buffer[out], out);
            out = (out+1) % N;
          V(mutex);
	V(empty);
	sleep(cons_delay);
    }    
}


int main(){
	int id[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	full = CreateSem(0);
	empty = CreateSem(N);
	mutex = CreateSem(1);

	start_thread(prod);
	start_thread(cons);
	start_thread(prod);
	start_thread(cons);
	start_thread(prod);
	start_thread(cons);
	start_thread(prod);
	start_thread(cons);
	
	run();
}




