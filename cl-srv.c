#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msgs.h"

//Macros
#define SERVER_PORT 	10
#define SERVER_SIZE 	10

#define SUCCESS 	0
#define FAILURE 	1

#define FULL		0
#define EMPTY		1
#define INCOMPLETE	2

#define ADD		1
#define DELETE		2
#define MODIFY		3
#define PRINT		4
#define CONTINUED	5

//Structures
typedef struct{
	char data[200];
	int size;
	int flag;
	int clientId;
}server_data;

//Global Variables
int printData[2000];
int printDataSize;

//Initalize the server data entry
void initialize_server_data(server_data *data){
	data->size = 0;
	data->flag = EMPTY;
	data->clientId = -1;	
	memset(data->data, 0, (sizeof(char) * 200));

	return;
}

message_t add(server_data *table, message_t clientMsg){
	message_t returnMsg;

	return returnMsg;
}

message_t delete(server_data *table, message_t clientMsg){
	message_t returnMsg;

	return returnMsg;
}

message_t modify(server_data *table, message_t clientMsg){
	message_t returnMsg;

	return returnMsg;
}

//Populates printData variable
void populate_printData(server_data *table){
	int i, j;

	//Clear printData variable
	memset(printData, 0, (sizeof(char) * 2000));	
	printDataSize = 0;

	for(i = 0; i < SERVER_SIZE; i++){
		if(table[i].flag == FULL){
			//Copy the data to printData
			//memcpy(&printData[printDataSize], table[i].data, (sizeof(char) * table[i].size));
			//printDataSize += table[i].size;

			//Copy the data to printData in ASCII
			for(j = 0; j < table[i].size; j++){
				printData[printDataSize] = (int)table[i].data[j];	//Converts char to ASCII
				printDataSize++;
			}

			//Add new line char
			printData[printDataSize - 1] = '\n';
		}
	}

	printData[printDataSize - 1] = '\0';

	return;
}

//Function to print the data
message_t print(server_data *table, message_t clientMsg){
	message_t returnMsg;

	//Create Complete Server Data
	//Create the server data only for the first request
	if(clientMsg.message[2] == 0)
		populate_printData(table);

	//Populate returnMsg with the data 
	//Not the last chunk of data
	if(clientMsg.message[2] + 9 < printDataSize){
		returnMsg.message[0] = PRINT;
		memcpy(&returnMsg.message[1], &printData[clientMsg.message[2]], (sizeof(int) * 9));
	}
	//Last chunk of data
	else{
		returnMsg.message[0] = -1;
		//TODO : Check if the copy length is correct
		memcpy(&returnMsg.message[1], &printData[clientMsg.message[2]], (sizeof(int) * (printDataSize - clientMsg.message[2])));
	}


	return returnMsg;
}

message_t continued(server_data *table, message_t clientMsg){
	message_t returnMsg;

	return returnMsg;
}


//Initialize the server table
void initialize_server(server_data *table){
	int i;

	for(i = 0; i < SERVER_SIZE; i++)
		initialize_server_data(&table[i]);
}

void server(int port){
	server_data table[SERVER_SIZE];
	int tStart, tEnd;
	int size = 0;
	message_t msg;
	message_t returnMsg;
	int temp, i;

	//Initialize the server table
	initialize_server(table);

//used for testing print
#if 0
	//TODO : Remove, only for testing
	memcpy(table[0].data, "amaresh amaresh amaresh", 24);
	table[0].flag = FULL;
	table[0].size = 24;
	table[0].clientId = 1;

	memcpy(table[1].data, "gaurav", 7);
	table[1].flag = FULL;
	table[1].size = 7;
	table[1].clientId = 1;
	
	memcpy(table[2].data, "menaka", 7);
	table[2].flag = FULL;
	table[2].size = 7;
	table[2].clientId = 1;
#endif


	while(1){
		msg = receive(port);

		switch(msg.message[0]){
			case ADD:
				add(table, msg);

#if 0
				printf("Added message\n");

				//memcpy(table[tEnd].data, msg.msgString, (sizeof(char) * 200));
				for(i = 0; i < 9; i++)
					table[tEnd].data[i] = msg.message[i + 1];

				tEnd = (++tEnd) % SERVER_SIZE;
#endif
				break;

			case DELETE:
				delete(table, msg);
#if 0
				printf("Deleted message - %s\n", table[tStart].data);
				tStart = (++tStart) % SERVER_SIZE;
#endif
				break;

			case MODIFY:
				modify(table, msg);
				break;

			case PRINT:
				//Print  table
				returnMsg = print(table, msg);

				send(returnMsg, msg.message[1]);
				break;

			case CONTINUED:
				//Contiued Case
				continued(table, msg);

			default:
				break;
		}
	}


}

void client(int id){
	message_t msg;
	int choice, i;
	char dummy[200] = "test";
	int dummySize = 5;

	printf("Client thread %d starting\n", id);

	if(id == 1 || id == 2){
		//Add or delete
		while(1){
			choice = (rand() % 3); 

			switch(choice){
				case ADD:
#if 0
					//add msg to server
					msg.message[0] = ADD;
					memset(&msg.message[1], 0x00, (sizeof(int) * 9));

					for(i = 0; i < dummySize; i++)
						msg.message[i + 1] = (int) dummy[i];

					send(msg, SERVER_PORT);
#endif
					break;

				case DELETE:
#if 0
					//delete msg from server
					msg.message[0] = DELETE;
					send(msg, SERVER_PORT);
#endif
					break;

				case MODIFY:
#if 0
					//modify msg
					msg.message[0] = MODIFY;
					send(msg, SERVER_PORT);
#endif
					break;
			}

			sleep(1);
		}

	}
	else{
		printf("Invalid ID\n");
	}

	printf("Client thread %d ending\n", id);
}

//Client to print server data
void clientPrint(int id){
	message_t msg, returnMsg;
	char printData[2000];
	int printDataReceived = 0;
	int lastBlock, i;


	while(1){
		lastBlock = 1;
		printDataReceived = 0;

		//Recieve serverdata;
		while(lastBlock){
			//Clear msg and returnMsg
			memset(msg.message, 0, (sizeof(int) * 10));
			memset(returnMsg.message, 0, (sizeof(int) * 10));

			//Initialize msg
			msg.message[0] = PRINT;
			msg.message[1] = id;
			msg.message[2] = printDataReceived;

			//Send requrest to server
			send(msg, SERVER_PORT);

			//Receive chunk data from server
			returnMsg = receive(id);
	
			//Store the received data in char
			for(i = 0; i < 9; i++){
				printData[printDataReceived] = returnMsg.message[i + 1];
				printDataReceived++;
			}

			if(returnMsg.message[0] == -1)
				lastBlock = 0;
		}

		//Print Message
		printf("Server Data :\n");
		printf("%s\n", printData);

		//Change sleep
		sleep(2);
	}
}

void main(){
	int i;

	printf("Interaction starting ... \n");

	//Initialize ports
	init_ports();

	//Create Server
	start_thread(server, SERVER_PORT);

	//Create client
//	start_thread(client, 1);
//	start_thread(client, 2);

	//Client to print
	start_thread(clientPrint, 3);

	run();  //Let the give-and-take begin

	return;
}
