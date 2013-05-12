
// tcpConnect.cpp
// Rosen
// 2013-5-9

// use getaddrinfo() 

#include "network.h"

int tcp_connect(const char* host, const char* srv)
{
	int sockfd, n;
	struct addrinfo hints, *res, *saveres;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((n = getaddrinfo(host, srv, &hints, &res) != 0))
	{
		err_sys("tcp_connect failed for %s, %s: %s", host,srv, gai_strerror(n));
	}

	saveres = res;
	
	do
	{
		printf("family:%d, socktype:%d, protocol: %d\n",res->ai_family,res->ai_socktype, res->ai_protocol);
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if( sockfd < 0)
			continue;
	
		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;

		close(sockfd);	
	}while((res = res->ai_next) != NULL);

	if (NULL == res)
	{
		err_sys("tcp-connect error for %s, %s", host, srv);
	}

	freeaddrinfo(saveres);

	return (sockfd)	;
} /* end of tcp_connect */

int Tcp_connect(const char* host, const char* srv)
{
	return 	tcp_connect(host, srv);
}
