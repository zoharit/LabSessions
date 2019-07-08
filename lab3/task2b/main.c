#include "toy_stdio.h"


int main(int argc, char *argv[]) {
toy_printf("Hex unsigned: %x\n", -1); toy_printf("Octal unsigned: %o\n", -1);
 toy_printf("Unsigned value: %u\n", 15);
 toy_printf("Unsigned value: %u\n", -15);
 toy_printf("With numeric placeholders: %09d\n", -1);
 toy_printf("With numeric placeholders!!: %12d\n", -1);
 toy_printf("Non-padded string: %s\n", "str");
 toy_printf("Right-padded string: %6s\n", "str");
 toy_printf("Left-added string: %-6s\n", "str");
 int integers_array[] = {1,2,3,4,5};
 char * strings_array[] = {"This", "is", "array", "of", "strings"};
 int array_size = 5;
 toy_printf("Print array of integers: %Ad\n", integers_array, array_size);
 toy_printf("Print array of strings: %As\n", strings_array, array_size);
} 
