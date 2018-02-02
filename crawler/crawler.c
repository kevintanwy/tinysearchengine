
/* 
 * crawler.c - CS50 'crawler' module
 *
 *
 * Kevin Tan, CS50 Lab 3, July 2017
 * Done with reference to bag.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <assert.h>
#include "hashtable.h"
#include "memory.h"
#include "webpage.h"
#include "set.h"
#include "bag.h"
#include "../common/pagedir.h"

#define MAXINPUTDEPTH 20 // in case user gives too large of an input


/* ***************************************** */
/* Private methods */
static bool checkArg (int argno, char *url, char *directory, char *depth);
static bool checkEmptyDir (char *directory);
static bool isPositiveInteger(char number[]);
static void urldelete(void *item);


/* *********************************************************************** */
/* Public methods */
bool crawler(char *seed, char *directory, int maxdepth, void (*itemdelete)(void *item));


/* ************* checkArg ******************** */
/* Checks the all the argument inputs. Calls the helper functions
 * isInternalURL, checkEmptyDir and isPositiveInteger to check the
 * arguments. Method reads arguments from left to right and will
 * return false once it encounters one bad argument
 *
 * Limitations:
 * If multiple bad arguments are given, then only the user will only
 * know the first argument from the left was bad.
 * 
 * Pseudocode: 
 * 1. Check for argument number. If anything other than 3, return false.
 * 2. Check if the URL is internal. If not return false.
 * 3. Check if the directory exists and empty. If not return false.
 * 4. Check if the depth is a positive integer. If not return false.
 * 5. If the above conditions are true, return true.
 */

static bool checkArg (int argno, char *url, char *directory, char *depth)
{
	if (argno == 4){
		if (NormalizeURL(url) && IsInternalURL(url)){

			if (checkEmptyDir(directory)){
				
				if (isPositiveInteger(depth)){
					return true;
				}
				printf("Depth is not a positive integer. \n");
			}

			else{	
				printf("Directory does not exist or is not empty. \n");
			}
		}

		else {
			printf("URL is not internal. \n");
		}

	}

	else{
		printf("Argument number is wrong. \n");
	}
	
	return false;
}


/* ************* checkEmptyDir ******************** */
/* Helper function to check if the directory exists, is writable and is empty.
 * done with reference to:
 * https://stackoverflow.com/questions/6383584/check-if-a-directory-is-empty-using-c-on-linux
 * 
 * Pseudocode: 
 * 1. Check if the directory exists and writable. If not return false.
 * 2. Check if the directory is empty by counting the number of files
 * in them. An empty directory should only have .  and .. files at most.
 * 3. If the count of the directory less than 2. If not, return false.
 * 4. Return true if all the conditions fufilled.
 */

static bool checkEmptyDir (char *directory)
{
	int n = 0;
  	struct dirent *d;
  	DIR *dir = opendir(directory);

	if (dir == NULL && access(directory, W_OK) != 0) //Not a directory, doesn't exist or is not writable
    	return false;
  	while ((d = readdir(dir)) != NULL) {
    	if(++n > 2)
      	break;
 	}
  	closedir(dir);
  	if (n <= 2) //Directory Empty
    	return true;
  	else
    	return false;
}


/* ************* isPositiveInteger ******************** */
/* Helper function to check if a given string is a positive integer.
 * 
 * Pseudocode: 
 * Iterate through all the characters and check if they are digits.
 * If they are, then the string is a positive integer.
 */

static bool isPositiveInteger(char number[])
{

	// Check if the string contains a non-digit character.
	for (int i = 0; number[i] != 0; i++){
        if (!isdigit(number[i])){
            return false;
        }
	}
	return true;
}


/* ************* urldelete ******************** */
/* Item delete function for hashtable.
*/
static void urldelete(void *item)
{
  if (item) {
    free(item);
  }
}

/* ************* crawler ******************** */
/* Takes in the seed url, directory name, maxdepth and an item
 * delete function and then initialises the bag and hashtable 
 * to be used by pagescanner and pagesaver to loop through the 
 * items in the bag and write them to the page. Upon successful
 * completion of the write, return true. If there was
 * an error in the creation of the bag and hashtable,
 * return false.
 *
 * Assumptions:
 *   * valid integer is given for max depth
 *   * user has already check whether the seed and directory
 *	   are valid
 * 
 * Pseudocode:
 *     1. Initialise the hashtable and bag
 *	   2. Check whether the bag and hashtable have been
 *			properly initialised. If not return false.
 *	   3. Initialise the webpage item for the seed url and
 *		  insert it into the bag.
 *	   4. Add the url to the hashtable.
 *	   5. Loop through the bag and call the pagescanner and
 *	      pagesaver modules
 *	   6. Delete all the items in the bag and hashtable when
 *		  done.
 */
bool crawler(char *seed, char *directory, int maxdepth, void (*itemdelete)(void *item))
{
	// initialise bag, hashtable and a webpage struture.
	hashtable_t *visited;
	visited = hashtable_new(101);
	bag_t *urls;
	urls = bag_new();

	// create a .crawler file
	char filename[strlen(directory) + 20];
	sprintf(filename, "%s/.crawler", directory);
	FILE *fp = fopen(filename, "w+");
	fclose(fp);


	if (visited == NULL || urls == NULL){
		printf("bag_new and/or hashtable_new failed \n");
		return false;
	}

	// initialise the seed webpage and add it to the bag and the hashtable
	webpage_t *webpage;
	webpage = webpage_new(seed, 0, NULL);
	bag_insert(urls, webpage);
	
	char *urlstring = malloc(strlen(seed) + 1);
    strcpy(urlstring, seed);
    hashtable_insert(visited, seed, urlstring);
	
	int documentid = 1;
	webpage_t *current = bag_extract(urls);
	

	while (current != NULL){

		pagescanner(current, maxdepth, visited, urls);

		pagesaver(directory, documentid, current);
			
   		documentid++;
   		webpage_delete(current);
   		current = bag_extract(urls);

	}

	// delete the bag & hashtable
	hashtable_delete(visited, itemdelete);
	bag_delete(urls, webpage_delete);
	return true;
}


int main(int argc, char **argv)
{	
	
	// Check if the rest of the arguments are correct. If not exit.
	if (!checkArg(argc, argv[1], argv[2], argv[3])){
		exit(1);
	}

	// Ensure that the user does not input too huge a depth.
	// Reset the max depth if necessary.
	int maxdepth;;
	maxdepth = atoi (argv[3]);
	if (maxdepth > MAXINPUTDEPTH){
			maxdepth = MAXINPUTDEPTH;
	}


	// If there is anything wrong with the hashtable_new or
	// bag_new function, exit with error code 2.
	if (!crawler(argv[1], argv[2], maxdepth, urldelete)){
		exit(2);
	}
	
	return 0;

}