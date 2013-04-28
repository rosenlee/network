
// network.cpp
// Rosen
// 2013-4-8

// implementation of network.h


#include "network.h"

void str_echo(int connfd)
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
