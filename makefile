
all: q.h TCB.h threads.h sem.h
	gcc thread_test.c

pc: 
	gcc prod_cons_s.c

clean:
	rm a.out
