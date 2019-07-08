all:task0a

task0a: client.o server.o common.o
	gcc -g -m64 -Wall -o task0a client.o server.o common.o

client.o: client.c
	gcc -g -m64 -Wall -c -o client.o client.c

server.o: server.c
	gcc -g -m64 -Wall -c -o server.o server.c
	
common.o: common.c
	gcc -g -m64 -Wall -c -o common.o common.c

clean: 
	rm -f *.o task0a 
