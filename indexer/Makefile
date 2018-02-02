# Makefile for 'indexer' module
#
# Done by David Kotz, April 2016, 2017 for 'bag' module
# updated by Xia Zhou, July 2016
# modified by Kevin Tan, July 2017 for CS50
all: indexer indextest
PROG1 = indexer
PROG2 = indextest
LLIBS = ../common/common.a ../libcs50/libcs50.a 
OBJ1 = indexer.o
OBJ2 = indextest.o
LIBS =

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make

$(PROG1): $(OBJ1) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG2): $(OBJ2) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: ../libcs50/hashtable.h ../libcs50/memory.h ../common/pagedir.h ../common/index.h ../common/word.h
indextest.o: ../libcs50/hashtable.h ../libcs50/memory.h ../common/pagedir.h ../common/index.h

.PHONY: clean
	
clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
