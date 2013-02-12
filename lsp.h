#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <strings.h>
#include <errno.h>
#include <string.h>

// Global Parameters. For both server and clients.

#define _EPOCH_LTH 2.0
#define _EPOCH_CNT 5;
#define _DROP_RATE 0.0;

void lsp_set_epoch_lth(double lth);
void lsp_set_epoch_cnt(int cnt);
void lsp_set_drop_rate(double rate);


typedef struct lsp_client{
	int sockfd;
	int conn_id;
	int last_seq_id;
	bool ACK_RCVD;
	struct addrinfo* servinfo;	
	
}lsp_client;

lsp_client* lsp_client_create(const char* dest, const char* port);
int lsp_client_read(lsp_client* a_client, char* pld);
bool lsp_client_write(lsp_client* a_client, uint8_t* pld, int lth);
bool lsp_client_close(lsp_client* a_client);


typedef struct client_info
{
    int sockfd;
	int conn_id;
	int last_seq_id;
	bool ACK_RCVD;
	struct addrinfo* servinfo;
	
} client_info;

typedef struct lsp_server
{
	int sockfd;
    int next_conn_id;
    client_info conn_map[50];
} lsp_server;

int lsp_server_read2(lsp_server* a_srv, char* pld, int* conn_id);
lsp_server* lsp_server_create(const char* port);
int  lsp_server_read(lsp_server* a_srv, void* pld, uint32_t* conn_id);
bool lsp_server_write(lsp_server* a_srv, char* pld, int lth, uint32_t conn_id);
bool lsp_server_close(lsp_server* a_srv, uint32_t conn_id);
