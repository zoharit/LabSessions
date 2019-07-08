#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: hexeditplus

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
hexeditplus: hexeditplus.o
	gcc -g -Wall -o hexeditplus hexeditplus.o

# Depends on the source and header files
hexeditplus.o: hexeditplus.c
	gcc -g -Wall -c -o hexeditplus.o hexeditplus.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o hexeditplus
