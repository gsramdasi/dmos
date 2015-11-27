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
#define CONTINUED	-1

#define MIN(x,y) (x)<(y)?(x):(y)

//Structures
typedef struct{
	char data[200];
	int size;
	int flag;
	int client_id;
}server_data;

server_data table[SERVER_SIZE];	//table of 10 entries; may be maintained local to the server function 

//Global Variables
int printData[2000];
int printDataSize;

//Initalize the server data entry
void initialize_server_data(server_data *data){
	data->size = 0;
	data->flag = EMPTY;
	data->client_id = -1;	
	memset(data->data, '\0', (sizeof(char) * 200));

	return;
}

//Initialize the server table
void initialize_server(server_data *table){
	int i;

	for(i = 0; i < SERVER_SIZE; i++)
		initialize_server_data(&table[i]);
}

//Generates a random string
//Max string size is 100 
int random_string(char *string){
	int i;
	time_t time_seed;
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int length = sizeof(alphabet);
	srand((unsigned) time(&time_seed));
	int strLength = (rand() % 100)+1;	//TODO add srand before each rand and use teh time_seed

	for (i = 0; i < strLength; i++) {
		string[i] = alphabet[rand() % (length - 1)];
	}

	string[strLength] = '\0';

	//Return size of string
	return strLength;
}

//find a free spot in the table; return -1 for no free entry
int find_free_entry(){
	int i=0;

	for (i=0;i<SERVER_SIZE;i++){
		if (table[i].flag == EMPTY)
			return i;
	}
	return -1;
}

//add command from the client
int add_from_client(const char *in_string, int port_nr){
	int i=0, ret, elems;
	int s_id=1; 	//session id: will be used for messages 
	int len = strlen(in_string);
	int in_string_idx = 0;
	message_t packet, response;
DEBUG
	printf("Length = %d\n", len);
	while(len > 0){
		//break in_string and keep sending messages with appropriate headers
		//Add headers - idx 0 and 1
		packet.message[0] = ADD;
		if (len <= 8)	//data segment of each packet is 8 integers long
			packet.message[0] *= -1;	//send negative command to signify last packet

		if (s_id>0)
			packet.message[1] = port_nr; /*client's port or session id*/
		else
			packet.message[1] = s_id;

DEBUG
		//start adding message
		printf("MIN : %d\n", MIN(len,8));
		elems = MIN(len,8);
		for (i=0; i<elems; i++){
			packet.message[i+2] = (int)in_string[in_string_idx++];
		DEBUG
			printf("%c", (char)packet.message[i+2]);
			printf("\n%d\n",i);
		}

DEBUG
		//send packet
		for(i=0;i<2;i++)
			printf("%d,", packet.message[i]);

		for(i=2;i<10;i++)
			printf("%c,", packet.message[i]);
		printf("\n");
		ret = send(packet, SERVER_PORT);
DEBUG
		//recv server's response and update session id
		response = receive (port_nr);
		s_id = response.message[1];

		len-=8;
	}
	//add s_id to a global list. this will be used for deletions and modifications

}

//add at the server
message_t add_at_server(server_data *table, message_t clientMsg){
	message_t ack;
	int client_port, elements;
	int i, table_idx, ret;
	int *index;
	DEBUG
		if (clientMsg.message[1] >= 0){	//first time here
		/*
		 * - Extract the port number from idx 1 and store it locally
		 * - TODO If the entry exists, return with FAILURE
		 * - Set the flags appropriately for the table entry and free the data if NULL
		 *   	> flag = INCOMPLETE
		 *   	> client_id = port number (optional)
		 * - Extract the message and copy to data using size as the index
		 * - If last message, change flag
		 *   	> Identified by if clientMsg[0] is negative for last message
		 *   	> Or add a length of message element to the first message
		 * - Create and send the Ack message - |ADD|S_ID|<blank>|
		 *   Note : S_ID could be used as negative of tabe entry number, so CONTINUED wont b needed
		 */

		client_port = clientMsg.message[1];
		table_idx = find_free_entry();
		printf("idx = %d\n", table_idx);
		if (table_idx <0)
			return;	//There is no free table entry
DEBUG
		table[table_idx].flag = INCOMPLETE;
		table[table_idx].client_id = client_port;
		index = &table[table_idx].size;
		//since this is the first time, no need to check for buf overflow
		for (i=0; i<8; i++){	//TODO Change upper limit to accomiodate smaller packet sizes
			table[table_idx].data[(*index)++] = (char)clientMsg.message[i+2];
			printf("char recvd : %c\n", table[table_idx].data[(*index)++]); 
			if (table[table_idx].data[(*index)++] == '\0')
				break;
		}

		//mark entry FULL if last
		if (clientMsg.message[0] < 0){
			table[table_idx].data[(*index)] = (char)'\0';	//not needed really
			table[table_idx].flag = FULL;
		}

	}
	else {/*if (clientMsg[1] <= 0)*/
		/*
		 * - Extract the session id from idx 1
		 * - See whether the table entry is marked INCOMPLETE, if not, return w/ FAILURE
		 * - Get the size from the table structure, which is also the last index value
		 * - Parse the message to see if the message is the last message
		 *   	> TODO How? Maybe assuming null terminated string and look for ascii of null
		 * - Write message from this point checking for buffer overflow
		 * - Create and send Ack message - |CONTINUED|S_ID|<blank>
		 */
DEBUG
		table_idx = (-1) * clientMsg.message[1];
		client_port = table[table_idx].client_id;
		
		if (table[table_idx].flag != INCOMPLETE){
			return ;	//Not in a position to continue writing
		}

		index = &table[table_idx].size;
		
		for (i=0; i<8; i++){	//TODO Change upper limit to accomiodate smaller packet sizes
			table[table_idx].data[(*index)++] = (char)clientMsg.message[i+2];
			if (table[table_idx].data[(*index)++] == '\0'){
				break;
			}
		}

		//mark entry FULL if last
		if (clientMsg.message[0] < 0){
			table[table_idx].data[(*index)] = (char)'\0';	//not needed really
			table[table_idx].flag = FULL;
		}
		
		printf("Message till now is : %s\n", table[table_idx].data);
	}
DEBUG
	ack.message[0] = ADD;
	ack.message[1] = (-1 * table_idx);
	ret = send (ack, client_port);

	//	return returnMsg;
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



void server(int port){
//	server_data table[SERVER_SIZE];
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
	table[0].size = random_string(table[0].data);
	table[0].flag = FULL;
	table[0].clientId = 1;

	table[1].size = random_string(table[1].data);
	table[1].flag = FULL;
	table[1].clientId = 1;

	table[2].size = random_string(table[2].data);
	table[2].flag = FULL;
	table[2].clientId = 1;
#endif

DEBUG
	while(1){
		msg = receive(port);
printf("case is %d\n", abs(msg.message[0]));
		switch(abs(msg.message[0])){
			case ADD:
				add_at_server(table, msg);
				DEBUG

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

			default:
				break;
		}
	}


}

void client(int id){
	message_t msg;
	int choice, i, ret;
	char dummy[200];

	ret = random_string (dummy);
	printf("Client thread %d starting\n", id);

	if(id == 1 || id == 2){
		//Add or delete
		while(1){
//			choice = (rand() % 3); 
			choice = ADD;			

			switch(choice){
				case ADD:
					printf("%s\n", dummy);
					DEBUG
					add_from_client(dummy, id);
					DEBUG
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
	start_thread(client, 1);
//	start_thread(client, 2);

	//Client to print
	start_thread(clientPrint, 3);

	run();  //Let the give-and-take begin

	return;
}
