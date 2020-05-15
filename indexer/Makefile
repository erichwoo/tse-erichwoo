# Makefile for the "indexer" and "indextest"
#
# Erich Woo - 12 May 2020

L = ../libcs50
C = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
PROG = indexer
TEST = indextest
OBJS = $(PROG).o
TESTOBJS = $(TEST).o
LLIBS = $C/common.a $L/libcs50.a 

all: $(PROG) $(TEST)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST): $(TESTOBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean test

test: testing.sh
	bash -v testing.sh
clean:
	rm -rf $(PROG) $(TEST) .*~ *~ *.o *.dSYM core output/ sorteddata/ sortedoutput/ unreadabledir/ unwriteabledir/ unwriteablefile
