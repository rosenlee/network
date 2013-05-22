
// tcpclient.cpp
// Rosen
// 2013-4-28

#include "network.h"

#define SOCKFD_MAX 255

void str_cli(FILE *fp, int fd);

void cli_sendMsg(int fd);

int sockfdArray[SOCKFD_MAX] = {};

int main(int argc, char **argv)
{
//	int sockfd = 0;
	struct sockaddr_in srvaddr;
	
	if (argc != 2)	
	{
		printf("usage: tcpclient <IPaddress>\n"	);
		return 0;
	}

	for( int i = 0; i < SOCKFD_MAX; i++)
	{ 

	pid_t child;
	if ((child = fork()) == 0)
	{
		// it is child 
	/* create socket  */
	sockfdArray[i] = socket(AF_INET,SOCK_STREAM , 0);

	/* initialize */
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SERV_PORT);
	
	
	Inet_pton(AF_INET, argv[1], (void*)&srvaddr.sin_addr);

	if( -1 == connect(sockfdArray[i], (struct sockaddr *)&srvaddr, sizeof(srvaddr)))
	{
	     err_sys("connect failed! @ - %d\n",i);
	}

	//str_cli(stdin, sockfd);	
	    cli_sendMsg(sockfdArray[i]);
		
	}
	printf("child %d birth: %d\n",i, child);
	// parent do not do anything, just exit

	}


	return 0;
}

void str_cli(FILE *fp, int fd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	
	while(Fgets(sendline, MAXLINE, fp) != NULL)
	{
		write(fd, sendline,strlen(sendline));

		if (read(fd,recvline, MAXLINE) == -1)
		{
			err_sys("str_cli: server terminated permaturely");
		}
		Fputs(recvline,stdout);
		memset(recvline, 0x00, MAXLINE); // reset
	}
}


void cli_sendMsg(int fd)
{
	char sendMsg[MAXLINE] = {};
	char recvline[MAXLINE] = {};

	sprintf(sendMsg, "%d %s", fd ,"  I am client : \n" );

	while(true)
	{
		write(fd, sendMsg, strlen(sendMsg));
		if (read(fd, recvline, MAXLINE) == -1)
		{
			err_sys("cli_sendMsg : server terminated permaturely");
		}
		Fputs(recvline, stdout);
		memset(recvline, 0x00, MAXLINE);
	}
}
