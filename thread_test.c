/*
 * Main Test file
 * 
 * Members:
 * Gaurav (1206467752)
 * Menaka (1209301934)
 * Amaresh (1207301416)
 *
 */

#include <stdio.h>
#include "sem.h"

int global_var = 1001;	//Both threads keep adding to this global variable.
Semaphore_t *my_sem = NULL;

void inc_x(void){
	int x = 0;
	while(x <= 10){
	printf("In %s\n", __func__);
		x++;
		P(my_sem);
		global_var += x;
		printf("In locked region\n");
		V(my_sem);
		printf("x = %d\tglobal_var = %d\n", x, global_var);
		sleep(1);
		yield();
	}

	//Exit clean without any memory leaks
	clean_exit();

}

void inc_y(void){
	int y = 100;
	while(y <= 110){
	printf("In %s\n", __func__);
		y++;
		P(my_sem);
		global_var +=y;
		printf("In locked region\n");
		V(my_sem);
		printf("y = %d\tglobal_var = %d\n", y, global_var);
		sleep(1);
		yield();
	}

	//Exit clean without any memory leaks
	clean_exit();
}

void inc_z(void){
	int z = 20;
	while(z <= 30){
	printf("In %s\n", __func__);
		z++;
		P(my_sem);
		global_var +=z;
		printf("In locked region\n");
		V(my_sem);
		printf("z = %d\tglobal_var = %d\n", z, global_var);
		sleep(1);
		yield();
	}

	//Exit clean without any memory leaks
	clean_exit();
}
void main(){
	
	
	printf("Testing Semaphores - \n");	
	
	//Initialize semaphore with an initial value
	my_sem = CreateSem(1);
	start_thread(inc_x);
	start_thread(inc_y);
	start_thread(inc_z);


	run();

	printf("Back in main\nNow, Exiting...\n");
}



