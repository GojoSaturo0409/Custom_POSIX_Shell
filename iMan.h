#ifndef IMAN_H
#define IMAN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_RESPONSE_LEN 1048576
struct hostent* resolve_dns(const char* hostname) ;
int connect_to_server(struct sockaddr_in* srv_addr, struct hostent* dns_info);
int send_http_request(int socket_fd, const char* search_term);
void receive_and_parse_response(int socket_fd) ;
void controller( char *argv[]);


#endif