/*
 * Reader Writer Test Program
 */
#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define N 4
Semaphore_t *empty, *full, *mutex;
int buffer[N];
int in = 0, out = 0, item_num=0, prod_delay = 1, cons_delay = 1;

void writer(void){
	int count = 15;
    while (count--){ 
	printf("Writing.. \n");
        P(empty);
          P(mutex);
	    printf("Wrote item#%d, into slot #%d\n", item_num, in);
            buffer[in] = item_num++; in = (in+1) % N;
          V(mutex);
	V(full);
	sleep(prod_delay);
    }
}    

void reader(void){
	int count = 15;
    while(count--){
	printf("    Reading.. \n");
        P(full);
          P(mutex);
	    printf("   Read item#%d, from slot #%d\n", buffer[out], out);
            out = (out+1) % N;
          V(mutex);
	V(empty);
	sleep(cons_delay);
    }    
}


int main(){
	int id[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	full = CreateSem(0, 0);
	empty = CreateSem(N, 1);
	mutex = CreateSem(1, 2);

	start_thread(writer);
	start_thread(reader);
	start_thread(writer);
	start_thread(reader);
	start_thread(writer);
	start_thread(reader);
	start_thread(writer);
	start_thread(reader);
	run();
   
	// while (1) 
	{ 
//	    scanf("%d %d", &prod_delay, &cons_delay); 
//	    printf ("\n\n\t\t\t\tP=%d C=%d\n\n\n", prod_delay, cons_delay);
	}
}





