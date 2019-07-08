#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <unistd.h>


int pipeArr[2];

int main(int argc, char* argv[]){
    pipe(pipeArr);
    pid_t pid;
    pid = fork();
        if (pid==-1){
        perror("fork error");
        exit(0x55);
    }
    else if (pid==0){
    char* argv[] = {"ls","-l",0};
    close(1); 

    dup(pipeArr[1]);
    close (pipeArr[1]); 
    execvp(argv[0],argv);
}
    else{
    close(pipeArr[1]); 
    int status;
    pid_t pid2;

    pid2 = fork();
    if (pid2==-1){
        perror("fork error");
        exit(0x55);
    }
    else if (pid2==0) { 
        char* argv2[] = {"tail","-n","2",0};
        close(0);
        dup (pipeArr[0]); 
        close (pipeArr[0]);
        execvp(argv2[0],argv2);
    }
    else { 

        close (pipeArr[0]); 

        int status2;
        waitpid (pid,&status,WUNTRACED); 
        waitpid (pid2,&status2,WUNTRACED);
    }

}

    return 0;
}
