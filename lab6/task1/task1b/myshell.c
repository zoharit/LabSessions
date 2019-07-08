
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
#include "job_control.h"


void execute(cmd_line *pCmdLine,int *pipe_l,int* pipe_r){   
    pid_t pid;
    pid = fork();
    if (pid==-1){
    perror("fork error");
    exit(0x55);
    }
    else if (pid==0){
            if(pipe_r!=NULL){
            dup2(pipe_r[1],1);
            close (pipe_r[1]);
            close (pipe_r[0]);
        }
        else if(pipe_l!=NULL){
        dup2(pipe_l[0],0);
        close(pipe_l[0]);
        close(pipe_l[1]);
        }  
       
         if(pCmdLine->input_redirect){
        close(0);
        open(pCmdLine->input_redirect, O_RDONLY );
        }
        if(pCmdLine->output_redirect){
        close(1);
        open(pCmdLine->output_redirect, O_RDWR | O_CREAT, 00777);
        } 
         if (execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
            perror("execvp error");
            exit(0x55);
        }      
    }
    else{   
        if(pipe_r!=NULL)
        {
        close (pipe_r[1]);
        }
        if(pipe_l!=NULL)
        {
        close (pipe_l[0]);
        free(pipe_l);  
        }
        waitpid(pid, NULL, 0); 
   if(pCmdLine->blocking==1){
        int status=0;
        waitpid(pid,&status,0);
    }
        if (pCmdLine->next!=NULL){
            if(pCmdLine->next->next!=NULL){
                
            int *new_pipe_l=malloc(2*sizeof(int));
            pipe(new_pipe_l);
            execute(pCmdLine->next,pipe_r,new_pipe_l);
        }
        else{
            execute(pCmdLine->next, pipe_r, NULL);
        }
    }
}
}

void handleSignal(int signalNum){
    printf("Ignoring this signal: %s\n",strsignal(signalNum));
}
int main(int argc ,char** argv){
    job* job_list = NULL; 
    job ** list_jobs=&job_list;

    while(1){
        signal(SIGQUIT, handleSignal); 
        signal(SIGTSTP, handleSignal);     
        signal(SIGCHLD, handleSignal);
            char pathName[PATH_MAX];
            getcwd(pathName, PATH_MAX);
            printf( "%s: ",pathName);
            
            char input[INSize];
            fgets(input,INSize,stdin);
            input[strlen(input)-1]=0;
            if (strcmp(input,"jobs\0")==0){
            print_jobs(list_jobs);
            }
            else{
            if(strcmp (input, "quit")==0) {
                return 0; 
            }
            cmd_line* result = parse_cmd_lines(input);
            if(result!=NULL){
                job* next_job=add_job(list_jobs, input);
            if(result->next==NULL){
                execute(result, NULL, NULL);
            }
            if (result->next!=NULL){
                int *pipe_r=malloc(2*sizeof(int));
                pipe(pipe_r);
                 execute(result,NULL,pipe_r);
            }  } 
                free_cmd_lines(result);
            }
    }
    return 0;
}
