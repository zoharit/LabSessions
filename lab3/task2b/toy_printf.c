
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

typedef struct {
    char** zohar;
    char* fs;
    int bool_not_space;
    int bool_first_tab;
    int bool_placeholder;
    int couner;
    int int_value_abs;
    int space_tabs;
    int bool_minus;
    int len_word;
    int size;
    int int_value;

} state_args;



struct state_result {
  int printed_chars;
  enum printf_state new_state;
};
struct state_result (*state_handlers_array[4])(va_list args, state_args* state);

struct state_result printf_percent_handler(va_list args,state_args* state);
struct state_result printf_array_handler(va_list args,  state_args* state);
struct state_result init_state_handler(va_list args, state_args* state);
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

struct state_result init_state_handler(va_list args,state_args* state){
    struct state_result res;
    res.printed_chars=0;
   // enum printf_state state_res=st_printf_init;
    switch (*(state->fs)) {
      case '%':
	res.new_state = st_printf_percent;
        return res;

      default:
	putchar(*(state->fs));
	res.printed_chars=res.printed_chars+1;
      }
      res.new_state=st_printf_init;
      return res;
}

char* nextChar(char* fs){
    while(*(fs)>='0' && *(fs)<='9'){
            fs++;
        }
       --fs;
    return fs;
}

struct state_result printf_percent_handler(va_list args, state_args* state){
   // int space_tabs=0;   
      struct state_result res;
    res.printed_chars=0;
  
    int i=0;
    int int_value = 0;
    int int_value_abs=0;
    char *string_value;
    int len_word=0;
    char char_value;
 // enum printf_state state_res=st_printf_percent;
    switch (*(state->fs)) {
      case '%':
	putchar('%');
	res.printed_chars++;
	res.new_state = st_printf_init;
	break;
    case '-':
          state->bool_minus=1;
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
        (state->space_tabs)=(state->space_tabs)*10+(*(state->fs)) - '0';
        state->bool_not_space=0;
        break;
    case '0':
        if(state->bool_first_tab==0){
            state->bool_first_tab=1;
            state->bool_placeholder=1;
        }
        else if(state->bool_first_tab==1){
        (state->space_tabs)=(state->space_tabs)*10;
        state->bool_not_space=0;
        }
        break;
          
        case 'A':
            res.new_state=st_array;
        break;
      case 'u':
          int_value=va_arg(args, int);
          res.printed_chars+=print_int1(int_value, 10,digit);
          res.new_state=st_printf_init;
          break;
      case 'd':
        int_value = va_arg(args, int);
        state->int_value_abs=int_value;
        while(abs(state->int_value_abs)>0){
            state->couner++;
            state->int_value_abs=int_value_abs/10;
        }
          if(state->bool_placeholder==1){
            if(int_value<0){
                putchar('-');
                for(i=0;i<(state->space_tabs-state->couner-1);i++){ 
                    putchar('0');
                }
            }else{
                for(i=0;i<(state->space_tabs-state->couner);i++){ 
                    putchar('0');
                }
            }
        if(int_value>=0){
	res.printed_chars+= print_int(int_value, 10, digit);
        }
	else{
          res.printed_chars+= print_int(-int_value, 10, digit);
           }
        }
        else if(state->bool_placeholder==0){
            if(state->bool_minus==1){
                for(i=0;i<state->space_tabs-state->couner;i++){
                    putchar(' ');
                }
	res.printed_chars += print_int(int_value, 10, digit);
            }
        else if(state->bool_minus==0){
                if(state->space_tabs-state->couner>0){
                res.printed_chars+= print_int(int_value, 10, digit);                    
              for(i=0;i<state->space_tabs-state->couner;i++){
                    putchar(' ');
                    if(i==(state->space_tabs-state->couner-1)){
                        putchar('#');

                    }
                }
                }
            }

// 	int_value = va_arg(args, int);
// 	chars_printed += print_int(int_value, 10, digit);
        }        
	res.new_state = st_printf_init;
	break;

      case 'b':
	int_value = va_arg(args, int);
	res.printed_chars += print_int1(int_value, 2, digit);
	res.new_state = st_printf_init;
	break;

      case 'o':
	int_value = va_arg(args, int);
	res.printed_chars += print_int1(int_value, 8, digit);
	res.new_state = st_printf_init;
	break;
	
      case 'x':
	int_value = va_arg(args, int);
	res.printed_chars+= print_int1(int_value, 16, digit);
	res.new_state = st_printf_init;
	break;

      case 'X':
	int_value = va_arg(args, int);
	res.printed_chars+= print_int1(int_value, 16, DIGIT);
	res.new_state = st_printf_init;
	break;

      case 's':
	string_value = va_arg(args, char *);
        len_word=strlen(string_value);
        if(state->bool_not_space==1){
            while(*string_value){
		res.printed_chars++;
		putchar(*string_value);
		string_value++;
            }
        }
        else if(state->bool_minus==0){
	if(state->space_tabs>0){
            while(*string_value){
                state->space_tabs--;
                res.printed_chars++;
                putchar(*string_value);
                string_value++;
            }
            if(state->space_tabs>0){
                while(state->space_tabs>0){
                    putchar(' ');
                    if(state->space_tabs==1){
                        putchar('#');
                    }
                    state->space_tabs--;
                }
            }
        }
        }
        else if(state->bool_minus==1){
            while((state->space_tabs-len_word)>0){
                    putchar(' ');
                    state->space_tabs--;
                }
        while(*string_value){
            state->space_tabs--;
		res.printed_chars++;
		putchar(*string_value);
		string_value++;
	}
        }  
	res.new_state = st_printf_init;
	break;

      case 'c':
	char_value = (char)va_arg(args, int);
	putchar(char_value);
	++res.printed_chars;
	res.new_state = st_printf_init;
	break;

      default:
	toy_printf("Unhandled format %%%c...\n", *(state->fs));
	exit(-1);
    }
    return res;
}


struct state_result printf_array_handler(va_list args, state_args* state){
    int *int_value_arr;
    char *char_value_arr;
    char **string_value_arr;
    int i=0;
   // enum printf_state state_res=st_array;

    struct state_result res;
    res.printed_chars=0;
   
    switch (*(state->fs)) {
      case '%':
	putchar('%');
	++res.printed_chars;
	res.new_state= st_printf_init;
        return res;

      case 'u':
         int_value_arr=va_arg(args, int *);
            state->size=va_arg(args, int);
            putchar('{');
            for(i=0;i<state->size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            res.printed_chars+=print_int1(int_value_arr[i], 10,digit);
            res.new_state=st_printf_init;

        }
        putchar('}');
                    return res;
      case 'd':
            int_value_arr=va_arg(args, int *);
            state->size=va_arg(args, int);
            putchar('{');
            for(i=0;i<state->size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            res.printed_chars+=print_int(int_value_arr[i], 10,digit);
            res.new_state=st_printf_init;

        }
        putchar('}');
        return res;

      case 'b':
         int_value_arr=va_arg(args, int *);
            state->size=va_arg(args, int);
            putchar('{');
            for(i=0;i<state->size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
           res.printed_chars+=print_int1(int_value_arr[i], 2,digit);
            res.new_state=st_printf_init;

        }
        putchar('}');
return res;

      case 'o':
	 int_value_arr=va_arg(args, int *);
            state->size=va_arg(args, int);
            putchar('{');
            for(i=0;i<state->size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            res.printed_chars+=print_int1(int_value_arr[i], 8,digit);
            res.new_state=st_printf_init;

        }
        putchar('}');
return res;
	
      case 'x':
	 int_value_arr=va_arg(args, int *);
            state->size=va_arg(args, int);
            putchar('{');
            for(i=0;i<state->size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            res.printed_chars+=print_int1(int_value_arr[i], 16,digit);
            res.new_state=st_printf_init;

        }
        putchar('}');
return res;

      case 'X':
	 int_value_arr=va_arg(args, int *);
            state->size=va_arg(args, int);
            putchar('{');
            for(i=0;i<state->size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
            res.printed_chars+=print_int1(int_value_arr[i], 16,DIGIT);
            res.new_state=st_printf_init;

        }
        putchar('}');
return res;
      case 's':
	string_value_arr = va_arg(args, char **);
        state->size=va_arg(args, int);
        putchar('{');
        for(i=0;i<state->size;i++){
       	if(i!=0){
            putchar(',');
            putchar(' ');
            
        }
        putchar('"');
        while(**string_value_arr){
            res.printed_chars++;
            putchar(**string_value_arr);
            (*string_value_arr)++;
        }
        putchar('"');
        string_value_arr++;
        res.new_state = st_printf_init;

        }
        putchar('}');
        return res;
      case 'c':
	char_value_arr = va_arg(args, char *);
        state->size=va_arg(args, int);
        putchar('{');
        for(i=0;i<state->size;i++){
            if(i!=0){
                putchar(',');
                putchar(' ');
            }
        putchar(*char_value_arr);
        ++res.printed_chars;
	res.new_state = st_printf_init;
        char_value_arr++;
            
        }
	putchar('}');
return res;

      default:
	toy_printf("Unhandled format %%%c...\n", *(state->fs));
	exit(-1);
      }   
    res.new_state = st_printf_init;
    return res;
}


void init_state_handers_array(){
	state_handlers_array[st_printf_init] = init_state_handler;
	state_handlers_array[st_printf_percent] = printf_percent_handler;
        state_handlers_array[st_array] =printf_array_handler;
}


void init_array_of_state(){
	state_handlers_array[st_printf_init] = init_state_handler;
	state_handlers_array[st_printf_percent] = printf_percent_handler;
        state_handlers_array[st_array] =printf_array_handler;
}
int toy_printf(char *fs, ...) {
  int chars_printed = 0;
  va_list args;
  enum printf_state state;
  state_args stateargs;
  struct state_result res;
  stateargs.bool_not_space=1;
  stateargs.bool_first_tab=0;
  stateargs.bool_placeholder=0;
  stateargs.couner=0;
  stateargs.size=0;
  stateargs.space_tabs=0;
  stateargs.bool_minus=0;
    stateargs.int_value_abs=0;
  stateargs.len_word=0;
  stateargs.int_value=0;
  va_start(args, fs);
    init_array_of_state(); 

  state = st_printf_init; 

        for (; *fs != '\0'; ++fs) {
            stateargs.fs = fs;
            res = (*state_handlers_array[state])(args, &stateargs);
            state = res.new_state;
            chars_printed += res.printed_chars;
	}

      
  va_end(args);

  return chars_printed;
}
