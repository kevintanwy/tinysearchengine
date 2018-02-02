# CS50 Tiny Search Engine (TSE) crawler testing results

Kevin Tan, June 2017

This is the crawler module that supports the TSE project. 

## Normal Test
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ mkdir pageDirectory
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory 5
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ ls pageDirectory
1  10  2  3  4	5  6  7  8  9
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ cat pageDirectory/6
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
4
<html><title>G</title>Gisfor<ahref=https://en.wikipedia.org/wiki/Graph_traversal>Graphtraversal</a>.<ahref=H.html>H</a><ahref=index.html>home</a></html>
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ cat pageDirectory/7
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/H.html
5
<html>
<title>H</title>
H is for <a href=https://en.wikipedia.org/wiki/Huffman_coding>Huffman coding</a>.
<a href=B.html>B</a>
<a href=index.html>home</a>
</html>

```

## Normal Test in the same graph
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html ./pageDirectory 5
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ ls pageDirectory
1  2  3  4  5  6  7  8	9
```

## Normal test to see if urls are being added properly to the bag
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory 1
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html 
## I added a print function in page scanner to produce the URL.
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

## Non-empty directory
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ mkdir pageDirectory
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory 5
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory 5
Directory does not exist or is not empty. 
```

## Invalid seed URL
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory 5
URL is not internal. 

```

## Non-integer argument for depth
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory abc
Depth is not a positive integer. 
```

## Negative integer for depth
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./pageDirectory -4
Depth is not a positive integer. 
```

## Input a bad (but valid) url
```
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html/ ./pageDirectory 5
URL or HTML are empty. 1 skipped. 
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ ls pageDirectory
1
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ cat pageDirectory/1
[kevintanwy@whaleback ~/cs50/labs/tse/crawler]$ 
 # The pagesaver function created the file, but seeing as the url and html were empty, it did not write anything to the file and just exited.
 ```