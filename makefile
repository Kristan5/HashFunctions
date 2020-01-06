CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

all: a4.o hash.o a4.h
	$(CC) -o a4 a4.o hash.o

hash.o:
	$(CC) $(CFLAGS) -c hash.c a4.h

a4.o:
	$(CC) $(CFLAGS) -c a4.c a4.h

clean:
	rm a4 a4.o hash.o