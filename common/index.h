/*
* index.h
* An index is special implemention of the hashtable structure. The (key,item) pair 
* is a (key, counters) pair.
* Kevin Tan, CS50 Lab 3, July 2017
*/

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "counters.h"
#include "memory.h"
#include "jhash.h"
#include "file.h"
#include "file.h"
#include "word.h"

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** global functions ****************/
/* Create a new (empty) index; return NULL if error. */
index_t *index_new(const int num_slots);

/* Insert counter, identified by key (string), into the given index.
 * The key string is copied for use by the hashtable.
 * Return false if key exists in ht, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool index_insert(index_t *ind, const char *key, void *item);

/* Return the counter structure associated with the given key;
 * return NULL if hashtable is NULL, key is NULL, key is not found.
 */
counters_t *index_find(index_t *ind, const char *key);

/* Print the whole index; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL index.
 * Print a table with no items if NULL itemprint.
 */
void index_print(index_t *ind, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item));

/* Iterate over all items in the index; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If ht==NULL or itemfunc==NULL, do nothing.
 */
void index_iterate(index_t *ind, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) );

/* Delete the whole index; ignore NULL ind.
 * Provide a function that will delete each item (may be NULL).
 */
void index_delete(index_t *ind, void (*itemdelete)(void *item) );

/* User provides an open file for reading and an initialized index
 * that will be modified based on the lines in the file.
*/
void index_load(FILE *fp, index_t *ind);


/* write the index into the file by iterating through all the entries and 
 * printing the counters in each entry. Uses the local counters_print and 
 * simple_print function.
 */
void index_save(FILE *file, index_t *ind);

#endif