# Makefile for the "crawler" program
#
# Erich Woo - 1 May 2020

L = ../libcs50
C = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
PROG = crawler
OBJS = $(PROG).o
LLIBS = $L/libcs50-given.a $C/common.a

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

#dependencies

.PHONY: clean

test: testing.sh
	bash -v testing.sh
clean:
	rm -rf $(PROG) tse-output/ unwritable/ .*~ *~ *.o *.dSYM core

