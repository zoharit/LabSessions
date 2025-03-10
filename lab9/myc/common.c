//CLIENT COMMON
//CLIENT COMMON
//CLIENT COMMON
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include "common.h"

static char* _list_dir(DIR * dir, int len) {
	struct dirent * ent = readdir(dir);
	struct stat info;

	if (ent == NULL) {
		char * listing = malloc((len + 1) * sizeof(char));
		listing[len] = '\0';
		return listing;
	}
	if (stat(ent->d_name, &info) < 0) {
		perror("stat");
		return NULL;
	}
	
	int ent_len = 0;
	if(S_ISREG(info.st_mode)){
		ent_len = strlen(ent->d_name);
		ent_len++; /* Account for the \n */
	}
	
	char * listing = _list_dir(dir, len + ent_len);	
	
	if(S_ISREG(info.st_mode)){	
		strcpy(&listing[len], ent->d_name);
		listing[len + ent_len - 1] = '\n';
	}
	return listing;
}

int file_size(char * filename){
	long filesize = -1;
	FILE * file;
	file = fopen (filename, "r");
	if (file == NULL){
		perror("fopen");
		goto err;
	}
	
	if(fseek (file, 0, SEEK_END) != 0){
		perror("fseek");
		goto err;
	}
	if((filesize = ftell(file)) < 0){
		perror("ftell");
		goto err;
	}		
	
	fclose(file);
	
	return filesize;
	
	err:
	if (file) {
		fclose(file);
	}
	return -1;
}

//@return must be freed by the caller
char* list_dir(){
	DIR *dir = opendir("."); 
	if (dir == NULL){
		perror("opendir");
		return NULL;
	}
	
	char * listing = _list_dir(dir, 0);
	closedir(dir);
	return listing;
} 


void initiallClient(client_state *client){
    client->conn_state=IDLE;         //the client's state is IDLE,
    client->server_addr= "nil";       //the server address is gethostname(--),
    client->client_id=NULL;          //the client's Iis NULL,
    client->sock_fd= -1;             //the socket file descriptor is -1.
}

void printClient(client_state *client){
    printf ("\033[32;1mClient info: \033[0m\n");
    if(client->conn_state==IDLE)        printf("conn_state: \tIDLE\n");
    if(client->conn_state==CONNECTING)  printf("conn_state: \tCONNECTING\n");
    if(client->conn_state==CONNECTED)   printf("conn_state: \tCONNECTED\n");
    if(client->conn_state==DOWNLOADING) printf("conn_state: \tDOWNLOADING\n");
    if(client->conn_state==SIZE)        printf("conn_state: \tSIZE\n");
    printf("server_addr: \t%s\n", client->server_addr);
    printf("client_id: \t%s\n", client->client_id);
    printf("sock_fd: \t%d\n", client->sock_fd);
}
