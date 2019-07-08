#include "lab4_util.h"
#include <stdio.h>
#include <string.h>
#define STDOUT 1
#define WRITE 1
#define OPEN 2
#define RDRW 2
#define RDONLY 00
#define EXIT 60
#define READ 0
#define DIR 4   
#define DEN 78
extern int system_call();

#define BUF_SIZE 1024
int name_dir=-1;

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct linux_dirent {
    long           d_ino;
    long          d_off;
    unsigned short d_reclen;
    char           d_name[];
};


int print_dir(char* name,char *path){
    int fd;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    char d_type;
    int nread;
    int j=0;
    char buf_path[BUF_SIZE+1];
    int prog=0;
    int i=0;
     int len_next_dir=0;
      int len_prev_dir=0;
    while(i<simple_strlen(path)){
        buf_path[i]=path[i];
        i++;
    }
    buf_path[i]='/';
    buf_path[i+1]=0;
    fd = system_call(OPEN, path, RDONLY, 0777);
    if (fd == -1){
        system_call(EXIT, 0x55);
    }
    for ( ; ; ) {

        nread=system_call(DEN, fd,buf, BUF_SIZE);
        if(nread<=0){
            break;
        }
         for(prog=0;prog<nread;){ 
            d=(struct linux_dirent*)(buf+prog);
            if(d->d_name[0]=='.'){
                d_type=*(buf+prog+d->d_reclen-1);
                prog+=d->d_reclen;
              continue;
            }
     		if(name_dir!=-1){
            if(simple_strcmp(name, d->d_name)==0){
            system_call(WRITE, STDOUT, buf_path, simple_strlen(buf_path));
            system_call(WRITE, STDOUT, d->d_name, simple_strlen(d->d_name));
            system_call(WRITE, STDOUT, "\n", 1);
            }
        }
     
            else{
            system_call(WRITE, STDOUT, buf_path, simple_strlen(buf_path));
            system_call(WRITE, STDOUT, d->d_name, simple_strlen(d->d_name));
            system_call(WRITE, STDOUT, "\n", 1);
            }
            d_type=*(buf+prog+d->d_reclen-1);
            prog+=d->d_reclen;
            if(d_type==DIR){
                 len_next_dir=simple_strlen(d->d_name);
                  len_prev_dir=simple_strlen(buf_path);
                char new_path [len_prev_dir+len_next_dir+2];
                for(j=0;j<len_prev_dir;j++){
                    new_path[j]=buf_path[j];
                }
                new_path[j]=0;
                for(j=len_prev_dir;j<len_next_dir+len_prev_dir;j++){
                    new_path[j]=d->d_name[j-len_prev_dir];
                }
                new_path[j]=0;
              print_dir(name,new_path);
            }
        }
}
return 0;
}



int main(int argc, char *argv[])
{
   int k=0;
   if(argc==1){
   	system_call(WRITE, STDOUT, ".\n",2);
   	print_dir("",".");
   }
   for(k=0;k<argc; k++){
    if(simple_strcmp(argv[k], "-n")==0){
        name_dir=k;
       	print_dir(argv[name_dir+1],".");

    }
}
   return 0;
}