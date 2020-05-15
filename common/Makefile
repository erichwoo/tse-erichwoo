# Makefile for "pagedir.c"

L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
OBJS = pagedir.o word.o index.o 
LIB = common.a
LLIBS = $L/libcs50.a

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h

index.o: index.h

word.o: word.h

.PHONY: clean

clean:	
	rm -f $(LIB) .*~ *~ *.o *.dSYM core
