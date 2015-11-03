#include "sem.h"

#define N 10
#define TOTAL_PORTS 100

typedef struct msgererer{
	int message[10];
}message_t;

typedef struct {
	message_t message[N];
	int send_idx, recv_idx;			//current send and receive indices
	Semaphore_t *empty, *full;	//producer and consumer semaphores
	Semaphore_t *mutex;				//mutex semaphore
}port_t;

port_t port[TOTAL_PORTS];
void init_ports(){
	/*
	 * init semaphores
	 * memset messages to 0s	-	Optional
	 */
	int i;
	for (i=0; i<TOTAL_PORTS; i++){
		port[i].empty = CreateSem(0);
		port[i].full = CreateSem(N);
		port[i].mutex = CreateSem(1);
		
		port[i].recv_idx = port[i].send_idx = 0;
	}
	printf("Ports have been initialized\n");

}
//Assuming messages of fixed length
int send (message_t msg, int port_nr){
	/*
	 * *************
	 * Pseudocode:
	 * *************
	 * 
	 * P(send_s)
	 * Copy message to port
	 * 		Check message length
	 * 		if large, error
	 * 		if small, pad the message with NULLs or 0s - i.e. make the message NULL before starting out
	 * V(send_s)
	 */
	
	//See if port is full
	P(port[port_nr].full);
	
	//may add len argument to function and compare with message_t size and memset only if less
	P(port[port_nr].mutex);
	memset(&port[port_nr].message[port[port_nr].send_idx], 0, sizeof(message_t));	//May be skipped
	memcpy(&port[port_nr].message[port[port_nr].send_idx], &msg, sizeof(message_t));
	
	//update send(write) index
	port[port_nr].send_idx = (++port[port_nr].send_idx)%N;
	V(port[port_nr].mutex);
	V(port[port_nr].empty);
	return 0;
}

message_t receive (int port_nr){

	/*
	 * *************
	 * Pseudocode:
	 * *************
	 *
	 * P(recv_s)
	 * Remove the message from port
	 * 		copy message
	 * 		memset message to 0s or NULLs
	 * V(recv_s)
	 */
	message_t ready_msg;

	//verify that the message buffer isn't empty
	P(port[port_nr].empty);	
	P(port[port_nr].mutex);

	//put message received from the sender into the respective port's message buffer
	memcpy(&ready_msg, &port[port_nr].message[port[port_nr].recv_idx], sizeof(message_t));
	memset(&port[port_nr].message[port[port_nr].recv_idx], 0, sizeof(message_t));	//May be skipped

	//update recv(read) index
	port[port_nr].recv_idx = (++port[port_nr].recv_idx)%N;
	if(port[port_nr].recv_idx < 0)
		port[port_nr].recv_idx += N; 

	V(port[port_nr].mutex);
	V(port[port_nr].full);


	return ready_msg;
}

