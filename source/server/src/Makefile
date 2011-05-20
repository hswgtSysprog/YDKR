# Options
CC=gcc
MAKE=make

CFLAGS=-Wall -g -rdynamic -O2
SRCS=server.c login.c client.c
OBJS=$(SRCS:.c=.o) ../common/network.o 
# OBJS=$(SRCS:.c=.o) ../common/network.o ../common/debug.o ../common/sem.c ../common/keymanager.o ../common/mq.o
LIB=-lpthread
BIN=./server.bin

# Server
all: $(OBJS)
	@echo Compiling Server
	@$(CC) -o $(BIN) $(CFLAGS) $(OBJS) $(LIB)

.c.o:
	@echo Compiling $<
	@@$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(BIN) *.o *.bak *~
