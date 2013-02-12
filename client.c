#include "lsp.h"

/* 
request.c
Created 1/30/2013 6:49 PM
Authors : Vardharaj and Sindhuja

Client side that sends cracking requests to the server
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

               
int main(int argc, char** argv) 
{
	lsp_client* myclient = lsp_client_create("127.0.0.1", argv[1]);
	//printf("Socket created, %d\n", myclient->sockfd);
	/*
	char* message = "ilovethiscoursealready";
	

	//lsp_client_write(myclient, (void *) message, strlen(message));
	lsp_client_write2(myclient, message, strlen(message));
	char buffer[4096];
	int bytes_read = lsp_client_read(myclient, buffer);
	
	//puts(buffer);
	
	//lsp_client_close(myclient);
	//sleep(10);
	printf("End of client code\n");
	return 0;*/
}

