
#define INSize 2048
#define SUSPENDED 0

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

int pipeArr[2];

void execute(cmd_line *pCmdLine){
    if (pCmdLine->next!=NULL)
        pipe(pipeArr);
    
    pid_t pid;
    pid = fork();
    if (pid==-1){
    perror("fork error");
    exit(0x55);
    }
    else if (pid==0){ 
        if(pCmdLine->input_redirect){
        close(0);
        open(pCmdLine->input_redirect, O_RDONLY );
        }
        if(pCmdLine->output_redirect){
        close(1);
        open(pCmdLine->output_redirect, O_RDWR | O_CREAT, 00777);
        }

        if (pCmdLine->next!=NULL){
            close(1); 
            dup(pipeArr[1]);
            close (pipeArr[1]);
            close (pipeArr[0]);
        }  
        int retVal=1;

        retVal=execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        if (retVal==-1){
            perror("execvp error");
            exit(0x55);
        }     
    }
     if(pCmdLine->blocking==1){
        int status=0;
        waitpid(pid,&status,0);
    }
    else{
        if (pCmdLine->next!=NULL){
            close(pipeArr[1]);  
            pid_t pid2;
    pid2 = fork(); 
    if (pid2==-1){
        perror("fork error");
        exit(0x55);
    }
    else if (pid2==0) {
        if(pCmdLine->next->input_redirect){
        close(0);
        open(pCmdLine->next->input_redirect, O_RDONLY );
        }
        if(pCmdLine->next->output_redirect){
        close(1);
        open(pCmdLine->next->output_redirect, O_RDWR | O_CREAT, 00777);
        }
     
        close (0);
        dup (pipeArr[0]);
        close (pipeArr[0]);  
        int retVal=1;
        
        retVal=execvp(pCmdLine->next->arguments[0],pCmdLine->next->arguments);
        if (retVal==-1){
            perror("execvp error");
            exit(0x55);
        }    
    }
    else { 
        close (pipeArr[0]);
        int status2;
        waitpid (pid2,&status2,WUNTRACED);
    }
            
        }
}}


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
