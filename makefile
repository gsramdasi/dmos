
all: q.h TCB.h threads.h sem.h
	gcc reader_writer.c

pc: 
	gcc prod_cons_s.c

clean:
	rm a.out
