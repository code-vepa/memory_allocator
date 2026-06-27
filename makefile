CC = gcc
CFLAGS = -g3 -Wall -Wextra -std=c11 -pedantic

TARGET = heap_alloc

OBJS = main.o heap_alloc.o list_impl.o

all: $(TARGET)
	
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c mem_alloc.h
	$(CC) $(CFLAGS) -c main.c

heap_alloc.o: heap_alloc.c mem_alloc.h
	$(CC) $(CFLAGS) -c heap_alloc.c

list_impl.o: list_impl.c mem_alloc.h
	$(CC) $(CFLAGS) -c list_impl.c

clean:
	rm -f *.o $(TARGET)
