# Options
CC=gcc
MAKE=make
Q=@
CFLAGS=-g -pthread -Wall `pkg-config --cflags  gtk+-2.0 --libs` `pkg-config gthread-2.0 --libs` -m32
LDFLAGS=`pkg-config --libs gtk+-2.0` -lgthread-2.0 -lpthread
SRCS=client.c listener.c fragewechsel.c
OBJS=$(SRCS:.c=.o) libquizgui.a ../../common/src/keymanager.o ../../common/src/sem.o
BIN=./client.bin

# Client
all: common $(OBJS)
	@echo Compiling Client
	$(Q)$(CC) -o $(BIN) $(CFLAGS) $(OBJS) $(LDFLAGS)

.c.o: 
	@echo Compiling $<
	$(Q)$(CC) $(CFLAGS) -c $<

common:
	$(Q)cd ../../common/src/; $(MAKE)

clean:
	rm -f $(BIN) *.o *.bak *~
