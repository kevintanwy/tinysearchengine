# Makefile for 'pagedir' library
#
#  Builds and compiles all these utility functions into a library.
#
# David Kotz, April 2016, July 2017 for libcs50
# updated by Xia Zhou, July 2016
# modified by Kevin Tan for common, July 2017

# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DNOSLEEP # -DMEMTEST 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h 
index.o: ../libcs50/hashtable.h ../libcs50/counters.h index.h
word.o: word.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f $(LIB) *~ *.o
