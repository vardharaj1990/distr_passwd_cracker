#include "lsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include "lspmessage.pb-c.h"
#define NUM_PORTS 10
#define BUFLEN 1024
#define PORT 9930

int no_clients = 0;
pthread_t client_receive;
pthread_t server_receive;


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

unsigned short* get_port(struct sockaddr *sa)
{
   if (sa->sa_family == AF_INET) 
   {
        return &(((struct sockaddr_in*)sa)->sin_port);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_port);
}

int port_map[NUM_PORTS];
int sockfd;
int sock;
lsp_server* serv;
char* msg;
char *address;

double epoch_lth = _EPOCH_LTH;
int epoch_cnt = _EPOCH_CNT;
double drop_rate = _DROP_RATE;

/*
 *
 *
 *				LSP RELATED FUNCTIONS
 *
 *
 */  

void lsp_set_epoch_lth(double lth){epoch_lth = lth;}
void lsp_set_epoch_cnt(int cnt){epoch_cnt = cnt;}
void lsp_set_drop_rate(double rate){drop_rate = rate;}
void* client_receive_fn(void*);
void* server_receive_fn(void*);
/*
 *
 *
 *				CLIENT RELATED FUNCTIONS
 *
 *
 */  


lsp_client* lsp_client_create(const char* src, const char* port)
{
    printf("trace: Client Create...\n");
    lsp_client *client = (lsp_client *)malloc(sizeof(lsp_client));
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    char pld[] = "msg";
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    addr_len = sizeof their_addr;

    if ((rv = getaddrinfo(src, port, &hints, &servinfo)) != 0) 
    {
        printf("\ngetaddrinfo: %s", gai_strerror(rv));
        return NULL;
    }

    p = servinfo;
    
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)            
    	printf("\ntalker: socket");

    if (p == NULL) 
    {
        printf("\ntalker: failed to bind socket\n");
        return NULL;
    }
    client->sockfd = sockfd;
    client->servinfo = servinfo;
    client->ACK_RCVD = false;
    if ((numbytes = sendto(client->sockfd, pld, strlen(pld), 0, p->ai_addr, p->ai_addrlen)) == -1) 
    {      	printf("\ntalker: sendto");
        exit(1);
    }
    else
    {
    	printf("\nSent.. %d bytes..", numbytes);
    }
    	
    pthread_create(&client_receive, NULL, client_receive_fn, (void*) client);
    while(!client->ACK_RCVD);
    printf("\n Received ack from server with conn id %d", client->conn_id);
    return client;
}

void* client_receive_fn(void* client)
{
    lsp_client* a_client = (lsp_client*) client;
    int numbytes;
    char pld[] = "msg";
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    addr_len = sizeof their_addr;
    struct addrinfo *p = a_client->servinfo;
    for(;;)
    {
    	
    	if ((numbytes = recvfrom(a_client->sockfd, pld, BUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
    	{
        	printf("\nRecv from error at client");
       		exit(1);
    	}
    	printf("\nReceived from server %s", pld);
    	a_client->ACK_RCVD = true;
    	a_client->conn_id = atoi(pld);
    	//return (void*) a_client;
    	client = (void*) a_client;
    } 
}

void* server_receive_fn(void* server)
{
 	char s[INET6_ADDRSTRLEN];
 	lsp_server* a_srv = (lsp_server*) server;
    struct sockaddr_storage their_addr;
    int numbytes;
    char pld[BUFLEN];
    socklen_t addr_len;
    addr_len = sizeof their_addr;
    for(;;)
    {
    	if ((numbytes = recvfrom(a_srv->sockfd, pld, BUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
    	{
        	perror("rcv");
        	printf("\nRecv from error at server");
       		exit(1);
    	}
    	printf("\nReceived request from client.. %s", pld);
    	if(pld[0])
    	{
    		sprintf(pld, "%d", a_srv->next_conn_id);
    		if ((numbytes = sendto(a_srv->sockfd, pld, strlen(pld), 0, (struct sockaddr *)&their_addr, addr_len)) == -1) 
    		{
      			printf("\ntalker: sendto");
        		exit(1);
    		}
    		else
    		{
    			printf("\nSent.. %d bytes..", numbytes);
    			a_srv->next_conn_id++;
    		}
    	}
    }
}


/*
void client_receive_fn(void* arg)
{
	lsp_client* client = (lsp_client*) arg;
	
}

int lsp_client_read(lsp_client* a_client, char* pld)
{
	printf("trace: %d", a_client->sockfd);
    pld = (char*) malloc(4096);
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_storage their_addr;
    int numbytes;
    socklen_t addr_len;
    addr_len = sizeof their_addr;
	
    if ((numbytes = recvfrom(a_client->sockfd, pld, BUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
    {
        printf("recvfrom");
        exit(1);
    }
    
    printf("listener: got packet from %s\n",inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    printf("listener: packet contains \"%s\"\n", pld);
	
}
/*
bool lsp_client_write(lsp_client* a_client, uint8_t* pld, int lth)
{
	msg = (char*) pld;
	sock = a_client->sockfd;
}

bool lsp_client_write2(lsp_client* a_client, char* pld, int lth)
{
	struct addrinfo hints, *p;
    int rv;
    int numbytes;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo("127.0.0.1", a_client->port, &hints, &p)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

	if ((numbytes = sendto(a_client->sockfd, pld, strlen(pld), 0, p->ai_addr, p->ai_addrlen)) == -1) 
    {
        perror("talker: sendto");
        exit(1);
    }
    conn_id[num_clients] = a_client;
    num_clients++;
    printf("talker: sent %d bytes to %s\n", numbytes, pld);
}

bool lsp_client_close(lsp_client* a_client)
{
	close(a_client->sockfd);
}

/*
 *
 *
 *				SERVER RELATED FUNCTIONS
 *
 *
 */  


lsp_server* lsp_server_create(const char* port)
{    
    printf("trace: Server Create...\n");
    lsp_server* serv = (lsp_server *)malloc(sizeof(lsp_server));
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) 
    {
    	printf("trace:");
        printf("\nGetaddrinfo error");
        return NULL;
    }
    //printf("trace:");
    
    p = servinfo;
    
     if (p == NULL)
     {
        printf("\nlistener: failed to bind socket");
		return NULL;
	 }

    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
    	printf("\nlistener: socket");
    else
    	printf("\nSocket created with id: %d", sockfd);

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
    {
            close(sockfd);
            perror("\nlistener: bind");
            exit(1);
    }
    else
    	printf("\nBind successful!");
    serv->sockfd = sockfd;
    serv->next_conn_id = 0;
    pthread_create(&server_receive, NULL, server_receive_fn, (void*) serv);
    return serv;
}

/*
int lsp_server_read(lsp_server* a_srv, void* pld, uint32_t* conn_id)
{
    pld = (void*) msg;
    conn_id = sock;	
}


int lsp_server_read2(lsp_server* a_srv, char* pld, int* conn_id)
{
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_storage their_addr;
    int numbytes;
    char buf[BUFLEN];
    socklen_t addr_len;
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(a_srv->sockfd, pld, BUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
    {
        perror("recvfrom");
        exit(1);
    }
    //connap[no_clients] = (struct sockaddr *)&their_addr;
    //unsigned short* pport = get_port((struct sockaddr *)&their_addr);
    printf("listener: got packet from %s %u\n",inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr), s, sizeof s), *pport);
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", pld);
    conn_id = no_clients;
    conn_map[no_clients] = *pport;
    printf("\n port stored : %d", conn_map[no_clients]);
    no_clients++;
    address = (char*)malloc(20);
    strcpy(address, inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s,sizeof(s)));
}

bool lsp_server_write(lsp_server* a_srv, char* pld, int lth, uint32_t conn_id)
{
	struct addrinfo hints, *p;
    int rv;
    int numbytes;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
    //char* port_c = (char *) malloc(10);
    //sprintf(port_c, "%d", conn_map[conn_id]);
    //printf("\n port of client: %s\n", port_c);
    
    if ((rv = getaddrinfo("127.0.0.1", port_c, &hints, &p)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

	if ((numbytes = sendto(a_srv->sockfd, pld, strlen(pld), 0, conn_map[conn_id]->addr->ai_addr, conn_map[conn_id]->addr->ai_addrlen)) == -1) 
    {
        perror("talker: sendto");
        exit(1);
    }

    printf("\n p->ai_addr %d", p->ai_addr);
    printf("\ntalker: sent %d bytes to %s\n", numbytes, pld);
}

bool lsp_server_close(lsp_server* a_srv, uint32_t conn_id)
{
	close(a_srv->sockfd);
}	*/
