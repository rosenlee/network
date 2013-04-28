
// filename: tcpserver.cpp
// Rosen
// 2013-4-28

#include <iostream>
#include <string>
#include <string.h>

#include "network.h"

using namespace std;

int main(int argc, char **argv)
{
	int listenfd,  /* create by socket() */
		connfd; /* created by connect()*/
	pid_t 	childpid;
	struct sockaddr_in srvaddr = {}; // server address
	struct sockaddr_in cliaddr = {}; // client address
	socklen_t clilen = 0;

	/* create a socket */
	listenfd  = socket(AF_INET,SOCK_STREAM,0);
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SERV_PORT);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	/* bind the ip and port to a socket */
	if (-1 == bind(listenfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)))
	{
		perror("bind failed");
		return -1;
	}

	listen(listenfd, LISTENQ);

	do
	{
		sleep(1);
		cout << "server is running" << endl;
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		if((childpid = fork()) == 0)
		{
			/* child process */
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd); /* parent process close its connect with client */
	}while(1);

	
	return 0;
}


