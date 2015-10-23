
rw: q.h TCB.h threads.h sem.h
	gcc reader_writer.c

pc: q.h TCB.h threads.h sem.h
	gcc prod_cons_s.c

clean:
	rm a.out
