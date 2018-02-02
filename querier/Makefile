# Makefile for 'querier' module
#
# Done by David Kotz, April 2016, 2017 for 'querier' module
# updated by Xia Zhou, July 2016
# modified by Kevin Tan, July 2017 for CS50

PROG = querier
LLIBS = ../common/common.a ../libcs50/libcs50.a 
OBJS = querier.o
LIBS =

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/memory.h ../common/pagedir.h ../common/index.h ../common/word.h

.PHONY: clean
	
clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
