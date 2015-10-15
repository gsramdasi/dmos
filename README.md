----------------------------------------------------------
README
----------------------------------------------------------

Authors:
Gaurav Ramdasi	(1206467752)
Menaka Kamath	(1209301934)
Amaresh Talluri	(1207301416)

Files:
q.h		:Contains all the queue handling functions. 
TCB.h		:Contains the TCB structure used in RunQ
threads.h	:Contains thread related functions, such as yielding, starting thread, exit
thread_test.h	:Contains the test code which tests the functionality
makefile	:Generate the object file using this makefile

Steps to Execute:
1. Unzip the folder.
2. Run the make file using the command "make"
3. Execute the object file created using the command "./a.out"

Note:
One additional function added in threads.h (clean_exit). 
This was to remove any memory leaks caused due to not freeing the RunQ. 

