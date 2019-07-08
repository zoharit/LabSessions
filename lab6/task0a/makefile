all : compile

compile : line_parser.o  myshell.o
	@echo 'invoking C linker'
	gcc -m32 -g -Wall -o myshell line_parser.o myshell.o
	@echo 'finished building'
#compile main.c
myshell.o : myshell.c
	@echo 'Compile myshell.c'
	gcc -g -m32 -ansi -Wall -c -o myshell.o myshell.c
line_parser.o : line_parser.c
	@echo 'Compile line_parser.c'
	gcc -g -m32 -ansi -Wall -c -o line_parser.o line_parser.c
clean:
	@echo 'clean all files'
	rm -f myshell.o
	rm -f line_parser.o
	rm -f myshell