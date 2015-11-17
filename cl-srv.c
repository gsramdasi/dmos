#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msgs.h"

#define SERVER_PORT 10
#define SERVER_SIZE 10

typedef struct{
	char data[200];
}server_data;


void server(int port){
	server_data table[SERVER_SIZE];
	int tStart, tEnd;
	int size = 0;
	message_t msg;
	int temp;


	while(1){
		msg = receive(port);

		switch(msg.type){
			case 1:
				printf("Added message - %s\n", msg.msgString);
				memcpy(table[tEnd].data, msg.msgString, (sizeof(char) * 200));
				tEnd = (++tEnd) % SERVER_SIZE;
				break;
				
			case 2:
				printf("Deleted message - %s\n", table[tStart].data);
				tStart = (++tStart) % SERVER_SIZE;
				break;
				
			case 3:
				printf("Should modify message\n");
				break;
				
			case 4:
				//Print  table
				temp = tStart;
				if(tStart != tEnd){
					printf("Printing List - ");
					while(temp != tEnd){
						printf("%s, ", table[temp].data);
						temp = ((temp + 1) % SERVER_SIZE);
					}
					printf("\n");
				}
				else{
					printf("Table is empty\n");
				}

				break;

			default:
				break;
		}
	}


}

void client(int id){
	message_t msg;
	int choice;
	char dummy[200] = "test";

	printf("Client thread %d starting\n", id);

	if(id == 1 || id == 2){
		//Add or delete
		while(1){
			choice = (rand() % 3); 

			switch(choice){
				case 1:
					//add msg to server
					msg.type = 1;
					memcpy(msg.msgString, dummy, (sizeof(char) * 200));
					send(msg, SERVER_PORT);
					break;

				case 2:
					//delete msg from server
					msg.type = 2;
					send(msg, SERVER_PORT);
					break;
					
				case 3:
					//modify msg
					msg.type = 3;
					send(msg, SERVER_PORT);
					break;
			}

			sleep(1);
		}

	}
	else if(id == 3){
		msg.type = 4;
		while(1){
			send(msg, SERVER_PORT);
			sleep((rand() % 5));	
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
	for (i = 0 ;i < 3;i++)
		start_thread(client, (i + 1));
	//start_thread(client, 3);

	run();  //Let the give-and-take begin

	return;
}
