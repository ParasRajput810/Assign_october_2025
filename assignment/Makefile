CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
LDFLAGS = -lm

all: horner

horner: horner.c
	$(CC) $(CFLAGS) -o horner horner.c $(LDFLAGS)

clean:
	rm -f horner *.o
