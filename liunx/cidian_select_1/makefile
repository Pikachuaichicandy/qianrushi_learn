CC = gcc
CFLAGS = -Wall
LDFLAGS = -lsqlite3

all: client server

client: client.c
	$(CC) $(CFLAGS) -o $@ $^ 

server: server.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f client server my.db show/my.db

.PHONY: all clean