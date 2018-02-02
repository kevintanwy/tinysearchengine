/* 
 * counter.c - CS50 'counter' module
 *
 * see counter.h for more information.
 *
 * Kevin Tan, CS50 Lab 3, July 2017
 * Done with reference to bag.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counternode {
  int counter;		      // pointer to the counter
  int key;			        // pointer to the integer key 
  struct counternode *next;	      // link to next node
} counternode_t;

/**************** global types ****************/
typedef struct counters {
  struct counternode *head;	      // head of the list of items in counter
  int count; 
} counters_t;


/**************** global functions ****************/
/* that is, visible outside this file */
/* see counter.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static counternode_t *counternode_new(int key, int counter);


static counternode_t *counternode_new(int key, int count){
  counternode_t *node = count_malloc(sizeof(counternode_t));
  node->key = key;
  node->counter = count;
  return node;
}


/* Create a new (empty) counter structure; return NULL if error. */
counters_t *counters_new(void) {
  counters_t *counters = count_malloc(sizeof(counters_t));

	if (counters == NULL) {
		return NULL; // error allocating counters
  	} 

  	else {
    	// initialize contents of counters structure
      counters->head = NULL;
    	return counters;
  	}
}

/* Increment the counter indicated by key; key must be >= 0.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * Ignore if ctrs is NULL or key is negative.
 * Return the new value of the counter related to the inserted key
 */
int counters_add(counters_t *ctrs, const int key){
  if (ctrs != NULL && key >= 0){
    // check whether the key exists. If it does, increment the counter indicated by key.
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
          if (node->key == key) {       
            node->counter += 1;
            return (node->counter);
          }
    }
  }
  else{
    return 0;
  }

  // key does not exist. create a counter for it and initialize to 1.
  counternode_t *new = counternode_new(key, 1);
  if (new != NULL) {
    new->next = ctrs->head;
    ctrs->head = new;
  }
  else{
    count_free(new);
    return 0;
  }
  return (new->counter);
}

/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */
int counters_get(counters_t *ctrs, const int key){
  if (ctrs != NULL && key >= 0){
    // check whether the key exists.
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
          if (node->key == key) {  
            return (node->counter);
          }
    }
  }
  return 0;
}

/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. Ignore if ctrs is NULL, if key < 0, or count < 0.
 */
void counters_set(counters_t *ctrs, const int key, int count){
  
  if (ctrs != NULL && key >= 0 && count >= 0){
    // check whether the key exists. If it does, set the counter indicated by key.
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      if (node->key == key){
        node->counter = count;
        return;
      }
    }

    // Make a new counternode and insert it.
    counternode_t *new = counternode_new(key, 1);
    if (new != NULL) {
      new->next = ctrs->head;
      ctrs->head = new;
    }
  }

}


/* Print all counters; provide the output file.
 * Ignore if NULL fp. Print (null) if NULL ctrs.
 */
void counters_print(counters_t *ctrs, FILE *fp) {
  if (fp != NULL) {
      if (ctrs != NULL) {
          fputc('{', fp);
          for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
            fputc(' ', fp);
            fprintf(fp, "%d", node->key);
            fputc(':', fp);
            fprintf(fp, "%d", node->counter);
          }
          fputc('}', fp);
      }

      else{
        fputs("(null)", fp);
      }
  }

}

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 * If ctrs==NULL or itemfunc==NULL, do nothing.
 */
void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count)) {
  if (ctrs != NULL && itemfunc != NULL){
    // call itemfunc with arg, on each item
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->counter); 
    }
  }
}

/* Delete the whole counters. ignore NULL ctrs. */
void counters_delete(counters_t *ctrs) {
  if (ctrs != NULL){
    for (counternode_t *node = ctrs->head; node != NULL; ) {
      counternode_t *next = node->next;
      count_free(node);
      node = next;
    }
    count_free(ctrs);
  }
}