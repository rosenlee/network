# Makefile
# // Rosen
# // 2013-5-12


CC=g++
FLAGS=-g
LIBS= network.cpp tcpConnect.cpp wrapsock.cpp
INCLUDE= 

.PHONY: clean

all: tcpclient  tcpserver daytimetcpcli

tcpclient: tcpclient.cpp
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) $^ -o $@

tcpserver: tcpserver.cpp
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) $^ -o $@

daytimetcpcli: daytimetcpcli.cpp
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) $^ -o $@


clean:
	rm -f *.o
	rm -f *.exe
