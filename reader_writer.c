#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define N 4
Semaphore_t *rsem;
Semaphore_t *wsem;
Semaphore_t *mutex;

int rc=0,rwc,wwc,wc=0;
int buffer[3]={1,2,3};
int reader_delay,writer_delay=1;

void reader(void)
{
//reader enter
while(1) {
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
printf("Reading from buffer: %d %d %d \n",buffer[0],buffer[1],buffer[2]);
//Reader Exit
P(mutex);
rc--;
if(rc==0 && wwc>0)
V(wsem);
else
V(mutex);
sleep(reader_delay);
}
}
 
void writer(void)
{
int i;
//Writer enter
while(1)
{
P(mutex);
if(rc>0 || wc>0) {
wwc++;
V(mutex);
P(wsem);
wwc--;
}
wc++;
V(mutex);

//write occurs here
for(i=0;i<3;i++)
{
buffer[i]=buffer[i]*4;
}
 printf("Writing to buffer: %d %d %d\n",buffer[0],buffer[1],buffer[2]);
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
rsem = CreateSem(N, 1);
mutex = CreateSem(1, 2);
start_thread(reader);
start_thread(writer);
run();
DEBUG;
}
