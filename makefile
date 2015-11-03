INCLUDES=./		#this ensures one can include custom header files like <header.h> ALSO. Not just as "header.h"

msg:
	gcc msgs_test.c -I$(INCLUDES)

rw: q.h TCB.h threads.h sem.h
	gcc read_write.c  -I$(INCLUDES)

pc: q.h TCB.h threads.h sem.h
	gcc prod_cons.c -I$(INCLUDES)

clean:
	rm a.out
