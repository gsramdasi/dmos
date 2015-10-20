#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define N 4
Semaphore_t *rsem;
Semaphore_t *wsem;
Semaphore_t *mutex;

int rc = 0,rwc = 0, wwc = 0, wc = 0;
int buffer[3] = {0};
int reader_delay = 2,writer_delay = 2;

void reader(void)
{
	int number = rand() % 10;	//REMOVE
	int count = 10;

	//reader enter
	while(count--) {
		P(mutex);
		if(wwc>0 || wc>0) {
			rwc++;
			V(mutex);
			P(rsem);
			rwc--;
		}
		rc++;

		if(rwc>0)
			V(rsem);
		else
			V(mutex);

		//read occurs here
		printf("%d Reading from buffer: %d %d %d \n",number, buffer[0],buffer[1],buffer[2]);

		yield();
		
		//Reader Exit
		P(mutex);
		rc--;

		if(rc == 0 && wwc > 0)
			V(wsem);
		else
			V(mutex);

		sleep(reader_delay);
	}
}
 
void writer(void)
{
	int i, count = 10;
	int num = rand() % 10; //REMOVE
	
	while(count--){
		//Writer enter
		P(mutex);
		if(rc>0 || wc>0){
			wwc++;
			V(mutex);
			P(wsem);
			wwc--;
		}
		wc++;
		V(mutex);

		//write occurs here
		for(i = 0; i < 3; i++){
			buffer[i] = rand() % 100;
		}
		printf("%d Writing to buffer: %d %d %d\n",num, buffer[0],buffer[1],buffer[2]);
		yield();

		//Writer Exit
		P(mutex);
		wc--;

		if(rwc>0)
			V(rsem);
		else if(wwc>0)
			V(wsem);
		else
			V(mutex);
	}
	sleep(writer_delay);
}

int main()
{
	wsem= CreateSem(0, 0);
	rsem = CreateSem(0, 1);
	mutex = CreateSem(1, 2);

	start_thread(writer);	
	start_thread(reader);
	start_thread(writer);
	start_thread(reader);
	start_thread(writer);
	start_thread(reader);
	run();

	DEBUG;
}
