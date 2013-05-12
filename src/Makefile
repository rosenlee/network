# Makefile
# // Rosen
# // 2013-5-12


CC=g++
FLAGS=-g
LIBS=
INCLUDE= 

.PHONY: clean

all: tcpclient  tcpserver

tcpclient: network.cpp  tcpclient.cpp
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) $^ -o $@

tcpserver: network.cpp  tcpserver.cpp
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) $^ -o $@


clean:
	rm -f *.o
	rm -f *.exe
