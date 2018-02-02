/* 
 * pagedir - utility functions for adding pages to a bag and hashtable, writing an
 			 extracted page into a file and a function to fetch a webpage.
 *           See pagedir.h for usage.
 *
 * Kevin Tan, July 2019 for tse lab, CS50
 *
 */


#ifndef __PAGEDIR_H
#define __PAGEDIR_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/**************** pagescanner ****************/
/* Scans a webpage structure for the URLs to other links by 
 * calling the webpage_nextURL() function. Function frees the 
 * malloced memory for the **word in the function. The URLs
 * will be used to create a new webpage structure which will
 * be inserted into the bag and the hashtable if the url is 
 * internal and not already in the hashtable.
 * 
 * Assumptions:
 * 1. *current will be a webpage structure
 * 2. maxdepth will be an integer greater than 0.
 * 3. *hashtable will be a hashtable.
 * 4. *bag will be a bag
 * 
 * Usage example (scan the first item of the bag for urls):
 * hashtable_t *visited;
 * visited = hashtable_new(100);
 * webpage_t *current = bag_extract(urls);
 * maxdepth = 4;
 * pagescanner(current, maxdepth, visited, urls);
 */
void pagescanner(void *current, int maxdepth, void *hashtable, void *bag);


/**************** pagescanner ****************/
/* Writes the webpage's url, depth and html code into a
 * numbered file in the directory that the user has given.
 * 
 * Assumptions:
 * 1. The digits for document id is less than 25 characters long.
 * 2. User gives a valid directory path.
 * 3. *current will be a webpage struct.
 * 
 * Usage example (creates a file in the  page directory in the 
 * current directory called pagedirectory/0) from the webpage given:
 * pagesaver(pageDirectory, 0, current);
 */
void pagesaver(char *dirname, int documentid, void *current);


/***************** pagefetch ******************************/
/* Calls the webpage_fetch function. See documentation for 
 * webpage_fetch() for more details.
 */
bool pagefetch(void *webpage);


#endif