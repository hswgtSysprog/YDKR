#
# $Id: Makefile 86 2011-01-27 20:18:45Z merkelt $
#

# Options
CC=gcc
MAKE=make

CFLAGS=

SRCS= 

 

OBJS=$(SRCS:.c=.o)
LIB=

# Common
all: $(OBJS)
	@echo Compiling Common

.c.o:
	@echo Compiling $<
	@$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(BIN) *.o *.bak *~

