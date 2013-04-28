
// tcpclient.cpp
// Rosen
// 2013-4-28

#include "network.h"

void str_cli(FILE *fp, int fd);

int main(int argc, char **argv)
{
	int sockfd = 0;
	struct sockaddr_in srvaddr;
	
	if (argc != 2)	
	{
		printf("usage: tcpclient <IPaddress>\n"	);
		return 0;
	}

	/* create socket  */
	sockfd = socket(AF_INET,SOCK_STREAM , 0);

	/* initialize */
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SERV_PORT);
	
	
	Inet_pton(AF_INET, argv[1], (void*)&srvaddr.sin_addr);

	connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr));

	str_cli(stdin, sockfd);	



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
	}
}
