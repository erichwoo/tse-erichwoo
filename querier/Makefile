# Makefile for 'querier' directory
#
# Erich Woo - 21 May 2020

L = ../libcs50
C = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=gnu11 -ggdb -I$L -I$C
PROG = querier
FUZZ = fuzzquery
OBJS = $(PROG).o
FUZZOBJS = $(FUZZ).o
LLIBS = $C/common.a $L/libcs50.a

all: $(PROG) $(FUZZ)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

$(FUZZ): $(FUZZOBJS) $L/libcs50.a
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean test

test: testing.sh
	bash -v testing.sh

clean:
	rm -rf $(PROG) $(FUZZ) unreadabledir notcrawlerdir unreadablefile testfile actual expected .*~ *~ *.o *.dSYM core
