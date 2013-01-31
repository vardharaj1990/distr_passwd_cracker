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

int 
main (int argc, char *argv[])
{
	 printf("This is the client program\n");
 
    int sockfd;
    int len, rc ;
    struct sockaddr_in address;
    int result;
    char ch = 'A';
 
   //Create socket for client.
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        perror("Socket create failed.\n") ; 
        return -1 ; 
    } 
     
    //Name the socket as agreed with server.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(7734);
    len = sizeof(address);
 
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1)
    {
        perror("Error has occurred");
        exit(-1);
    }
 
    while ( ch < 'B') {
 
        //Read and write via sockfd
        rc = write(sockfd, &ch, 1);
        printf("write rc = &#37;d\n", rc ) ; 
        if (rc == -1) break ; 
         
        read(sockfd, &ch, 1);
        printf("Char from server = %c\n", ch);
        break;
        //if (ch == 'A') sleep(5) ;  // pause 5 seconds 
    } 
    close(sockfd);
 
    exit(0);	
	return 0;
}




