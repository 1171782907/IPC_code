all:synchronous_sem mutex_sem  																
synchronous_sem:synchronous_sem.o 								
	gcc -o synchronous_sem  synchronous_sem.o			
mutex_sem:mutex_sem.o 								
	gcc -o mutex_sem  mutex_sem.o		
clean:										
	rm -rf  synchronous_sem mutex_sem  *.o