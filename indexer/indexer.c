/* 
 * indexer.c - CS50 'indexer' module
 *
 *
 * Kevin Tan, CS50 Lab 5, July 2017
 * Done with reference to bag.c
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <webpage.h>
#include <unistd.h>
#include <string.h>
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "memory.h"
#include "../common/index.h"
#include "../common/word.h"


/* ***************************************** */
/* Private methods */
static bool checkDir (char *directory);
static void itemdelete(void *item);


/* *********************************************************************** */
/* Public methods */
void index_build (char *pageDirectory, index_t *index);


/* ************* checkDir ******************** */
/* Checks if the directory is readable and is crawler created.
 * 
 * Limitations:
 * We are assuming that the crawler will leave a file called 
 * .crawler in the directory.
 * 
 * Pseudocode:
 * 1. Check if the directory exists and is readable. Else return false.
 * 2. Contatenate the directory name and ".crawler" string and try to open the
 *  file. If the file is able to be opened, then the directory is a valid 
 *  directory. Else return false.
 */

static bool checkDir (char *directory){
	//struct dirent *d;
  	DIR *dir = opendir(directory);
  	FILE *fp;

	if (dir == NULL && access(directory, R_OK) != 0)
		return false;
	
	else {
		char filename[strlen(directory) + 25];
		sprintf(filename, "%s/.crawler", directory);
		fp = fopen(filename, "r");
		
		if (fp != NULL){
			fclose(fp);
			closedir(dir);
			return true;
		}

		closedir(dir);
		return false;
	}
}


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



/* ************* index_build ******************** */
/* Takes in a directory name and initialised index.
 * The function then loops through all the files in the 
 * directory and creates a webpage structure for them.
 * The function then uses the webpage functions to read 
 * the words in the HTML code, counts them, and records it
 * in the index.
 *
 * Assumptions:
 *   * file names start from the integer 1, increase by 
 		1 for the next file name and there are no missing
 		integers between files (e.g. if there are supposed to
 		be 3 files in the directory, they are supposed to be 
 		numbered 1,2,3 and not 1,3,4 or any other combination).
 *   * user has already checked whether the file to be written to 
 *		and the index has been successfully initialised
 *	 * user has initialised the index with an appropriate size.
 *	 * files were written by the crawler module and the directory
 *	 	is an crawler created directory.
 * 
 * Pseudocode:
 *     1. Contantenate the directory name and file number to create
 * 		the full path name for the file.
 *	   2. Open the file and get its first line, which is assumed to be
 *		its url since the file was created by crawler
 *	   3. Create a webpage structure for it, fetch its URL and then call
 *		webpage_getNextWord to scan for the words in the webpage
 *	   4. Based on the words found, update the index accordingly.
 *	   5. Clean up the webpage, opened files and move onto the next file.
 */

void index_build (char *pageDirectory, index_t *index)
{
	// Get the filename to be opened by appending the number
	// to the pagedirectory.
	FILE *fileread;
	int filenumber = 1;
	char filename[strlen(pageDirectory) + 25];
	sprintf(filename, "%s/%d", pageDirectory, filenumber);
	fileread = fopen(filename, "r");

	while(fileread != NULL){

			char *url = readlinep(fileread);
			NormalizeURL(url);

			// If the file is not a blank file and the first line is an URL.
			if (url != NULL && IsInternalURL(url)){
				// initialise a webpage struct using the URL on the first line
				// of the file. Then fetch its HTML.
				webpage_t *webpage = webpage_new(url, 0, NULL);
				webpage_fetch(webpage);

				// Read the HTML for words
				int pos = 0;
				char *result;
				while ((pos = webpage_getNextWord(webpage, pos, &result)) > 0) {
					
					// ignore any words with 3 characters or less
					if (strlen(result) > 2){
						// find the word
						counters_t *cntr = index_find(index, NormalizeWord(result));
						if (cntr == NULL){
						// if it does not exist, create a counter structure and insert it
						// to the index
							cntr = counters_new();
							counters_add(cntr, filenumber);
							index_insert(index, NormalizeWord(result), cntr);
						}
				
						else{
							// if it exists, then just add it.
							counters_add(cntr, filenumber);
						}

					}
					// free the result and loop back to get the next word
					count_free(result);
					
				}

				// clean up for this webpage and file and then move on to the next.
				count_free(url);
				webpage_delete(webpage);			
				fclose(fileread);
				filenumber++;
				memset(filename, 0, strlen(pageDirectory) + 25);
				sprintf(filename, "%s/%d", pageDirectory, filenumber);
				fileread = fopen(filename, "r");
			}
	}
}


int main( int argc, char **argv )
{	
	if (argc == 3 && checkDir(argv[1])){
		
		// initialise the index and build it
		index_t *index = index_new(1001);
		index_build(argv[1], index);
		
		// initialise the file and write the index to it.
		FILE *towrite;
		towrite = fopen(argv[2], "w+");
		if (towrite == NULL){
			printf("failure to create file to write. \n");
			index_delete(index, itemdelete);
			exit(2);
		}

		// clean up
		index_save(towrite, index);
		fclose(towrite);
		index_delete(index, itemdelete);

	}

	else{
		printf("wrong argument parameters. \n");
		exit(1);
	}

	return 0;
}

