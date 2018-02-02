# CS50 Tiny Search Engine (TSE) crawler utility library

Kevin Tan, June 2017

This is the crawler module that supports the TSE project. 

## Usage
To build `crawler`, run `make`. Make sure that you have copied your set.c, counters.c, and hashtable.c to the libcs50 folder and there the functions to fetch, save and scan the page are in the common folder.

To clean up, run `make clean`.

## Overview

 * crawler: The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”. Uses set and hashtable data structures from lab 3.
 * pagedir: functions that support the fetching, saving and reading of webpages for crawler.
