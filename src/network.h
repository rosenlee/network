
// network.h
// Rosen
// 2013-4-28

#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/wait.h>


#define  LISTENQ 	1024
#define SERV_PORT  	10445
#define  MAXLINE 	4096


typedef void Sigfunc(int );

void str_echo(int connfd);
void err_sys(const char*, ...);
void err_ret(const char* fmt, ...);
FILE* Fdopen(int fd, const char *mode);
char *Fgets(char *ptr, int n, FILE * stream);
void Getpeername(int fd, struct sockaddr* sa, socklen_t *salenptr);
int Tcp_connect(const char* host, const char* srv);
char *Sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
void Fputs(const char* ptr , FILE *stream);
void Inet_pton(int family, const char *strptr, void *addrptr);
void sig_chld(int signo);

/* signal */
Sigfunc * signal(int signo, Sigfunc *func);
Sigfunc * Signal(int signo, Sigfunc *func);





#endif 
