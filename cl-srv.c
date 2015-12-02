#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msgs.h"

//Macros
#define SERVER_PORT 	10
#define SERVER_SIZE 	10

#define TRUE 1
#define FALSE 0

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
	int modifying;
	int client_id;
}server_data;

//Global Variables
int printData[2000];
int printDataSize;

//Initalize the server data entry
void initialize_server_data(server_data *data){
	data->size = 0;
	data->flag = EMPTY;
	data->modifying = FALSE;
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
int find_free_entry(server_data *table){
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

	//	printf("Length = %d\n", len);
	while(len > 0){
		//break in_string and keep sending messages with appropriate headers
		//Add headers - idx 0 and 1
		memset(packet.message, 0, sizeof(int) * 10);
		packet.message[0] = ADD;
		if (len <= 8)	//data segment of each packet is 8 integers long
			packet.message[0] *= -1;	//send negative command to signify last packet

		if (s_id>0)
			packet.message[1] = port_nr; /*client's port or session id*/
		else
			packet.message[1] = s_id;

		//start adding message
		//printf("MIN : %d ", MIN(len,8));
		elems = MIN(len,8);
		for (i=0; i<elems; i++){
			packet.message[i+2] = (int)in_string[in_string_idx++];
		}

		ret = send(packet, SERVER_PORT);

		//recv server's response and update session id
		response = receive (port_nr);
		s_id = response.message[1];

		len-=8;

		sleep(1);
	}
}
//modify at the server
message_t modify_at_server(server_data *table, message_t clientMsg, int pos){
	message_t ack;
	int client_port, elements;
	int i, table_idx, ret;

	if (clientMsg.message[1] <= 0){
		if (pos < 0)
			table_idx = (-1) * clientMsg.message[1];
		else
			table_idx = pos;
		client_port = table[table_idx].client_id;

		if (table[table_idx].flag != INCOMPLETE){
			return ;	//Not in a position to continue writing
		}

		for (i=0; i<8; i++){	
			table[table_idx].data[table[table_idx].size] = (char)clientMsg.message[i+2];
			if (table[table_idx].data[table[table_idx].size] == '\0'){
				printf("Server : Msg completely recieved\n");
				table[table_idx].flag = FULL;
				table[table_idx].modifying = FALSE;
				break;
			}
			table[table_idx].size++;
		}
	}


}

//add at the server
message_t add_at_server(server_data *table, message_t clientMsg, int pos){
	message_t ack;
	int client_port, elements;
	int i, table_idx, ret;

	if (clientMsg.message[1] > 0){	//first time here
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
		
		//Find an empty entry
		if(pos < 0)
			table_idx = find_free_entry(table);
		else
			table_idx = pos;
		//printf("idx = %d\n", table_idx);
		if (table_idx < 0)
			return;	//There is no free table entry

		printf("Server Add Initiated\n");

		table[table_idx].flag = INCOMPLETE;
		table[table_idx].client_id = client_port;

		//since this is the first time, no need to check for buf overflow
		for (i=0; i<8; i++){	//TODO Change upper limit to accomiodate smaller packet sizes
			table[table_idx].data[table[table_idx].size] = (char)clientMsg.message[i+2];
			if (table[table_idx].data[table[table_idx].size] == '\0'){
				printf("Server : message completely recieved\n");
				table[table_idx].flag = FULL;
				break;
			}

			table[table_idx].size++;
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
		if (pos < 0)
			table_idx = (-1) * clientMsg.message[1];
		else
			table_idx = pos;
		client_port = table[table_idx].client_id;

		if (table[table_idx].flag != INCOMPLETE){
			return ;	//Not in a position to continue writing
		}

		for (i=0; i<8; i++){	
			table[table_idx].data[table[table_idx].size] = (char)clientMsg.message[i+2];
			if (table[table_idx].data[table[table_idx].size] == '\0'){
				printf("Server : Msg completely recieved\n");
				table[table_idx].flag = FULL;
				table[table_idx].modifying = FALSE;
				break;
			}
			table[table_idx].size++;
		}
	}

	//printf("Message till now is : %s, %d\n", table[table_idx].data, table[table_idx].size);

	ack.message[0] = abs(clientMsg.message[0]);
	ack.message[1] = (-1 * table_idx);
	ret = send (ack, client_port);
}

//Function to delete table entry at the server
message_t delete(server_data *table, message_t clientMsg){
	message_t returnMsg;
	int sessionId = abs(clientMsg.message[1]);
	int client_id = clientMsg.message[2];

	if(/*(table[sessionId].client_id == client_id) &&*/ (table[sessionId].flag == FULL)){
		initialize_server_data(&table[sessionId]);
		printf("Data deleted at %d\n", sessionId);
		returnMsg.message[1] = SUCCESS;
	}
	else{
		returnMsg.message[1] = FAILURE;
	}

	returnMsg.message[0] = clientMsg.message[1];
	return returnMsg;
}

//Start sending to server with a negative session id from the very beginning
int modify_from_client(const char *in_string, int port_nr, int s_id){
	int i=0, ret, elems;
	int len = strlen(in_string);
	int in_string_idx = 0;
	message_t packet, response;

	//	printf("Length = %d\n", len);
	while(len > 0){
		//break in_string and keep sending messages with appropriate headers
		//Add headers - idx 0 and 1
		memset(packet.message, 0, sizeof(int) * 10);
		packet.message[0] = MODIFY;
		if (len <= 8)	//data segment of each packet is 8 integers long
			packet.message[0] *= -1;	//send negative command to signify last packet

		packet.message[1] =  s_id;

		//start adding message
		//printf("MIN : %d ", MIN(len,8));
		elems = MIN(len,8);
		for (i=0; i<elems; i++){
			packet.message[i+2] = (int)in_string[in_string_idx++];
		}

		ret = send(packet, SERVER_PORT);

		//recv server's response and update session id
//		response = receive (port_nr);
		//		s_id = response.message[1];

		len-=8;

		sleep(1);
	}
}


//Populates printData variable
void populate_printData(server_data *table){
	int i, j;
	int flag = FAILURE;
	//Clear printData variable
	memset(printData, 0, (sizeof(char) * 2000));		
	printDataSize = 0;

	for(i = 0; i < SERVER_SIZE; i++){
		if(table[i].flag == FULL){
			flag = SUCCESS;
			//Copy the data to printData in ASCII
			for(j = 0; j < table[i].size; j++){
				printData[printDataSize] = (int)table[i].data[j];	//Converts char to ASCII
				printDataSize++;
			}

			//Add new line char
			printData[printDataSize - 1] = '\n';
		}
	}

	if(flag == SUCCESS)
		printData[printDataSize - 1] = '\0';
	else{
		printData[0] = '\0';
		printDataSize = 1;
	}
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

void server(int port){
	server_data table[SERVER_SIZE];
	int tStart, tEnd;
	int size = 0;
	message_t msg;
	message_t returnMsg;
	int temp, i, entry;

	//Initialize the server table
	initialize_server(table);

	while(1){
		msg = receive(port);
		//	printf("case is %d\n", abs(msg.message[0]));
		switch(abs(msg.message[0])){
			case ADD:
				add_at_server(table, msg, -1);
				break;

			case DELETE:
				returnMsg = delete(table, msg);
				send(returnMsg, msg.message[2]);
				break;

			case PRINT:
				//Print  table
				returnMsg = print(table, msg);
				send(returnMsg, msg.message[1]);
				break;

			case MODIFY:
			/*
				entry = abs(msg.message[1]);
				if(table[entry].modifying == FALSE){
					returnMsg = delete(table, msg);
					table[entry].flag = INCOMPLETE;
					if (returnMsg.message[1] == SUCCESS)
						table[entry].modifying =TRUE;
					send(returnMsg, msg.message[2]);
				}
				//receive next message and add to the server
				add_at_server(table, msg, entry);
				*/
				entry = abs(msg.message[1]);
				table[entry].flag = INCOMPLETE;
				modify_at_server(table, msg, entry);

				break;

			default:
				break;
		}
	}


}

void client(int id){
	message_t msg, returnMsg;
	int sessionId;
	int choice, i, ret;
	char dummy[200] = "test";
	int dummySize = 5;

	ret = random_string (dummy);
	printf("Client thread %d starting\n", id);

	if(id == 1 || id == 2){
		//Add or delete
		while(1){
			choice = (rand() % 3)+1; 
			//	choice = ADD;			

			ret = random_string (dummy);
			switch(choice){
				case ADD:
					add_from_client(dummy, id);
					break;

				case DELETE:
					sessionId = rand()%10; /*0; //TODO: pick a session id*/
					sessionId = -1 * sessionId;
					msg.message[0] = DELETE;
					msg.message[1] = sessionId;
					msg.message[2] = id;

					//Send the message to server and get back its response
					send(msg, SERVER_PORT);
					returnMsg = receive(id);

					//Delete from local add table on success
					if(returnMsg.message[1] == SUCCESS){
						//TODO: Delete from local table
					}

					break;

				case MODIFY:
					//First Delete
					sessionId = rand()%2; /*0; //TODO: pick a session id*/
					sessionId = -1 * sessionId;
					msg.message[0] = DELETE;
					msg.message[1] = sessionId;
					msg.message[2] = id;

					//Send the message to server and get back its response
					send(msg, SERVER_PORT);
					returnMsg = receive(id);

					if(returnMsg.message[1] == SUCCESS){
						//add to the server at the specified entry
						modify_from_client(dummy, id, sessionId);
						printf("Modified Entry %d to %s\n", abs(sessionId), dummy);
					}
					else{
						printf("Cannot modify entry %d\n", abs(sessionId));
					}

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
	int lastBlock, i, sleepTime;

	while(1){
		sleepTime = 5;
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
		if(printData[0] != '\0'){
			printf("Server Data :\n");
			printf("%s\n", printData);
		}
		else{
			printf("Server Data : Empty\n");
		}

		//Change sleep
		sleep(1);
		while(sleepTime > 0){
			yield();
			sleepTime--;
		}
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
