/* 
 * pagedir - utility functions for adding pages to a bag and hashtable, writing an
 			 extracted page into a file and a function to fetch a webpage.
 *           See pagedir.h for usage.
 *
 * Kevin Tan, July 2019 for tse lab, CS50
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/memory.h"
#include "../libcs50/webpage.h"
#include "../libcs50/set.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"

void pagescanner(void *current, int maxdepth, void *hashtable, void *bag);
void pagesaver(char *dirname, int documentid, void *current);
bool pagefetch(void *webpage);

/* ************* pagescanner ******************** */
/* Scans a webpage's HTML code for URLs to other webpages.
 *
 * Assumptions:
 * 1. current is a valid webpage object
 * 2. maxdepth is greater than 0
 * 3. result is null and will be allocated internally
 *
 * Pseudo-code:
 * 1. Initialise pos and result.
 * 2. Call the get_NextURL function, normalise the URL, check if it is internal
 *    and check if it is not in the hashtable.
 * 3. If the above conditions hold, then insert this new URL to the hashtable.
 * 4. Create a new_webpage structure and insert this webpage into the bag with
 *    its depth as the current webpage's depth + 1
 * 5. Free result.
 * 
*/

void pagescanner(void *current, int maxdepth, void *hashtable, void *bag)
{
	if(pagefetch(current) && webpage_getDepth(current) < maxdepth) {
			
			int pos = 0;
			char *result;

			while ((pos = webpage_getNextURL(current, pos, &result)) > 0) {
      			// Check if the url already exists in the hashtable. If not add it to the hashtable and add it
      			// to the bag

      			if (NormalizeURL(result) && IsInternalURL(result) && hashtable_find(hashtable, result) == NULL){
      				
      				char *toinsert = malloc(strlen(result) + 1);
      				strcpy(toinsert, result);
      				hashtable_insert(hashtable, result, toinsert);

      				webpage_t *newwebpage;
      				newwebpage = webpage_new(result, webpage_getDepth(current) + 1, NULL);
      				bag_insert(bag, newwebpage);

      			}

      			free(result);

  			}

		}

}


/* ************* pagesaver ******************** */
/* Creates a file and writes the contents of the  
 * webpage into it. The filename is expected to be 
 * an integer. The file will then create the file 
 * in the directory specified.
 *
 * Limitations:
 * 1. Can only accept documentids that have less than 24 digits
 * 2. User must give valid directory path.
 *
 * Pseudo-code:
 * 1. Allocate memory for an array for the directory and filename.
 * 2. Create the file with the filename and open it for writing.
 * 3. Get the webpage's URL and HTML and check that they are not null.
 * 4. Print the webpage's url as the first line
 * 5. Print the webpage's depth as the second line
 * 6. Print the rest of the HTML code.
 * 7. Close the file.
 * 
*/

void pagesaver(char *dirname, int documentid, void *current)
{
	FILE *fp;

	char filename[strlen(dirname) + 25];
	sprintf(filename, "%s/%d", dirname, documentid);
	fp = fopen(filename, "w+");
	if (fp!=NULL){
				
		// Write to file.
		char *currURL = webpage_getURL(current);
		char *currHTML = webpage_getHTML(current);

		if (currURL != NULL && currHTML != NULL){
			fputs(webpage_getURL(current),fp);
    		fputc('\n', fp);
     		fprintf(fp, "%d", webpage_getDepth(current));
     		fputc('\n', fp);
     		fputs(webpage_getHTML(current), fp);
		}

		else{
			printf("URL or HTML are empty. %d skipped. \n", documentid);

		}
     			
    fclose(fp);
	}

	else{
		printf("Error in making file for %d. \n", documentid);
	}
}

/* ************* pagefetch ******************** */
/* Calls the webpage_fetch function.
 * see webpage.h for usage documentation.
 * As taken from webpage.c documentation:
 * Limitations:
 *   * can only handle http (not https or other schemes)
 *   * can only handle URLs of form http://host[:port][/pathname]
 *   * cannot handle redirects (HTTP 301 or 302 response codes)
 * 
 * Pseudocode:
 *     1. check for valid page 
 *     2. parse url into hostname, port, and filename
 *     3. open a connection to the given host
 *     4. send http request
 *     5. fetch html response
 *     6. cleanup
 */


bool pagefetch(void *webpage)
{
	return webpage_fetch(webpage);
}
