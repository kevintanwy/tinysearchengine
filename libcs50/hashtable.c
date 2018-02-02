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
#include "hashtable.h"
#include "memory.h"
#include "jhash.h"
#include "set.h"



/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/

typedef struct hashtable {
	struct set **array;
	int slots;
} hashtable_t;



/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

// New hashtable function
hashtable_t *hashtable_new(const int num_slots){

	hashtable_t *ht = malloc(sizeof(hashtable_t));
	ht->array = calloc(num_slots, sizeof(set_t*));

	if (ht == NULL || ht->array == NULL){
		count_free(ht->array);
		count_free(ht);
		return NULL;
	}

	else {
		ht->slots = num_slots;
		return ht;
	}
}

/* Insert item, identified by key (string), into the given hashtable.
 * The key string is copied for use by the hashtable (done through the set function).
 * Return false if key exists in ht, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item){

	if (ht != NULL && key != NULL && item != NULL){
		int slotno = JenkinsHash(key, ht->slots);
		// check if the array slot has not been initialised
		if (ht->array[slotno] == NULL){
			// if not, initialise it
			set_t *set = set_new();
			if (set == NULL) {
    			fprintf(stderr, "set_new failed\n");
    			return false;
  			}
  			set_insert(set, key, item);
			ht->array[slotno] = set;
			return true;
		}

		else{
			// if it already has been initialsed, check if the key exists
			set_t *set = ht->array[slotno];
			if (set_find(set, key) != NULL){
				return false;
			}

			else{
				set_insert(set,key, item);
				return true;
			}	
		}
	}

	//one or more of the input parameters was null.
	else{
		return false;
	}
}

/* Return the item associated with the given key;
 * return NULL if hashtable is NULL, key is NULL, key is not found.
 */
void *hashtable_find(hashtable_t *ht, const char *key){
	
	if (ht != NULL && key != NULL){

		int slotno = JenkinsHash(key, ht->slots);
		set_t *set = ht->array[slotno];

		if (set_find(set,key) == NULL){
			return NULL;
		}
		else{
			return set_find(set,key);
		}
	}

	else{
		return NULL;
	}
}

/* Print the whole table; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL ht.
 * Print a table with no items if NULL itemprint.
 */
void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item)){

	if (fp!=NULL){
		if (ht != NULL){
			if (itemprint != NULL){
				for (int i=0; i< ht->slots; i++){
				set_print(ht->array[i], fp, (*itemprint));
				}
			}
			else{
				printf("{} \n");
			}

		}
		else{
			printf("(null) \n");
		}
	}
}

/* Iterate over all items in the table; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If ht==NULL or itemfunc==NULL, do nothing.
 */
void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) ){
	if (ht != NULL && itemfunc != NULL){
		for (int i = 0; i < ht->slots; i++){
			set_iterate(ht->array[i], arg, itemfunc);
		}
	}
}

/* Delete the whole hashtable; ignore NULL ht.
 * Provide a function that will delete each item (may be NULL).
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ){

	if (ht != NULL){
		for(int i = 0; i < ht->slots; i++){
			set_delete(ht->array[i], itemdelete);
		}
		
		// Only free the array memory if the item delete function is given so we are sure that
		// the items have been deleted
		if (itemdelete != NULL){
			count_free(ht->array);
			count_free(ht);
		}	
	}
}
