CC = cc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c17 -Iinclude

all: server client

server:
	$(CC) $(CFLAGS) src/server.c -o server

client:
	$(CC) $(CFLAGS) src/client.c -o client

clean:
	rm -f server client

.PHONY: all server client clean
