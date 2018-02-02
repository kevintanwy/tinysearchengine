# CS50 Tiny Search Engine (TSE) indexer testing results

Kevin Tan, June 2017

This is the crawler module that supports the TSE project. 

## Normal Test
```
[kevintanwy@flume ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ pageDirectory 5
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ indexer ../crawler/pageDirectory test
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ indextest test testing
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ sort test > sort1
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ sort testing > sort2
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ diff sort1 sort2
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ cat sort1
algorithm 2 1 
biology 10 1 
breadth 4 1 
coding 7 1 
computational 10 1 
depth 9 1 
eniac 5 1 
fast 8 1 
first 4 1 9 1 
for 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 1 10 1 
fourier 8 1 
graph 6 1 
home 1 2 2 1 3 2 4 1 5 1 6 1 7 1 8 1 9 1 10 1 
huffman 7 1 
page 1 1 3 1 
playground 1 1 3 1 
search 4 1 9 1 
the 1 1 3 1 
this 1 1 3 1 
transform 8 1 
traversal 6 1 
tse 1 1 3 1 
```

## Wrong number of arguments
```
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ indexer
wrong argument parameters. 
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ indexer 1 2 3 4 
wrong argument parameters. 
```

## Wrong directory
```
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ indexer pageDirectory test
wrong argument parameters. 
```

## Wrong number of inputs
```
[kevintanwy@whaleback crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory
Argument number is wrong.
```

## Directory does not exist
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ rm -rf pageDirectory
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory 5
Directory does not exist or is not empty. 
```

## Directory does not have .crawler
```
[kevintanwy@flume ~/cs50/labs/tse/crawler]$ rm -rf pageDirectory
[kevintanwy@flume ~/cs50/labs/tse/crawler]$ mkdir pageDirectory
[kevintanwy@flume ~/cs50/labs/tse/crawler]$ cd ../indexer
[kevintanwy@flume ~/cs50/labs/tse/indexer]$ indexer ../crawler/pageDirectory test
wrong argument parameters. 

```