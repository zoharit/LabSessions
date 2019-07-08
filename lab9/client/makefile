all:client

client: client.o
	gcc -g -m64 -Wall -o client client.o

client.o: client.c
	gcc -g -m64 -Wall -c -o client.o client.c

clean: 
	rm -f *.o client 
