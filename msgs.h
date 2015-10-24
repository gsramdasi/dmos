#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define PORT_MESSEGE_COUNT 4

typedef struct message_struct{
	int data[10];
}message;

typedef struct port_struct{
	message* msgQ;
	Semaphore_t sem;
}port;

typedef struct set_struct{
	port ports[100];
}set;
