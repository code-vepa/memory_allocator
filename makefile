CC = gcc
CFLAGS = -g3 -Wall -Wextra -std=c11 -pedantic

all: heap_alloc.o  
	$(CC) $(CFLAGS) -o heap_alloc heap_alloc.o

heap_alloc.o: heap_alloc.c
	$(CC) $(CFLAGS) -c heap_alloc.c

clean:
	rm -f *.o heap_alloc
