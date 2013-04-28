
// network.h
// Rosen
// 2013-4-28

#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include <arpa/inet.h>


#define  LISTENQ 	1024
#define SERV_PORT  	9998
#define  MAXLINE 	4096



void str_echo(int connfd);
void err_sys(const char*, ...);
FILE* Fdopen(int fd, const char *mode);
char *Fgets(char *ptr, int n, FILE * stream);
void Fputs(const char* ptr , FILE *stream);







#endif 
