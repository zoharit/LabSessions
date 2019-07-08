all:server

server: server.o
	gcc -g -m64 -Wall -o server server.o

server.o: server.c
	gcc -g -m64 -Wall -c -o server.o server.c

clean: 
	rm -f *.o server 
