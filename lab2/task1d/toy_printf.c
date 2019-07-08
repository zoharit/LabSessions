
/* toy-io.c
 * Limited versions of printf and scanf
 *
 * Programmer: Mayer Goldberg, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* the states in the printf state-machine */
enum printf_state {
  st_printf_init,
  st_array,
  st_printf_percent,
  st_printf_octal2,
  st_printf_octal3
};

#define MAX_NUMBER_LENGTH 64
#define is_octal_char(ch) ('0' <= (ch) && (ch) <= '7')

int toy_printf(char *fs, ...);
const char *digit = "0123456789abcdef";
const char *DIGIT = "0123456789ABCDEF";

int print_int_helper1(unsigned int n, int radix, const char *digit) {
int result;
  if (n < radix) {
    putchar(digit[n]);
    return 1;
  }
  else {
    result = print_int_helper1(n / radix, radix, digit);
    putchar(digit[n % radix]);
    return 1 + result;
  }
}

int print_int_helper(int n, int radix, const char *digit) {
  int result;

  if (n < radix) {
    putchar(digit[n]);
    return 1;
  }
  else {
    result = print_int_helper(n / radix, radix, digit);
    putchar(digit[n % radix]);
    return 1 + result;
  }
}

int print_int1(unsigned int n, int radix, const char * digit) {
  if (radix < 2 || radix > 16) {
    toy_printf("Radix must be in [2..16]: Not %d\n", radix);
    exit(-1);
  }

  if (n > 0) {
    return print_int_helper1(n, radix, digit);
  }
  if (n == 0) {
    putchar('0');
    return 1;
  }
  else {
    putchar('-');
    return 1 + print_int_helper1(-n, radix, digit);
  }
}


int print_int(int n, int radix, const char * digit) {
  if (radix < 2 || radix > 16) {
    toy_printf("Radix must be in [2..16]: Not %d\n", radix);
    exit(-1);
  }

  if (n > 0) {
    return print_int_helper(n, radix, digit);
  }
  if (n == 0) {
    putchar('0');
    return 1;
  }
  else {
    putchar('-');
    return 1 + print_int_helper(-n, radix, digit);
  }
}

/* SUPPORTED:
 *   %b, %d, %o, %x, %X -- 
 *     integers in binary, decimal, octal, hex, and HEX
 *   %s -- strings
 */

int toy_printf(char *fs, ...) {
  int chars_printed = 0;
  int int_value = 0;
  int i=0;
  int *int_value_arr;
  char *char_value_arr;
  int size=0;
  char **string_value_arr;
  int space_tabs=0;
  char *string_value;
  char char_value;
  va_list args;
  int len_word=0;
  int bool_minus=0;
  int bool_not_space=1;
  int bool_first_tab=0;
  int bool_placeholder=0;
  int couner=0;
  enum printf_state state;
  int int_value_abs=0;
  va_start(args, fs);

  state = st_printf_init; 

  for (; *fs != '\0'; ++fs) {
    switch (state) {
    case st_printf_init:
      switch (*fs) {
      case '%':
	state = st_printf_percent;
	break;

      default:
	putchar(*fs);
	++chars_printed;
      }
      break;

    case st_printf_percent:
      switch (*fs) {
      case '%':
	putchar('%');
	++chars_printed;
	state = st_printf_init;
	break;
    case '-':
          bool_minus=1;
        break;
    case '1':
         
    case '2':
         
    case '3':
        
    case '4':
          
    case '5':
       
    case '6':
       
    case '7':
        
    case '8':
       
    case '9':
        space_tabs=space_tabs*10+(*fs) - '0';
        bool_not_space=0;
        break;
    case '0':
        if(bool_first_tab==0){
            bool_first_tab=1;
            bool_placeholder=1;
        }
        else if(bool_first_tab==1){
        space_tabs=space_tabs*10;
        bool_not_space=0;
        }
        break;
          
        case 'A':
            state=st_array;
        break;
      case 'u':
          int_value=va_arg(args, int);
          chars_printed+=print_int1(int_value, 10,digit);
          state=st_printf_init;
          break;
      case 'd':
        int_value = va_arg(args, int);
        int_value_abs=int_value;
        while(abs(int_value_abs)>0){
            couner++;
            int_value_abs=int_value_abs/10;
        }
        if(bool_placeholder==1){
            if(int_value<0){
                putchar('-');
                for(i=0;i<(space_tabs-couner-1);i++){ 
                    putchar('0');
                }
            }else{
                for(i=0;i<(space_tabs-couner);i++){ 
                    putchar('0');
                }
            }
        if(int_value>=0){
	chars_printed += print_int(int_value, 10, digit);
        }
	else{
            chars_printed += print_int(-int_value, 10, digit);
           }
        }
        else if(bool_placeholder==0){
            if(bool_minus==1){
                for(i=0;i<space_tabs-couner;i++){
                    putchar(' ');
                }
	chars_printed += print_int(int_value, 10, digit);
            }
        else if(bool_minus==0){
                if(space_tabs-couner>0){
                	chars_printed += print_int(int_value, 10, digit);                    
              for(i=0;i<space_tabs-couner;i++){
                    putchar(' ');
                    if(i==(space_tabs-couner-1)){
                        putchar('#');
                    }
                }
                }
            }
// 	int_value = va_arg(args, int);
// 	chars_printed += print_int(int_value, 10, digit);
        }        
	state = st_printf_init;
	break;

      case 'b':
	int_value = va_arg(args, int);
	chars_printed += print_int1(int_value, 2, digit);
	state = st_printf_init;
	break;

      case 'o':
	int_value = va_arg(args, int);
	chars_printed += print_int1(int_value, 8, digit);
	state = st_printf_init;
	break;
	
      case 'x':
	int_value = va_arg(args, int);
	chars_printed += print_int1(int_value, 16, digit);
	state = st_printf_init;
	break;

      case 'X':
	int_value = va_arg(args, int);
	chars_printed += print_int1(int_value, 16, DIGIT);
	state = st_printf_init;
	break;

      case 's':
	string_value = va_arg(args, char *);
        len_word=strlen(string_value);
        if(bool_not_space==1){
            while(*string_value){
		chars_printed++;
		putchar(*string_value);
		string_value++;
            }
        }
        else if(bool_minus==0){
	if(space_tabs>0){
            while(*string_value){
                space_tabs--;
                chars_printed++;
                putchar(*string_value);
                string_value++;
            }
            if(space_tabs>0){
                while(space_tabs>0){
                    putchar(' ');
                    if(space_tabs==1){
                        putchar('#');
                    }
                    space_tabs--;
                }
            }
        }
        }
        else if(bool_minus==1){
            while((space_tabs-len_word)>0){
                    putchar(' ');
                    space_tabs--;
                }
        while(*string_value){
            space_tabs--;
		chars_printed++;
		putchar(*string_value);
		string_value++;
	}
        }  
	state = st_printf_init;
	break;

      case 'c':
	char_value = (char)va_arg(args, int);
	putchar(char_value);
	++chars_printed;
	state = st_printf_init;
	break;

      default:
	toy_printf("Unhandled format %%%c...\n", *fs);
	exit(-1);
      }
      break;
      case st_array:
           switch (*fs) {
      case '%':
	putchar('%');
	++chars_printed;
	state = st_printf_init;
	break;
      case 'u':
         int_value_arr=va_arg(args, int *);
            size=va_arg(args, int);
            putchar('{');
            for(i=0;i<size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            chars_printed+=print_int1(int_value_arr[i], 10,digit);
            state=st_printf_init;
        }
        putchar('}');
          break;
      case 'd':
            int_value_arr=va_arg(args, int *);
            size=va_arg(args, int);
            putchar('{');
            for(i=0;i<size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            chars_printed+=print_int(int_value_arr[i], 10,digit);
            state=st_printf_init;
        }
        putchar('}');
          break;

      case 'b':
         int_value_arr=va_arg(args, int *);
            size=va_arg(args, int);
            putchar('{');
            for(i=0;i<size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            chars_printed+=print_int1(int_value_arr[i], 2,digit);
            state=st_printf_init;
        }
        putchar('}');
        break;

      case 'o':
	 int_value_arr=va_arg(args, int *);
            size=va_arg(args, int);
            putchar('{');
            for(i=0;i<size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            chars_printed+=print_int1(int_value_arr[i], 8,digit);
            state=st_printf_init;
        }
        putchar('}');
          break;
	
      case 'x':
	 int_value_arr=va_arg(args, int *);
            size=va_arg(args, int);
            putchar('{');
            for(i=0;i<size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            chars_printed+=print_int1(int_value_arr[i], 16,digit);
            state=st_printf_init;
        }
        putchar('}');
          break;

      case 'X':
	 int_value_arr=va_arg(args, int *);
            size=va_arg(args, int);
            putchar('{');
            for(i=0;i<size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            chars_printed+=print_int1(int_value_arr[i], 16,DIGIT);
            state=st_printf_init;
        }
        putchar('}');
          break;
      case 's':
	string_value_arr = va_arg(args, char **);
        size=va_arg(args, int);
        putchar('{');
        for(i=0;i<size;i++){
       	if(i!=0){
            putchar(',');
            putchar(' ');
            
        }
        putchar('"');
        while(**string_value_arr){
            chars_printed++;
            putchar(**string_value_arr);
            (*string_value_arr)++;
        }
        putchar('"');
        string_value_arr++;
        state = st_printf_init;
        }
        putchar('}');
        break;
      case 'c':
	char_value_arr = va_arg(args, char *);
        size=va_arg(args, int);
        putchar('{');
        for(i=0;i<size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
        putchar(*char_value_arr);
        ++chars_printed;
	state = st_printf_init;
        char_value_arr++;
            
        }
	putchar('}');
        break;

      default:
	toy_printf("Unhandled format %%%c...\n", *fs);
	exit(-1);
      }
          
          break;
      
    default:
      toy_printf("toy_printf: Unknown state -- %d\n", (int)state);
      exit(-1);
      
    }
  }

  va_end(args);

  return chars_printed;
}
