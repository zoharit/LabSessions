#include "lab4_util.h"
extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
    char* name_file=argv[1];
    char* name =argv[2];
    int des=system_call(2, name_file,2,0777);
    if(name_file<0){
    	system_call(60);
    }
    else {
	system_call(8, des, 0x1015,1);
	system_call(1, des, name, simple_strlen(name)+1);
	system_call(3,des);
    }

  return 0;
}