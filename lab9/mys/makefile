all:server

server: server.o common.o
	gcc -g -m64 -Wall -o server server.o common.o

server.o: server.c
	gcc -g -m64 -Wall -c -o server.o server.c

common.o: common.c
	gcc -g -m64 -Wall -c -o common.o common.c

clean: 
	rm -f *.o server 
