/* 
 * indexer.c - CS50 'indexer' module
 *
 * see indexer.h for more information.
 *
 * Kevin Tan, CS50 Lab 3, July 2017
 * Done with reference to hashtable.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/memory.h"
#include "../libcs50/jhash.h"
#include "../libcs50/file.h"
#include "../common/word.h"
#include "memory.h"



/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/

typedef hashtable_t index_t;

/**************** local functions ****************/

static void simple_print(void *arg, const int key, int count);
static void print_counter(void *arg, const char* key, void *cntrs);

/* Simple print function to print a counter's key and count. Helper
 * function to print_counter.
 */
static void simple_print(void *arg, const int key, int count)
{
	fprintf(arg, "%d %d ", key, count);
}

/* Function that iterates through all the counters and calls simple_print
 * on them. Format will be in:
 * [word_key] [counter_key1] [counter_count1] [counter_key2] [counter_count2]...
 * Helper function to index_save
 */
static void print_counter(void *arg, const char* key, void *cntrs)
{
	fprintf(arg, "%s ", key); 					// prints the key as the first thing in the line
	counters_iterate(cntrs, arg, simple_print); // prints the rest of the counters
	fprintf(arg, "\n"); 						// insert a new line character for the next key.

}


/**************** global functions ****************/
/* that is, visible outside this file */
/* see index.h for comments about exported functions */

// New index function.
index_t *index_new(const int num_slots)
{
	return hashtable_new(num_slots);
}

/* Insert item, identified by key (string), into the given index.
 * The key string is copied for use by the hashtable (done through the set function).
 * Return false if key exists in ind, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool index_insert(index_t *ind, const char *key, void *item)
{
	return hashtable_insert(ind, key, item);
}

/* Return the item associated with the given key;
 * return NULL if hashtable is NULL, key is NULL, key is not found.
 */
counters_t *index_find(index_t *ind, const char *key)
{
	return (counters_t*) hashtable_find(ind, key);
}

/* Print the whole index; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL ht.
 * Print a table with no items if NULL itemprint.
 */
void index_print(index_t *ind, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item))
{
	hashtable_print(ind, fp, itemprint);
}

/* Iterate over all items in the index; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If ht==NULL or itemfunc==NULL, do nothing.
 */
void index_iterate(index_t *ind, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) )
{
	hashtable_iterate(ind, arg, itemfunc);
}

/* Delete the whole index; ignore NULL ind.
 * Provide a function that will delete each item (may be NULL).
 */
void index_delete(index_t *ind, void (*itemdelete)(void *item) )
{
	hashtable_delete(ind, itemdelete);
}

/* User provides an open file for reading and an initialized index
 * that will be modified based on the lines in the file.
*/

void index_load(FILE *fp, index_t *ind)
{
		char *key;
		int counter_key;
		int counter_count;

		while ((key = readwordp(fp)) != NULL){
			counters_t *word_counters;

			// initialise a new counter structure
			word_counters = counters_new();

			// Assign the values to key and count.
			while (fscanf(fp, "%d %d ", &counter_key, &counter_count) == 2){
				counters_set(word_counters, counter_key, counter_count);
			}

			// insert the counters structure to the index.
			index_insert(ind, key, word_counters);
			free(key);
		}

}



/* write the index into the file by iterating through all the entries and 
 * printing the counters in each entry.
 */
void index_save(FILE *file, index_t *ind)
{
	index_iterate(ind, file, print_counter);
}