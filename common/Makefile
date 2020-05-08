# Makefile for "pagedir.c"

L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
OBJS = pagedir.o 
LIB = common.a
LLIBS = $L/libcs50-given.a

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h

.PHONY: clean

clean:	
	rm -f $(LIB) .*~ *~ *.o *.dSYM core
