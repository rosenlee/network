
// datetimetcpcli.c
// Rosen
// 2013-5-12
// datetime tcp client

#include "network.h"


int main(int argc, char ** argv)
{
	int 		sockfd, n;
	char		recvline[MAXLINE + 1];
	socklen_t	len;
	struct sockaddr_storage ss;
	
	if (argc != 3)
	{
		err_sys("Usage: daytimetcpcli <hostname/IPaddress> <service/port#>"); 
	}
	
	sockfd = Tcp_connect(argv[1], argv[2]); 

	len = sizeof(ss);
	Getpeername(sockfd, (struct sockaddr*)&ss, &len);
	printf("connect to %s\n", Sock_ntop_host((struct sockaddr *)&ss, len));

	while((n = read(sockfd, recvline, MAXLINE)) > 0)
	{
		recvline[n] = 0;
		Fputs(recvline, stdout);
	}
	
	return 0;
}
