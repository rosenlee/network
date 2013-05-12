
// wrapsock.cpp
// Rosen
// 2013-5-12

#include "network.h"


void Getpeername(int fd, struct sockaddr* sa, socklen_t *salenptr)
{
	if (getpeername(fd, sa, salenptr) < 0)
	{
		err_sys("getpeername error");
	}
}

char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
	static char str[128];
	
	switch (sa->sa_family)
	{
	case AF_INET:
	{
		struct sockaddr_in *sin = (struct sockaddr_in*)sa;
		
		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
		{
			return (NULL);
		}
		return (str);
	}
	
	break;
	case AF_INET6:
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)sa;
		
		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
		{
			return (NULL);
		}
		return (str);
	}
	break;
	case AF_UNIX:
	default:
		snprintf(str, sizeof(str), "sock_ntop_host: unkown AF_XXX: %d, len %d");
		return (str);

	} 

	return (NULL);
}
char *Sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
	char *ptr = NULL;

	if ((ptr = sock_ntop_host(sa, salen)) == NULL)
	{
		err_sys("sock_ntop_host error");
	}
	
	return (ptr);
}
