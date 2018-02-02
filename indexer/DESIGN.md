# CS50 Tiny Search Engine (TSE) indexer design

Kevin Tan, August 2017

This is the indexer module that supports the TSE project. 

## Design

Index is implemented as a special hashtable structure which takes in the words as its keys and a counters structure as its item. The keys for the counters indicate the file in which the word appears and the count indicate the number of times the given word appears in the file. As a matter of pragmatic programming, the design of index had to be modified from hashtable because there were additional functionality such as index_save and index_load which involved file input and output which the original hashtable struct did not support. The hashtable implementation is chosen at the start as it allows us to find words in O(1) time. Counters were chosen as they were best able to map a document number to the number of time the words appeared since it takes a int for key and int as item.

index_build builds an inverted index structure which maps from words to documents and prints this inverted index to a file. It is included in indexer.c instead of index.c because it involves the temporary creation of webpage data structure for word processing. Furthermore, it serves as the overall function that coordinates the file input, counters input and index input into the index_save function. Because of its overall complexity, it is not included in the index.c file but in the indexer.c.