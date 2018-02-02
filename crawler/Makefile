# Makefile for 'crawler' module
#
# Done by David Kotz, April 2016, 2017 for 'bag' module
# updated by Xia Zhou, July 2016
# modified by Kevin Tan, July 2017 for CS50

PROG = crawler
LLIBS = ../libcs50/libcs50.a ../common/common.a
OBJS = crawler.o
LIBS =

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: ../libcs50/bag.h ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/memory.h ../common/pagedir.h

.PHONY: clean
	
clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
