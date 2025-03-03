all: rodcut_fifo rodcut_lru

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lm

# FIFO version
FIFO_OBJS = rodcut.o fifo_cache.o
rodcut_fifo: $(FIFO_OBJS)
	$(CC) -o rodcut_fifo $(CFLAGS) $(FIFO_OBJS) $(LDFLAGS)

# LRU version
LRU_OBJS = rodcut.o lru_cache.o
rodcut_lru: $(LRU_OBJS)
	$(CC) -o rodcut_lru $(CFLAGS) $(LRU_OBJS) $(LDFLAGS)

# Object files
rodcut.o: rodcut.c cache.h
	$(CC) $(CFLAGS) -c rodcut.c -o rodcut.o

fifo_cache.o: fifo_cache.c cache.h
	$(CC) $(CFLAGS) -c fifo_cache.c -o fifo_cache.o

lru_cache.o: lru_cache.c cache.h
	$(CC) $(CFLAGS) -c lru_cache.c -o lru_cache.o

clean:
	rm -f rodcut_fifo rodcut_lru *.o