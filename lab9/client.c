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


#define PORT "3490" // the port client will be connecting to 
#define MAXDATASIZE 100 // max number of bytes we can get at once 
#define MaxAddress 1000
#define Maxstdin 2000

typedef enum {
	IDLE,
	CONNECTING,
	CONNECTED,
	DOWNLOADING,
} c_state;
	
typedef struct {
	char* server_addr;	// Address of the server as given in the [connect] command. "nil" if not connected to any server
	c_state conn_state;	// Current state of the client. Initially set to IDLE
	char* client_id;	// Client identification given by the server. NULL if not connected to a server.
	int sock_fd;		// The file descriptor used in send/recv
} client_state;

client_state client;

/*addrinfo
  struct addrinfo {
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
/*cmd_line
  typedef struct cmd_line
  {
      char * const arguments[MAX_ARGUMENTS];  // command line arguments (arg 0 is the command)
      int arg_count;                          // number of arguments 
      char const *input_redirect;             // input redirection path. NULL if no input redirection 
      char const *output_redirect;            // output redirection path. NULL if no output redirection 
      char blocking;                          // boolean indicating blocking/non-blocking 
      int idx;                                // index of current command in the chain of cmd_lines (0 for the first) 
      struct cmd_line *next;                  // next cmd_line in chain 
  } cmd_line;
*/


int exec(cmd_line* cmd){
    if(cmd==NULL) return -1;
    if(strcmp(cmd->arguments[0],"conn")==0 && cmd->arg_count==2){
        int sockfd, numbytes;  
        char buf[MAXDATASIZE];
        struct addrinfo hints, *servinfo, *p;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        int rv;
        if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }

        // loop through all the results and connect to the first we can
        //this part is to chck for all the possiblistists to connect to server
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                    p->ai_protocol)) == -1) {
                perror("client: socket");
                continue;
            }
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("client: connect");
                continue;
            }
            break;
        }
        //this part to chack if we success to connect
        if (p == NULL) {
            fprintf(stderr, "client: failed to connect\n");
            return 2;
        }

        char *s = malloc(INET6_ADDRSTRLEN);
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
        printf("client: connecting to %s\n", s);

        freeaddrinfo(servinfo); // all done with this structure

        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';

        printf("client: received '%s'\n",buf);

    }
    
    
}

void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]){ //argv[1] is the "www.google.com" or IP

    client.server_addr= "nil";
    client.client_id=NULL;
    client.conn_state=IDLE;
    client.sock_fd= -1;
    
    while(1){
        char buf[Maxstdin];
        memset(buf, 0, Maxstdin);
        fprintf(stdout, "server:%s>\n", client.server_addr);
        fgets(buf, Maxstdin, stdin);
        cmd_line* cmd;
        cmd = parse_cmd_lines(buf);
        exec(cmd);
    }

    return 0;
}