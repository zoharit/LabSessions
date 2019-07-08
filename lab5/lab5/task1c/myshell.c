
#include <unistd.h>
#include <linux/limits.h>
#include <stdio.h>
#include "line_parser.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define INSize 2048

void execute (cmd_line *pCmdLine){
    pid_t pid;
    pid = fork();
    if (pid ==-1){
          char * error=strerror(errno);
            perror(error);
            free_cmd_lines(pCmdLine);
            exit(0x55);
    }
    if( pid== 0){
         if(pCmdLine->input_redirect){
        close(0);
        fopen(pCmdLine->input_redirect, O_RDONLY );
        }
        if(pCmdLine->output_redirect){
        close(1);
        fopen(pCmdLine->output_redirect, O_RDWR | O_CREAT | O_APPEND, 777);
        }
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
            char * error=strerror(errno);
            perror(error);
            free_cmd_lines(pCmdLine);
            exit(0x55);
        }

    }
    if(pCmdLine->blocking==1){
        int status=0;
        waitpid(pid,&status,0);
    }
}

int main(int argc ,char** argv){

    
    while(1){
            char pathName[PATH_MAX];
            getcwd(pathName, PATH_MAX);
            printf( "%s: ",pathName);
            
            char input[INSize];
            fgets(input,INSize,stdin);
            input[strlen(input)-1]=0;
            if(strcmp (input, "quit")==0) {
                return 0;
            }
            
            cmd_line* result = parse_cmd_lines(input);
            if(result!=NULL){
                 execute(result);
                free_cmd_lines(result);
            }
    }
    return 0;
}
