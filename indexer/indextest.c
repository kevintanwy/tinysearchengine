/* 
 * indexertest.c - testing file for CS50 TSE 'indexer module'
 *
 * Kevin Tan, CS50 Lab 5, July 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "memory.h"
#include "../common/index.h"
#include "../common/word.h"

/* ***************************************** */
/* Private methods */

/* ************* itemDelete ******************** */
/* Item delete function to delete the counters in the index.
 * Calls counters_delete to accomplish this task.
 *
 * Pseudocode:
 * 1. Check if the index has an item (which will be a counter)
 * and then call the counters_delete function on it.
 */

static void itemdelete(void *item)
{
  if (item) {
    counters_delete(item);
  }
}


int main( int argc, char **argv )
{
	// Check arguments
	if (argc == 3){

		// Open the files for reading and writing.
		FILE *readfrom =  fopen(argv[1], "r");
		if (readfrom == NULL) {
			printf("unable to open %s \n", argv[1]);
			exit(2);
		}

		FILE *writeto = fopen(argv[2], "w+");
		if (writeto == NULL) {
			printf("unable to open %s \n", argv[2]);
			fclose(readfrom);
			exit(2);
		}

		// Initialise the index that will be modified based
		// on the file information.
		index_t *index;
		int lines = lines_in_file(readfrom);
		index = index_new(0.75 * lines);		// Same as Java's load factor
		index_load(readfrom, index);
		index_save(writeto, index);

		// clean up
		index_delete(index, itemdelete);
		fclose(writeto);
		fclose(readfrom);

	}

	else{
		printf("wrong input parameters \n");
		exit(1);
	}

	return 0;
	
}