# Options
CC=gcc
MAKE=make
Q=@
CFLAGS=-g -pthread -Wall `pkg-config --cflags  gtk+-2.0`
LDFLAGS=`pkg-config --libs gtk+-2.0` -lgthread-2.0 -lpthread
SRCS=../../src/main.c
OBJS=$(SRCS:.c=.o) ../../src/libquizgui.a
BIN=./client

# Client
all: $(OBJS)
	@echo Compiling Client
	$(Q)$(CC) -o $(BIN) $(CFLAGS) $(OBJS) $(LDFLAGS)

.c.o: commons.h
	@echo Compiling $<
	$(Q)$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(BIN) *.o *.bak *~
