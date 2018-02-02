# CS50 Tiny Search Engine (TSE) querier utility library

Kevin Tan, August 2017

This is the querier module that supports the TSE project. 

## Usage

To build `querier`, run `make`. Make sure that you have copied your set.c, counters.c, file.c, webpage.c and jhash.c and hashtable.c to the libcs50 folder and word.c and index.c are in the common folder. Also make sure that crawler has been run to create a page directory of saved webpages with a .crawler file added and that indexer has built a file that holds the inverted index structure.

To clean up, run `make clean`.

## Overview

 *querier: The TSE querier reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user. Specifically, it tells the user if the words the user is looking for is in the webpages that were crawled and prints out the webpages in order of descending rank (rank is defined by the number of times the word(s) appeared in a given webpage relative to other webpages).