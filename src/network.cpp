
// network.cpp
// Rosen
// 2013-4-8

// implementation of network.h


#include "network.h"

void str_echo_v2(int connfd)
{
	char line[MAXLINE];
	FILE *fpin, *fpout;
	
	fpin = Fdopen(connfd, "r");
	fpout = Fdopen(connfd, "w");
	while(Fgets(line,MAXLINE,fpin) != NULL)
	{
		Fputs(line,fpout);
	}
}

void str_echo (int sockfd)
{
	ssize_t n;
	char line[MAXLINE];
	do 
	{
		if ((n = read(sockfd,line,MAXLINE)) == -1)
		{
			printf("read line failed\n");
			return;
		}
		write(sockfd, line, n);
	} while(1);
}
void err_sys(const char* str, ...)
{
	printf("%s\n", str);	
	exit(0);
}

FILE* Fdopen(int fd, const char *mode)
{
	FILE *fp;

	if ( (fp = fdopen(fd, mode)) == NULL )
	{
		err_sys("fdopen error");
	}

	return (fp);
}

char *Fgets(char *ptr, int n, FILE * stream)
{
	char *rptr = NULL;
	
	if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
	{
		err_sys("fgets error");
	}
	
	return (rptr);
}

void Fputs(const char* ptr , FILE *stream)
{
	if( fputs(ptr, stream) == EOF)
	{
		err_sys("fput error");
	}
}

void Inet_pton(int family, const char *strptr, void *addrptr)
{
	int n;
	
	if ( (n = inet_pton(family, strptr, addrptr)) < 0)
	{
		err_sys("inet_pton error for %s", strptr);
	}
	else if( n == 0)
	{
		err_sys("inet_pton error for %s", strptr);
	}
}
