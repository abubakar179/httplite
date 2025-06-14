CC = gcc
CFLAGS = -Wall -Wextra -O2

all: httplite

httplite: start-httplite.c
	$(CC) $(CFLAGS) -o httplite start-httplite.c

clean:
	rm -f httplite
