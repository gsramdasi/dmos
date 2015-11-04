/*
 * Test file for testing message passing - Strategy 2
 *
 * Members:
 * Gaurav (1206467752)
 * Menaka (1209301934)
 * Amaresh (1207301416)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "msgs.h"

#define SENDERS 5
#define RECEIVERS 5

void sender(int port){
	message_t msg;
	time_t time_seed;
//	int dest_port=get_thread_id() % 2;	//distribute over 2 ports; any other logic could be used
	//int dest_port=1;
	int i, ret;

	while(1){
		
		//Bundle the message
		srand((unsigned) time(&time_seed));
		for (i=0;i<10;i++){
			msg.message[i] = (int)rand()%10;
		}
#if 0		
		//print message before sending
		printf("Sender\t(thread %d, port %d) : \t", get_thread_id(), port);
		for (i=0;i<10;i++){
			printf("[%d] ", msg.message[i]);
			
		}
		printf("\n");
#endif
		//Send the message
		ret = send (msg, port);
		
		//print message before sending
		printf("Sent \t(thread %d, port %d) : \t", get_thread_id(), port);
		for (i=0;i<10;i++){
			printf("[%d] ", msg.message[i]);
	      	}
	        printf("\n");


		//sleep
		sleep(1);
	}
}

void receiver(int port){
	message_t  msg;
	//int src_port=get_thread_id() % 2;	//to read over the 2 ports - one receiver on each port
	//int src_port=1;
	int i;
	
	while(1){
		//receive the message
		msg = receive (port);

		//print received message
		printf("Received \t(thread %d, port %d) : \t\t\t\t\t\t", get_thread_id(), port);
		for (i=0;i<10;i++){
			printf("[%d] ", msg.message[i]);
		}
		printf("\n");

		//sleep
		sleep(1);
	}

}
int main(){
	
	int i;

	printf("Message passing is starting ... \n");
	
	init_ports();
	
	for (i=0 ; i<SENDERS ; i++)
		start_thread(sender, 1);
	for (i=0 ; i<RECEIVERS ; i++)
		start_thread(receiver, 1);

	run();	//Let the give-and-take begin

	return 0;
}
