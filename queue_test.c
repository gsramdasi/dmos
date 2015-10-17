/*
 * NOT the Main Test file.
 * Just the queue testing program
 * 
 * Members:
 * Gaurav (1206467752)
 * Menaka (1209301934)
 * Amaresh (1207301416)
 *
 */



#include <stdio.h>
#include "threads.h"

void main(){
	printf("Test\n");	
	char cmd='a';
	int data = 0;
	TCB_t *runq=NULL, *node, *start;
	while (cmd != 'q'){
		scanf("%c",&cmd);

		if(cmd == 'n'){
			scanf("%d", &data);
			node = (TCB_t*)malloc(sizeof(TCB_t));
			node->test = data++;
			AddQ(&runq,node);
			printf("Node added\n");
		}
		if(cmd == 'd'){
			node = DelQ(&runq);
			if(node != NULL){
				printf("Deleted %d\n", node->test);
				free(node);
			}
		}
		if(cmd == 'p'){
			printf("Printing list ...\n");
			if (runq == NULL){
				printf("RunQ is NULL\n");
				continue;
			}
			start = runq;
			printf("%d -> ", runq->test);
			runq = runq->next;
			while(runq != start){
				printf("%d -> ", runq->test);
				RotateQ(&runq);
			}
			printf("\n");
		}
		if (cmd == 'r'){
			RotateQ(&runq);
		}
	}
	while(node = DelQ(&runq)){
		free(node);
	}
}



