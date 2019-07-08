
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


struct state_result (*state_handlers_array[5])(va_list args, state_args* state);

struct state_result (*percent_state[128])(va_list args, state_args* state);
struct state_result (*init_state[128])(va_list args, state_args* state);
struct state_result (*array_state[128])(va_list args, state_args* state);

struct state_result init_state_handler(va_list args, state_args* state);

struct state_result d(va_list args, state_args* state);
struct state_result b(va_list args, state_args* state);
struct state_result x(va_list args, state_args* state);
struct state_result X(va_list args, state_args* state);
struct state_result o(va_list args, state_args* state);
struct state_result s(va_list args, state_args* state);
struct state_result c(va_list args, state_args* state);
struct state_result u(va_list args, state_args* state);
struct state_result number(va_list args, state_args* state);




struct state_result {
  int printed_chars;
  enum printf_state new_state;
};

struct state_result printf_percent_handler(va_list args,state_args* state);
struct state_result printf_array_handler(va_list args,  state_args* state);
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

struct state_result number(va_list args, state_args* state){
	struct state_result res;
	res.printed_chars = 0;
        (state->space_tabs)=(state->space_tabs)*10+(*(state->fs)) - '0';
        state->bool_not_space=0;
        	res.new_state = st_printf_percent;
                res.printed_chars=1;

return res;
    
}

struct state_result zero(va_list args, state_args* state){
	struct state_result res;
	res.printed_chars = 0;
 if(state->bool_first_tab==0){
            state->bool_first_tab=1;
            state->bool_placeholder=1;
        }
        else if(state->bool_first_tab==1){
        (state->space_tabs)=(state->space_tabs)*10;
        state->bool_not_space=0;
        }
        res.new_state = st_printf_percent;
                res.printed_chars=1;

        return res;
    
}

struct state_result percent_char_handler(va_list args, state_args* state){
	struct state_result res;
	res.printed_chars = 0;
        putchar('%');
	res.printed_chars++;
	res.new_state = st_printf_init;

 return res;       
}

struct state_result minus_func(va_list args, state_args* state){
	struct state_result res;
	res.printed_chars = 0;
        state->bool_minus=1;
res.new_state = st_printf_percent;
                res.printed_chars=1;

return res;
}
struct state_result A(va_list args, state_args* state){
	struct state_result res;
	res.printed_chars = 0;
        res.new_state=st_array;
        return res;
}

struct state_result u(va_list args, state_args* state){
            struct state_result res;
            res.printed_chars = 0;
            int int_value;
            int_value=va_arg(args, int);
          res.printed_chars+=print_int1(int_value, 10,digit);
          res.new_state=st_printf_init;
return res;
    
}

struct state_result d(va_list args, state_args* state){
    int int_value;    
    struct state_result res;
    int i=0;
        res.printed_chars = 0;
        int_value = va_arg(args, int);
        state->int_value_abs=int_value;
        while(abs(state->int_value_abs)>0){
            state->couner++;
            state->int_value_abs=state->int_value_abs/10;
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
            
        }
        res.new_state = st_printf_init;
        return res;

}


struct state_result b(va_list args, state_args* state){
    struct state_result res;
    res.printed_chars = 0;
    int int_value;
    int_value = va_arg(args, int);
    res.printed_chars += print_int1(int_value, 2, digit);
    res.new_state = st_printf_init;
    return res;
}



struct state_result o(va_list args, state_args* state){
    struct state_result res;
    res.printed_chars = 0;
   int int_value;
    int_value = va_arg(args, int);
    res.printed_chars += print_int1(int_value, 8, digit);
    res.new_state = st_printf_init;
    return res;	
}





struct state_result x(va_list args, state_args* state){
    struct state_result res;
    res.printed_chars = 0;
    int int_value;
 
    int_value = va_arg(args, int);
    res.printed_chars+= print_int1(int_value, 16, digit);
    res.new_state = st_printf_init;

   return res;
}


struct state_result X(va_list args, state_args* state){
    struct state_result res;
    res.printed_chars = 0;
    int int_value;
 
    int_value = va_arg(args, int);
    res.printed_chars+= print_int1(int_value, 16, DIGIT);
    res.new_state = st_printf_init;

   return res;
}


struct state_result s(va_list args, state_args* state){
struct state_result res;
    char *string_value;

    res.printed_chars = 0;
    
        string_value = va_arg(args, char *);
        state->len_word=strlen(string_value);
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
            while((state->space_tabs-state->len_word)>0){
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

return res;
    
}

struct state_result init_percent_handler(va_list args, state_args* state){
	struct state_result res;
	res.printed_chars = 0;
	res.new_state = st_printf_percent;
	return res;
}


struct state_result c(va_list args, state_args* state){
    struct state_result res;
    char char_value;
    res.printed_chars = 0;
    char_value = (char)va_arg(args, int);
	putchar(char_value);
	++res.printed_chars;
	res.new_state = st_printf_init;
        return res;}
        
struct state_result Arr_c(va_list args, state_args* state){
        struct state_result res;
        char *char_value_arr;
            int i=0;
        res.printed_chars = 0;     
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

       
}
         
struct state_result Arr_s(va_list args, state_args* state){
        struct state_result res;
        char **string_value_arr;
        res.printed_chars = 0;     
            int i=0;

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
        
}

struct state_result Arr_d(va_list args, state_args* state){
        struct state_result res;
        int *int_value_arr;
        res.printed_chars = 0;     
            int i=0;

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
}


struct state_result Arr_u(va_list args, state_args* state){
        struct state_result res;
        int *int_value_arr;
        res.printed_chars = 0;  
            int i=0;

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
    
}


struct state_result Arr_o(va_list args, state_args* state){
        struct state_result res;
        int *int_value_arr;
        res.printed_chars = 0;  
      int i=0;

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
    
}




struct state_result Arr_x(va_list args, state_args* state){
        struct state_result res;
        int *int_value_arr;
        res.printed_chars = 0;  
      int i=0;

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
}


struct state_result Arr_X(va_list args, state_args* state){
 struct state_result res;
        int *int_value_arr;
        res.printed_chars = 0;  
      int i=0;

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
}



struct state_result Arr_b(va_list args, state_args* state){
 struct state_result res;
        int *int_value_arr;
        res.printed_chars = 0;  
      int i=0;

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
}

struct state_result default_handler(va_list args, state_args* state){
	struct state_result res;
	toy_printf("Unhandled format %%%c...\n", *state->fs);
	exit(-1);
	//will never get here, won't compile without it.
	return res;
}


struct state_result init_state_default_char_handler(va_list args, state_args* state){
	struct state_result res;
	res.printed_chars = 1;
	putchar(*state->fs);

	res.new_state = st_printf_init;
	return res;
}

struct state_result init_state_handler(va_list args, state_args* state){
	int index = (int) state->fs[0];
	return (*init_state[index])(args, state);

}
struct state_result percent_state_handler(va_list args, state_args* state){
	int index = (int) state->fs[0];
	return (*percent_state[index])(args, state);
}

struct state_result printf_array_handler(va_list args, state_args* state){
	int index = (int) state->fs[0];
	return (*array_state[index])(args, state);
}


void init_array_state(){
	int i;
	for(i = 0; i < 128; i++){
		percent_state[i] = default_handler;
		init_state[i] = init_state_default_char_handler;
		array_state[i] = default_handler;
	}
        percent_state['-'] = minus_func;
	percent_state['%'] = percent_char_handler;
	percent_state['d'] = d;
	percent_state['b'] = b;
	percent_state['o'] = o;
	percent_state['x'] = x;
	percent_state['X'] = X;
	percent_state['s'] = s;
	percent_state['c'] = c;
	percent_state['u'] = u;
	percent_state['A'] = A;
        percent_state['0'] = zero;
	percent_state['1'] = number;
	percent_state['2'] = number;
	percent_state['3'] = number;
	percent_state['4'] = number;
	percent_state['5'] = number;
	percent_state['6'] = number;
	percent_state['7'] = number;
	percent_state['8'] = number;
	percent_state['9'] = number;

        
        init_state['%'] = init_percent_handler;

	array_state['c'] = Arr_c;
	array_state['s'] = Arr_s;
	array_state['d'] = Arr_d;
	array_state['u'] = Arr_u;
	array_state['o'] = Arr_o;
	array_state['x'] = Arr_x;
	array_state['X'] = Arr_X;
	array_state['b'] = Arr_b;

}


void init_state_arr(){
	state_handlers_array[st_printf_init] = init_state_handler;
	state_handlers_array[st_printf_percent] = percent_state_handler;
        state_handlers_array[st_array] =printf_array_handler;
}


int toy_printf(char *fs, ...) {
    init_state_arr();
  init_array_state();
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
stateargs.fs=fs;
  va_start(args, fs);
  state = st_printf_init; 
	for (; *stateargs.fs != '\0'; ++stateargs.fs) {
		res = (*state_handlers_array[state])(args, &stateargs);
		state = res.new_state;
		chars_printed += res.printed_chars;
	}

  va_end(args);

  return chars_printed;
}
