
all: q.h TCB.h threads.h sem.h
	gcc thread_test.c

clean:
	rm a.out
