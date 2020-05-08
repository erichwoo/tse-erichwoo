# Makefile for the "crawler" program
#
# Erich Woo - 1 May 2020

L = ../libcs50
C = ../common

#Uncomment to turn on testing mode
UNITTEST = -DTEST

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm $(UNITTEST) -I$L -I$C
PROG = crawler
OBJS = $(PROG).o
LLIBS = $L/libcs50-given.a $C/common.a

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

all: $(PROG) $(PROG)test
#dependencies

.PHONY: clean test

test: testing.sh
	bash -v testing.sh
clean:
	rm -rf $(PROG) tse-output/ unwritable/ .*~ *~ *.o *.dSYM core

