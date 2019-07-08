#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "line_parser.h"
#include "common.h"


#define PORT "2018" // the port client will be connecting to 
#define MAXDATASIZE 10000 // max number of bytes we can get at once 
#define MaxAddress 1000
#define Maxstdin 2000

//THE CLIENT
client_state client;
//THE Debug Flag
int dFlag;


/*typedef enum {
	IDLE,
	CONNECTING,
	CONNECTED,
	DOWNLOADING,
} c_state;
*/
/*typedef struct {
	char* server_addr;	// Address of the server as given in the [connect] command. "nil" if not connected to any server
	c_state conn_state;	// Current state of the client. Initially set to IDLE
	char* client_id;	// Client identification given by the server. NULL if not connected to a server.
	int sock_fd;		// The file descriptor used in send/recv
} client_state;
*/
/*struct addrinfo {
      int              ai_flags;
      int              ai_family;
      int              ai_socktype;
      int              ai_protocol;
      socklen_t        ai_addrlen;
      struct sockaddr *ai_addr;
      char            *ai_canonname;
      struct addrinfo *ai_next;
  };
*/
/*typedef struct cmd_line{
      char * const arguments[MAX_ARGUMENTS];  // command line arguments (arg 0 is the command)
      int arg_count;                          // number of arguments 
      char const *input_redirect;             // input redirection path. NULL if no input redirection 
      char const *output_redirect;            // output redirection path. NULL if no output redirection 
      char blocking;                          // boolean indicating blocking/non-blocking 
      int idx;                                // index of current command in the chain of cmd_lines (0 for the first) 
      struct cmd_line *next;                  // next cmd_line in chain 
  } cmd_line;
*/


void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int exec(cmd_line* cmd){
    int rv;
    int numbytes;
    char buf[MAXDATASIZE];
    int i=0;
    int sockfd=-1;
    char s[INET6_ADDRSTRLEN];
    if(cmd==NULL) 
        return -1;
    if(strcmp(cmd->arguments[0],"conn")==0 ||strcmp(cmd->arguments[0],"bye")==0 || (strcmp(cmd->arguments[0],"ls")==0)) {
        if(strcmp(cmd->arguments[0],"conn")==0 && cmd->arg_count==2){
            client.server_addr= cmd->arguments[1];
            
            
            struct addrinfo hints, *servinfo, *p;
            
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            
            if ((rv = getaddrinfo(cmd->arguments[1], PORT, &hints, &servinfo)) != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return 1;
            }

            // loop through all the results and connect to the first we can
            //this part is to chck for all the possiblistists to connect to server
            for(p = servinfo; p != NULL; p = p->ai_next) {
                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                    perror("client: socket");
                    continue;
                }
                if(client.conn_state==IDLE){
                    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                        close(sockfd);
                        perror("client: connect");
                        continue;
                    }
                }
                else{
                    return -2;
                }
                break;
            }
            //this part to chack if we success to connect
            if (p == NULL) {
                fprintf(stderr, "client: failed to connect\n");
                return 2;
            }
            
            inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
            printf("client: connecting to %s\n", s);
            
            freeaddrinfo(servinfo); // all done with this structure
            send(sockfd,"hello",6,0); //sending "hello to the server by sodkfd"
            client.conn_state=CONNECTING;
            if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) <7) {  //getting the numbytes that redieved from the server if it's under 6 procacly without the string""
                perror("recv");
            }
            if(dFlag) fprintf(stderr, "stderr: %s|Log: %s \n", client.server_addr, buf);
            client.client_id= &buf[6];
            client.conn_state=CONNECTED;
            client.sock_fd=sockfd;
            buf[numbytes] = '\0';
            printf("client: received '%s'\n",buf);

         //   return 0;
        }
        if(strcmp(cmd->arguments[0],"bye")==0 && cmd->arg_count==1){
            if(client.conn_state==CONNECTED){
                sockfd= client.sock_fd;
                send(sockfd,"bye",4,0);
                if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0))<3)
                    perror("recv");
                buf[numbytes] = '\0';
                printf("client: received '%s'\n",buf);
                initiallClient(&client);
                close(sockfd);
                return 0;
            }
            else{
                return -2;
            }
        }
        if(strcmp(cmd->arguments[0],"ls")==0){
        if(client.conn_state==CONNECTED){
          sockfd=client.sock_fd;
            send(sockfd,"ls",3,0);

        if ((numbytes = recv(sockfd, buf, 3, 0)) ==-1) {  //getting the numbytes that redieved from the server if it's under 6 procacly without the string""    
            perror("recv");
            }
             buf[numbytes]=0;
        if(strcmp(buf,"ok")==0){
            if ((numbytes = recv(sockfd, buf,  MAXDATASIZE-1, 0)) ==-1) {  
                perror("recv");
            }
            buf[numbytes]=0;
        cmd_line *cmd=parse_cmd_lines(buf);
        for(i=0;i<cmd->arg_count;i++){
            printf("%s\n", cmd->arguments[i]);
        }
        }
       
       
        if(strcmp(buf,"nok")==0){//TODO BOOM
//        client.
        }        

        }
        else{
            return -2;
        }
    }
    }

    return -1;
}


void beforeEnd(){
    //TODO: to close all the client and server
}

int main(int argc, char *argv[]){
    if (argc>1 && strcmp(argv[1], "-d")==0){
        dFlag=1;
    }
    initiallClient(&client);
    while(1){
        char buf[Maxstdin];
        memset(buf, 0, Maxstdin);
        
        fprintf(stdout, "server:%s>\n", client.server_addr);
        fgets(buf, Maxstdin, stdin);
        if(strcmp(buf, "quit\n")==0) 
            break;
        cmd_line* cmd;
        cmd = parse_cmd_lines(buf);
        
        exec(cmd);
    }
    beforeEnd();
    return 0;
}