/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Kevin Tan, CS50 Lab 3, July 2017
 * Done with reference to bag.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  void *item;		      // pointer to data for this item
  char *key;			  // pointer to the key for this item
  struct setnode *next;	      // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;	      // head of the list of items in set
} set_t;


/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char *key, void *item);

static setnode_t *setnode_new(const char *key, void *item){

  setnode_t *node = count_malloc(sizeof(setnode_t));
  node->key =  malloc(strlen(key) + 1);
  strcpy(node->key,key);
  node->item = item;
  return node;
}

/* Create a new (empty) set; return NULL if error. */
set_t *set_new(void) {
	set_t *set = count_malloc(sizeof(set_t));

	if (set == NULL) {
		return NULL; // error allocating set
  	} 

  	else {
    	// initialize contents of set structure
    	set->head = NULL;
    	return set;
  	}
}

/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool set_insert(set_t *set, const char *key, void *item) {

  // Check if the string exists
  if (set != NULL && item != NULL && key != NULL) {
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      if (strcmp(node->key, key) == 0) {
        return false;
      }
    }

    // allocate a new node to be added to the list
    setnode_t *new = setnode_new(key, item);
    
    if (new != NULL) {
      // add it to the head of the list
      new->next = set->head;
      set->head = new;
      return true;
    }

    // if not remember to free free the allocated memory
    free(new);
    return false;
  }

  else{
    return false;
  }
  
}

/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */
void *set_find(set_t *set, const char *key){

	if (set != NULL && key != NULL) {
    	for (setnode_t *node = set->head; node != NULL; node = node->next) {
      		if (strcmp(node->key, key) == 0) {		    // if possible...
      			return node->item; 
      		}
      } 
  }
  return NULL;
}

/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/
void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item) ) {
	
  if (fp != NULL) {
    	if (set != NULL) {
      		fputc('{', fp);
      		for (setnode_t *node = set->head; node != NULL; node = node->next) {
        		// print this node
        		if (itemprint != NULL) {  // print the node's key and item 
          			fputc(' ', fp);
          			(*itemprint)(fp, node->key, node->item);
        		}
      		}

      	fputc('}', fp);
        fputc('\n', fp);
    	}

    else {
      fputs("(null)", fp);

    }
  }
}

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) ){
	if (set != NULL && itemfunc != NULL) {
    	// call itemfunc with arg, on each item
    	for (setnode_t *node = set->head; node != NULL; node = node->next) {
      		(*itemfunc)(arg, node->key, node->item); 
    }
  }
}

/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */
void set_delete(set_t *set, void (*itemdelete)(void *item) ){
	if (set != NULL) {
    	for (setnode_t *node = set->head; node != NULL; ) {

      		if (itemdelete != NULL) {		    // if possible...
        		(*itemdelete)(node->item);	    // delete node's item

            if(node->key != NULL){
              count_free(node->key);
              node->key = NULL;          // delete node's key
            }
      		}

          else{
            return;
          }
      	setnode_t *next = node->next;	    // remember what comes next
        count_free(node);			    // free the node
      	node = next;			        // and move on to next
      }
  }

  count_free(set);
}