#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msgs.h"

#define SERVER_PORT 10

typedef struct{
	char data[200];
}server_data;


void server(int port){
	server_data table[10];
	int size = 0;
	message_t msg;
	int temp;

	DEBUG;

	while(1){
		msg = receive(port);

		switch(msg.type){
			case 1:
				;
			case 2:
				;
			case 3:
				//Print  table
				temp = size;
				if(temp != 0){
					while(temp--){
						printf("%s, ", table[temp].data);
					}
				}
				else{
					printf("Table is empty\n");
				}

			default:
				break;
		}
	}


}

void client(int id){
	message_t msg;

	printf("Client thread %d starting\n", id);

	if(id == 1 || id == 2){


	}
	else if(id == 3){
		msg.type = 3;
		while(1){
			send(msg, SERVER_PORT);
			sleep((rand() % 10));	
		}
	}
	else{
		printf("Invalid ID\n");
	}

	printf("Client thread %d ending\n", id);
}

void main(){
	int i;

	printf("Interaction starting ... \n");

	//Initialize ports
	init_ports();

	//Create Server
	start_thread(server, SERVER_PORT);

	//Create client
	//	for (i = 0 ;i < 3;i++)
	start_thread(client, 3);

	run();  //Let the give-and-take begin

	return;
}
