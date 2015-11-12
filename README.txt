----------------------------------------------------------
README
----------------------------------------------------------

Authors:
Gaurav Ramdasi	(1206467752)
Menaka Kamath	(1209301934)
Amaresh Talluri	(1207301416)

Files:
q.h			:Contains all the queue handling functions. 
TCB.h			:Contains the TCB structure used in RunQ
threads.h		:Contains thread related functions, such as yielding, starting thread, exit
makefile		:Generate the object file using this makefile
sem.h			:Contains routines related to semaphores
msgs.h			:Contains the port functions such as send and recieve
msgs_test.c		:Test code for the port functions

Steps to Execute:
1. Unzip the folder.
2. To test the messaging functionality run "make msgs"
	Execute the object file created using the command "./a.out"

Notes:
1. The code submitted uses strategy 3. But our code even supports strategy 2, 
	we just need to change the macro value of SEM_COUNT (in msgs.h) to number of ports to get strategy 2. 
2. We have modifed the thread functionality, now the start thread function doesnt just take the thread function pointer,
	but also arguments to this thread function

