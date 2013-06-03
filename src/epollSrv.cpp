
/// epollSrv.cpp
// Rosen
// 2013-5-31
// all source from sparkling


#include "network.h"

int gEpollFd;
myevent_s gEvents[MAX_EVENTS + 1]; 

void RecvData(int fd, int events, void *arg);
void SendData(int fd, int events, void *arg);

// accept new connect from clients
void AcceptConn(int fd, int events, void *arg)
{
	struct sockaddr_in sin;
	socklen_t len  = sizeof(struct sockaddr_in);
	int nfd, i = 0;

	// accept 
	if ((nfd = accept(fd, (struct sockaddr*)&sin, &len )) == -1)
	{
		if(errno != EAGAIN && errno != EINTR)
		{
		//todo:	
		}
		err_sys("%s; accept , %d", __func__, errno);
	}

	do
	{
		// find a unused position for new connect
		for(i = 0; i < MAX_EVENTS; i++)
		{
			if(0 == gEvents[i].status )
			{
				break;
			}
		}
		
		if (i == MAX_EVENTS)
		{
			err_ret("%s: max connection limit[%d]. ", __func__, MAX_EVENTS);
			//err_sys("%s: max connection limit[%d]. ", __func__, MAX_EVENTS);
		}

		int iret = 0;
		if((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0)
		{
			err_sys("%s: fcntl nonblocking failed: %d", __func__, iret);
		}

		EventSet(&gEvents[i], nfd, RecvData, &gEvents[i]);
		EventAdd(gEpollFd, EPOLLIN, &gEvents[i]);

	}while(0);
	printf("new connection[%s:%d][time:%ld], pos[%d]\n",
		inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), gEvents[i].last_active,i);
	
}

void RecvData(int fd, int events, void *arg)
{
	struct myevent_s *ev = (struct myevent_s *)arg;

	int len;
	
	memset(ev->buff, 0, sizeof(ev->buff));
	ev->len = 0;
	len = recv(fd, ev->buff, sizeof(ev->buff)-1, 0);
	EventDel(gEpollFd, ev);

	if(len > 0)
	{
		ev->len += len;
		ev->buff[len] = '\0';
		printf("C[%d]:%s, len:%d\n", fd, ev->buff,len);
		
		EventSet(ev, fd, SendData, ev);
		EventAdd(gEpollFd, EPOLLOUT, ev);

		ev->len += len;
		ev->buff[len] = '\0';
		printf("C[%d]:%s, len:%d - after\n", fd, ev->buff,len);

	}
	else if (0 == len)
	{
		close(ev->fd);
		printf("[fd=%d] pos[%d], closed gracefully.\n ", fd, events);
	}
	else
	{
		close(ev->fd);
		printf("recv[fd=%d] error[%d] :%s\n", fd, errno, strerror(errno));
	}
}

void SendData(int fd, int events, void *arg)
{
	struct myevent_s *ev  = (struct myevent_s*) arg;
	int len;
	char msg[] = "i am server";

	printf("ready to send data - %d, msg : %s-> len:%d\n", fd, ev->buff, ev->len);	
	// send data
	len = send(fd, ev->buff, ev->len, 0);
	//len = send(fd, msg, strlen(msg)+1, 0);

	if(len > 0)
	{
		printf("send[fd=%d], [%d<->%d] %s\n", fd, len, ev->len,ev->buff);
		ev->s_offset += len;
		if(ev->s_offset == ev->len)
		{
			// change to receive event 
		}
	}
	else
	{
		err_ret("send msg failed @ %d", fd);
	}
	
}

bool InitListenSocket(int epollFd, short port)
{
	int listenFd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(listenFd, F_SETFL, O_NONBLOCK );
	printf("server listen fd=%d\n", listenFd);
	EventSet(&gEvents[MAX_EVENTS], listenFd, AcceptConn, &gEvents[MAX_EVENTS]);
	
	// add listen socket
	EventAdd(epollFd, EPOLLIN, &gEvents[MAX_EVENTS]);
	
	// bind & listen 

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);

	bind(listenFd, (const sockaddr*)&sin, sizeof(sin));

	listen(listenFd, 5);
	return true;
}

int main(int argc, char **argv)
{
	unsigned short port = 12345; // default port
	
	if(argc == 2)
	{
		port = atoi (argv[1]);
	}

	// create epoll
	gEpollFd = epoll_create(MAX_EVENTS);
	if(gEpollFd < 0)
	{
		err_sys("create epoll failed.%d\n", gEpollFd);
	}
	
	InitListenSocket(gEpollFd, port);

	struct epoll_event events[MAX_EVENTS];
	printf("server running : port[%d]\n",port);
	int checkPos = 0;
	while(1)
	{
		// a simple time out check here, every time 100
		long now = time(NULL);
		for(int i = 0; i < 100; i++, checkPos++)
		{
			if(checkPos == MAX_EVENTS) checkPos = 0;
			if(gEvents[checkPos].status != 1) continue;
				
			long duration = now - gEvents[checkPos].last_active;
			if(duration >= 60)
			{
				close(gEvents[checkPos].fd);
				printf("fd=%d timeout [%ld--%ld].\n", gEvents[checkPos].fd,	
					 gEvents[checkPos].last_active, now );
				EventDel(gEpollFd, &gEvents[checkPos]);
			}	
		}
	
		// wait for events to happen
		int fds = epoll_wait(gEpollFd, events,MAX_EVENTS,1000);
		if(fds < 0)
		{
			err_sys("epoll_wait error , exit");
		}
		
		for(int i = 0; i< fds; i++)
		{
			myevent_s *ev = (struct myevent_s*)events[i].data.ptr;
			if((events[i].events & EPOLLIN) && (ev->events && EPOLLIN))
			{
				ev->call_back (ev->fd, events[i].events, ev->arg);
			}
			
			if((events[i].events & EPOLLOUT) && (ev->events && EPOLLOUT))
			{
				ev->call_back (ev->fd, events[i].events, ev->arg);
			}
		}
	} // end of while
	
	return 0;
}

void EventSet(myevent_s *ev, int fd,CALLBACK callback, void *arg) 
{
	ev->fd = fd;
	ev->call_back = callback;
	ev->events = 0;
	ev->arg = arg;
	ev->status = 0;
	//bzero(ev->buff, sizeof(ev->buff));	
	ev->s_offset  = 0;
	//ev->len  = 0;
	ev->last_active = time(NULL); 

}

// add/mod an event to epoll
void EventAdd(int epollFd, int events, myevent_s *ev)
{

	struct epoll_event epv = {0, {0}};
	int op;
	epv.data.ptr = ev;
	epv.events = ev->events = events;

	if(1 == ev->status)
	{
		op = EPOLL_CTL_ADD;
	}
	else
	{
		op = EPOLL_CTL_ADD;
		ev->status = 1; 
	}

	if (epoll_ctl(epollFd, op, ev->fd, &epv) < 0)
	{
		err_ret("event add failed[fd=%d], op=%d,events[%d]\n",ev->fd,op, events);
	}
	else
	{
		printf("Event add OK[fd=%d], op=%d, events[%0X]\n", ev->fd, op, events);
	}

}

void EventDel(int epollFd, myevent_s *ev)
{
	struct epoll_event epv = {0, {0}};
	
	if(ev->status != 1) return;
	
	epv.data.ptr = ev;
	ev->status = 0;
	if( 0 >	epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv) )
	{
		err_ret("event delete from epoll failed!");
	}	
}
