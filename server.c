/* 
server.c
Created 1/30/2013 6:49 PM
Authors : Vardharaj and Sindhuja

Server that handles cracking requests, assigns the requests to workers
and sends the cracked password back to the requesting client
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int 
main (int argc, char *argv[])
{
	//Declaring process variables.
    int server_sockfd, client_sockfd;
    int server_len ; 
    int rc ; 
    unsigned client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
 
    //Remove any old socket and create an unnamed socket for the server.
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(7734) ; 
    server_len = sizeof(server_address);
 
    rc = bind(server_sockfd, (struct sockaddr *) &server_address, server_len);
    if (rc == -1)
    {
    	printf("Bind Error\n");
    	exit(0);
    }
    printf("RC from bind = %d\n", rc ) ; 
     
    //Create a connection queue and wait for clients
    rc = listen(server_sockfd, 5);
    
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);

    while(1)
    {
        char ch;
        printf("server waiting\n");
 
        //Accept a connection
        //client_len = sizeof(client_address);
        //client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
        //printf("after accept()... client_sockfd = %d\n", client_sockfd) ; 
        //Read write to client on client_sockfd
 
        rc = read(client_sockfd, &ch, 1);
        printf("RC from read = %d\n", rc ) ;        
        //if (ch=='A') break ; 
        //ch++;
        printf("Read from client %c\n", ch++);
        write(client_sockfd, &ch, 1);
        break;
    }
 
    printf("server exiting\n");
 
    //close(client_sockfd);
    close(client_sockfd);
    return 0;	
	return 0;
}

 
