# CS50 Tiny Search Engine (TSE) indexer utility library

Kevin Tan, August 2017

This is the indexer module that supports the TSE project. 

## Usage

To build `indexer`, run `make`. Make sure that you have copied your set.c, counters.c, file.c, webpage.c and jhash.c and hashtable.c to the libcs50 folder and word.c and index.c are in the common folder. 

To clean up, run `make clean`.

## Overview

 *indexer: The TSE indexer is a program that reads the files in the directory that has been created by the crawler URL. It creates a modified hashtable called index and inserts the words found in the webpages as the keys and the counters of the number of time the word has appeared in each file in the as the item. The index is then output to another file for further processing.
 * index: modified hashtable datastructure that takes in the words as keys and counters of the number of times the words have appeared in each fle as the items. Uses the hashtable and counter from lab 3.