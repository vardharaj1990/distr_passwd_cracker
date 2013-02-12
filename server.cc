
#include "lsp.h"

/* 
server.c
Created 1/30/2013 6:49 PM
Authors : Vardharaj and Sindhuja

Server that handles cracking requests, assigns the requests to workers
and sends the cracked password back to the requesting client
*/
 

#include <cstdio>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;
   

int main(int argc, char** argv) 
{
	printf("Server...\n");
	lsp_server* myserver = lsp_server_create(argv[1]);
	printf("socket created, %d\n", myserver->sockfd);
	for(;;);
	/*uint8_t payload[4096];
	uint32_t returned_id;
	int returned_id2;
	int bytes_read;
	char payload2[1000];
	for(;;)
	{
		printf("Issuing read\n");
		int bytes = lsp_server_read2(myserver, payload2, &returned_id2);
		//sleep(5);
		//	Echo it right back
		lsp_server_write(myserver, payload2, sizeof(payload2), returned_id2);
		//if(payload2[0] != '\0') break; 
	}*/
	
	return 0;
}

