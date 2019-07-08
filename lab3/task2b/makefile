#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: main

# Tool invocations
# Executable "main" depends on the files main.o and run.o.
main: toy_printf.o main.o
	gcc -g -Wall -o main toy_printf.o main.o

# Depends on the source and header files
toy_printf.o: toy_printf.c
	gcc -g -Wall -c -o toy_printf.o toy_printf.c 
 
main[1].o: main.c
	gcc -g -Wall -c -o main.o main.c 


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main