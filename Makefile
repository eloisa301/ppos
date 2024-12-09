semaforo: 
	@gcc -o ppos-test ppos-core-aux.c pingpong-semaphore.c
	@./ppos-test
fila: 
	@gcc -o ppos-test ppos-core-aux.c pingpong-mqueue.c
	@./ppos-test
race:
	@gcc -o ppos-test ppos-core-aux.c pingpong-racecond.c
	@./ppos-test

run: 
	@./ppos-test
clean:
	rm *.o ppos-test