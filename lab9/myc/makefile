all:client

client: client.o common.o line_parser.o
	gcc -g -m64 -Wall -o client client.o common.o line_parser.o

client.o: client.c
	gcc -g -m64 -Wall -c -o client.o client.c
	
common.o: common.c
	gcc -g -m64 -Wall -c -o common.o common.c

line_parser.o: line_parser.c
	gcc -g -m64 -Wall -c -o line_parser.o line_parser.c
clean: 
	rm -f *.o client 
