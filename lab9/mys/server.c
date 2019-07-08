#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "common.h"

#define PORT "2018"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold
#define NAMESIZE 255
#define MAXDATASIZE 10000
//THE CLIENT
int dFlag;

client_state client;
int counter(){ static int counter =0; return counter++;}



// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    
     if(argc>1 && strcmp(argv[1], "-d")==0){
        dFlag=1;
    }
    while(1) { 
         char listdir[2048];
    char * name= malloc(NAMESIZE);
    gethostname(name, NAMESIZE);
    initiallClient(&client);
    client.server_addr=name;
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;

    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);
    
    printf("server: waiting for connections...\n");
 // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
        while(1){
        int numbytes;
        //int oldNumbytes=-1;
        char buf[MAXDATASIZE];
        numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0);
        if(numbytes!=-1){
            char* nameString=malloc(11);
            buf[numbytes]=0;
            //oldNumbytes=numbytes; 
            //printf("server recv numbytes: %d\n", numbytes);       
            //printf("server recv oldNumbytes: %d\n", oldNumbytes);
            //printf("server recv buf: %s\n", buf);
            
            //hello case
            if (numbytes==6 && strcmp("hello",buf)==0){
                printf("server line hello case\n");
                client.conn_state=CONNECTED;
                sprintf(nameString, "hello %d", counter());
                client.client_id=nameString; 
                if (send(new_fd, nameString , 11, 0) == -1)
                    perror("send");
            }
            if(dFlag) fprintf(stderr, "%s|Log: %s\n", client.server_addr, buf);
            if( strcmp("ls",buf)==0){
                printf("server line ls case\n");
                client.conn_state=IDLE;
                 printf("ls\n");
                   if(send(new_fd, "ok" , sizeof("ok"), 0) == -1){
                       perror("send");}
                else{
                   
                    strcpy(listdir,list_dir());
                    client.conn_state=CONNECTED;
                    sprintf(nameString, "list dir %d", counter());
                    client.client_id=listdir; 
                    if (send(new_fd, listdir , sizeof(listdir), 0) == -1)
                    perror("send");
                }
            }
            
            //bye case
            if (numbytes==4 && strcmp("bye",buf)==0){
                printf("server line bye case\n");
                client.conn_state=IDLE;
                printf("bye\n");
                if (send(new_fd, "bye" , 3, 0) == -1)
                perror("send");
                close(new_fd);
                close(sockfd); 
                free(nameString);
                break;
            }
        }
        }
    }
    return 0;
}