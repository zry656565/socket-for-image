OBJS = client.o server.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: client server

client.o: client.cpp
	$(CC) $(CFLAGS) client.cpp

server.o: server.cpp
	$(CC) $(CFLAGS) server.cpp

client: client.o
	$(CC) $(LFLAGS) client.o -o client

server: server.o
	$(CC) $(LFLAGS) server.o -o server

clean: rm client.o server.o client server