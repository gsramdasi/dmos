
msg:
	gcc msgs_test.c

rw: q.h TCB.h threads.h sem.h
	gcc read_write.c

pc: q.h TCB.h threads.h sem.h
	gcc prod_cons.c

clean:
	rm a.out
