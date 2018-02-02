# CS50 Tiny Search Engine (TSE) querier testing results

Kevin Tan, Aug 2017

This is the querier module that supports the TSE project.
The crawler page directory created from http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ as the seed URL and with depth 1.
The index was subsequently created from the files in that page directory.

## Normal Test
```
[kevintanwy@flume ~/cs50/labs/tse/querier]$ querier ../crawler/pageDirectory ../indexer/test
Query: 
dartmouth
score 416 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   1 doc   1 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ 
score   1 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
Query: 
college
score 242 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   2 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
score   1 doc   1 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ 
score   1 doc   2 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Linked_list.html 
Query: 
dartmouth college
score 242 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   1 doc   1 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ 
score   1 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
Query: 
dartmouth or college
score 658 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   3 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
score   2 doc   1 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ 
score   1 doc   2 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Linked_list.html 
Query: 


```

## Camel Case Test
```
Query: 
DartMouth or CoLLeGE
score 658 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   3 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
score   2 doc   1 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ 
score   1 doc   2 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Linked_list.html 
Query: 
professor or Dartmouth and College
score 248 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   2 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
score   1 doc   1 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ 
```

## Mispelling Test
```
Query: 
Dartmith Colloge or Pofeissr
No matches found 
Query: 
Dartmith Colloge or professor
score   6 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   1 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
```

## Non-alphabet character
```
Query: 
abc123
non-alphabet character detected at position 3 
Query: 
Dartmouth?
non-alphabet character detected at position 9 
```

## Wrong sequence of "and"/"or"
```
Query: 
Dartmouth and and College
and/or cannot be two consecutive words 
Query: 
Dartmouth and Or College
and/or cannot be two consecutive words 
Query: 
Dartmouth or And College
and/or cannot be two consecutive words 
Query: 
Dartmouth or or college
and/or cannot be two consecutive words 
Query: 
and Dartmouth College
and/or cannot be the first or last word 
Query: 
or Dartmouth College
and/or cannot be the first or last word 
Query: 
Dartmouth or
and/or cannot be the first or last word 
Query: 
College or
and/or cannot be the first or last word 
```

## EOF
```
[kevintanwy@flume ~/cs50/labs/tse/querier]$ myvalgrind querier ../crawler/pageDirectory ../indexer/test
==5205== Memcheck, a memory error detector
==5205== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==5205== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
==5205== Command: querier ../crawler/pageDirectory ../indexer/test
==5205== 
Query: 
nkw vwnwvvbwbvwvjwv
No matches found 
Query: 
Dartmouth or College
score 658 doc   4 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html 
score   3 doc   7 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html 
score   2 doc   1 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ 
score   1 doc   2 http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Linked_list.html 
Query: 
exiting... 
==5205== 
==5205== HEAP SUMMARY:
==5205==     in use at exit: 0 bytes in 0 blocks
==5205==   total heap usage: 43,454 allocs, 43,454 frees, 3,729,240 bytes allocated
==5205== 
==5205== All heap blocks were freed -- no leaks are possible
==5205== 
==5205== For counts of detected and suppressed errors, rerun with: -v
==5205== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```


## Wrong number of arguments
```
[kevintanwy@flume ~/cs50/labs/tse/querier]$ querier ../crawler/pageDirectory
invalid argument parameters 
```

## Wrong directory
```
[kevintanwy@flume ~/cs50/labs/tse/querier]$ querier ../pageDirectory ../indexer/test
invalid argument parameters 
```

## Invalid index file
```
[kevintanwy@flume ~/cs50/labs/tse/querier]$ querier ../pageDirectory ../indexer/test10
invalid argument parameters 
```