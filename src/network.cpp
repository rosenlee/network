
// network.cpp
// Rosen
// 2013-4-8

// implementation of network.h


#include "network.h"

static void err_doit(int errnoflag, int level, const char* fmt, va_list ap);


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

void err_sys(const char* fmt, ...)
{
//	printf("%s\n", str);	
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR,fmt, ap);		
	va_end(ap);
	exit(1);
}

void err_ret(const char* fmt, ...)
{
//	printf("%s\n", str);	
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, LOG_INFO,fmt, ap);		
	va_end(ap);
	return;
}

static void err_doit(int errnoflag, int level, const char* fmt, va_list ap)
{
	int errno_save, n;
	char buf[MAXLINE + 1];
	
	errno_save = errno;
	
	vsnprintf(buf, MAXLINE, fmt, ap);
	
	n = strlen(buf);
	if(errnoflag)
	{
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
	}
	strcat(buf,"\n");
/*
	if (daemon_proc)
	{
		syslog(level, buf);
	}
	else */
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	return;
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


Sigfunc * signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;
	
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	if(SIGALRM == signo)
	{
		// nothing	
	}

	if(sigaction(signo, &act, &oact) < 0)
	{
		return (SIG_ERR);
	}
	
	return (oact.sa_handler);
}

Sigfunc * Signal(int signo, Sigfunc *func)
{
	Sigfunc *sigfunc;

	if ( (sigfunc = signal(signo, func)) == SIG_ERR)
	{
		err_sys("signal error");
	}	
	else
	{
		printf("register signal func succeed\n");
	}
	return (sigfunc);
}


void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	
	pid = wait(&stat);
	printf("child %d terminated\n", pid);

	return ;
}
