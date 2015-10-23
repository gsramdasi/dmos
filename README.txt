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
thread_test.c		:Contains the test code which tests the functionality
makefile		:Generate the object file using this makefile
sem.h			:Contains routines related to semaphores
proc_cons.c		:Test code for procedure consumer problem, This code has been provided by the professor
read_writs.c		:Test code for reader writer problem.
read_write_s.c		:Modified test code of reader writer provided by the professor

Steps to Execute:
1. Unzip the folder.
2. To test the procedure consumer problem run "make pc"
	Execute the object file created using the command "./a.out"
3. To test the reader writer problem run "make rw"
	Execute the object file created using the command "./a.out"
4. To test the professors read write code run "gcc read_write_s.c"
	Execute the object file created using the command "./a.out"

Notes:
1. The procedure consumer creates 4 threads each, the procedures write to different location in the buffer. 
	And the consumer read from this buffer.
2. The reader writer creates 3 threads each, The writer writes to the buffer, and each reader reads the entire buffer.
	Writes write only after all the readers have read. 


