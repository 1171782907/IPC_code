all:creat_fifo read_fifo write_fifo  																
creat_fifo: creat_fifo.o 								
	gcc -o creat_fifo creat_fifo.o
read_fifo: read_fifo.o 								
	gcc -o read_fifo read_fifo.o	
write_fifo: write_fifo.o 								
	gcc -o write_fifo write_fifo.o					
clean:										#清理规则，
	rm -rf  creat_fifo read_fifo write_fifo myfifo *.o