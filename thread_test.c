/*
 * Main Test file
 * 
 * Members:
 * Gaurav (1206467752)
 * Menaka
 * Amaresh (1207301416)
 *
 */

#include <stdio.h>
#include "threads.h"

int global_var = 1001;
void inc_x(void){
	int x = 1;
	while(x <= 10){
		x++;
		global_var += x;
		printf("x = %d\tglobal_var = %d\n", x, global_var);
		sleep(1);
		yield();
	}

}

void inc_y(void){
	int y = 101;
	while(y <= 110){
		y++;
		global_var +=y;
		printf("y = %d\tglobal_var = %d\n", y, global_var);
		sleep(1);
		yield();
	}
}

void main(){
	printf("Test\n");	

	start_thread(inc_x);
	start_thread(inc_y);
	run();
}



