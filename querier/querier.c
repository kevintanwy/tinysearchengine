
/* 
 * querier.c - CS50 'querier' module
 *
 *
 * Kevin Tan, CS50 Lab 6, Aug 2017
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
#include "file.h"
#include "counters.h"
#include "../common/index.h"
#include "../common/pagedir.h"


/* ***************************************** */
/* Data structures */
/* For counters union and intersect*/
struct two_counters {
	counters_t *dstn;
	counters_t *src;
};

/* For ranking the counter nodes*/
struct node_counter{
	int max_count;
	char *pageDir;
};

/* ***************************************** */
/* Private methods */
static bool checkDir (char *directory);
static bool checkStdin (char **words, int arraysize);
static void itemdelete(void *item);
static int processQuery(char *line, char **words);
static void counters_intersect (counters_t *dstn, counters_t *src);
static void counters_intersect_helper(void *arg, int key, int count);
static void counters_union (counters_t *dstn, counters_t *src);
static void counters_union_helper(void *arg, int key, int count);
static void counters_max(void *arg, const int key, int count);
static void rank_counters(void *arg, const int key, int count);
static void setFinalCounters(index_t *index, counters_t *temp, counters_t *final, char **words, int wordcount);
static void setNodeCounter(struct node_counter* nc, char *pageDir, counters_t *final);
static void printRank(struct node_counter *nc, counters_t *final);


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
static bool checkDir (char *directory)
{
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


/* ************* processQuery ******************** */
/* Takes in a character string and an appropriately sized character pointer array.
 * The function breaks the character string up and puts the words into the array 
 * and returns how many words there are in the array.
 * 
 * Limitations:
 * 1. User needs to provide a character array with a large enough size such that the 
 * array will not overflow and cause segmentation fault.
 * 
 * Pseudocode:
 * 1. Break up the line using " " as a delimiter.
 * 2. Insert the normalised words into the array until there are none left. Keep
 * increasing the counter for the number of words accordingly.
 * 3. At the end of the loop, adjust the counter for the number of words and return it.
 */
static int processQuery(char *line, char **words){
// break line up and store in array
	char *word = strtok(line, " ");
	int i = 0; 						// number of words
	while (word != NULL){
		words[i] = NormalizeWord(word);
		i++;
		word = strtok(NULL, " ");
	}
	i--;
	return i;
}


/* ************* checkStdin ******************** */
/* Checks if stdin contains invalid input. Returns true if all the input
 * is valid and false if not. Takes in an array of words and the size of the
 * array.
 * 
 * Limitations:
 * 1. If an non-alphabet character is encountered, the user its given its position
 * based on the array starting from position 0.
 * 2. To keep the code short, the code does not tell the user exactly "and" or "or" 
 * was at the start/end of the code or was seen in consecutive sequence.
 * 
 * Pseudocode:
 * 1. Iterate through all the words in stdin
 * 2. Check if the word contains a non-alphabet character. If so, return false.
 * 3. Check that if the word "and"/"or" is encountered, it never appears at the
 * start or end of the stdin and never appears more than once in a row. If this
 * fails, return false.
 */
static bool checkStdin (char **words, int arraysize)
{
	// check the words.
	for (int j = 0; j <= arraysize ; j ++){
		char *totest = words[j];

		// verify that the words only contain alphabet characters.
		for (int k = 0; k < strlen(totest); k++){
			if (!isalpha(totest[k])){
				printf("non-alphabet character detected at position %d \n", k);
				return false;
				}
			}

		// verify that and/or appears only in the middle of stdin and never
		// occurs twice or more in a row.
		if (strcmp(totest, "and") == 0 || strcmp(totest, "or") == 0){
			if ((j == 0) || (j == arraysize)){
				printf("and/or cannot be the first or last word \n");
				return false;
			}
						
			else if (strcmp("and", words[j + 1]) == 0 || strcmp("or", words[j + 1]) == 0){
				printf("and/or cannot be two consecutive words \n");
				return false;
			}						
		}
	}
	return true;
}




/* ************* counters_union ******************** */
/* Assign the two_counters pointers to the destination and source counters and then
 * pass it into the counters_iterate function that will also use counters_union_helper
 * to get the union of the two counters. For a detailed explaination on what union
 * is supposed to achieve, see the querier requirements spec.
 * 
 * Limitations:
 * 1. valid counters must provided as input.
 * 
 * Pseudocode:
 * 1. Assign the pointers in two_counters to dstn and src.
 * 2. Iterate through source and with the help of counters_union_helper,
 * check if dstn has the key and modify the count accordingly.
 */
static void counters_union (counters_t *dstn, counters_t *src) 
{
	struct two_counters twocntrs = {dstn, src};
	counters_iterate(src, &twocntrs, counters_union_helper);
}


/* ************* counters_union_helper ******************** */
/* Helper function to counters_union_helper

 * Limitations:
 * 1. void*arg is assumed to be the two_counters struct that holds
 * pointers to two valid counters.
 * 2. Assume that we are iterating through the src counter structure.
 * 
 * Pseudocode:
 * 1. Get the count of the dstn counter for this key. 
 * 2. If it is 0, call counters_set for the key in dstn and fill
 * it with the count from src.
 * 3. If it is non-0, call counters_set for the key in dstn and fill
 * it wih the sum of the old value and the count from src.
 */
static void counters_union_helper(void *arg, int key, int count)
{
	struct two_counters *twocntrs = arg;
	int dstn_count = counters_get(twocntrs->dstn, key);
	if (dstn_count == 0){
		counters_set(twocntrs->dstn, key, count);
	}
	else{
		counters_set(twocntrs->dstn, key, dstn_count + count);
	}
	
}

/* ************* counters_intersects ******************** */
/* Assign the two_counters pointers to the destination and source counters and then
 * pass it into the counters_iterate function that will also use counters_intersect_helper
 * to get the intersect of the two counters. For a detailed explaination on what intersect
 * is supposed to achieve, see the querier requirements spec.
 * 
 * Limitations:
 * 1. valid counters must provided as input.
 * 
 * Pseudocode:
 * 1. Assign the pointers in two_counters to dstn and src.
 * 2. Iterate through dstn and with the help of counters_union_helper,
 * check if src has the key and modify the count for dstn accordingly.
 */

static void counters_intersect (counters_t *dstn, counters_t *src) 
{
	struct two_counters twocntrs = {dstn, src};
	counters_iterate(dstn, &twocntrs, counters_intersect_helper);
}


/* ************* counters_intersect_helper ******************** */
/* Helper function to counters_intersect_helper

 * Limitations:
 * 1. void*arg is assumed to be the two_counters struct that holds
 * pointers to two valid counters.
 * 2. Assume that we are iterating through the dstn counter structure.
 * 
 * Pseudocode:
 * 1. Get the count of the src counter for this key. 
 * 2. If it is smaller than dstn count, that means it is the smaller of the
 * two and so set destn's counters to this smaller value.
 * 3. Else, we know that dstn's count is smaller and do nothing.
 */

static void counters_intersect_helper(void *arg, int key, int count)
{
	struct two_counters *twocntrs = arg;
	if (counters_get(twocntrs->src, key) < count){
		counters_set(twocntrs->dstn, key, counters_get(twocntrs->src, key));
	}
	
}


/* ************* setFinalCounters ******************** */
/* Function that modifies one of the two counter structures given as parameters to be the
 * final coutners list. To do that, the function also needs a valid word array and an integer 
 * with the number of words in the array and an index created by indexer.c.
 *
 * Limitations:
 * 1. The temp counters is deleted at the end of the function.
 * 2. I am assuming that the index and word array are valid and non-empty.
 * 3. wordcount must be accurate for the loop to work properly.
 * 
 * Pseudocode:
 * 1. Union the temp counter with the counter for the first word.
 * 2. Loop through all the words.
 * 3. If the word is "and"/"or", skip to the next word. We can ignore "and" because when we 
 * perform the intersect function, it is the same as when "and" is present and when it is not.
 * 4. If the previous word was "or", then we flush the temp list to the final list and 
 * reinitialise the temp list for this new "and" sequence of words.
 * 5. Else just perform a normal counters_intersect for the current sequence of words which
 * is assumed to be an "and" sequence.
 * 6. Once there are no more words to be read, flush the temp list to the final list again and 
 * then delete the temp counters.
 */
static void setFinalCounters(index_t *index, counters_t *temp, counters_t *final, char **words, int wordcount)
{
	counters_union(temp, index_find(index, words[0]));
	for (int j = 1; j <= wordcount; j++){
		if (strcmp(words[j],"and")  == 0 || strcmp(words[j],"or") == 0){
			continue;
		}

		// flush temp list to final list.
		else if (strcmp(words[j - 1],"or") == 0){
			counters_union(final, temp);
			counters_delete(temp);
			temp = counters_new();
			counters_union(temp, index_find(index, words[j]));
		}
					
		else{
			counters_intersect(temp, index_find(index, words[j]));
		}
					
	}
	counters_union(final,temp);
	counters_delete(temp);
}


/* ************* setNodeCounter ******************** */
/* Sets the values for a newly initialised node_counter structure. The struct
 * is meant to hold the value of the maximum count of the given counters structure
 * and the string of the page directory string where all the files are stored.
 * Method calls counters_iterate which needs the helper function counters_max
 *
 * Limitations:
 * 1. User must initialise the node_counter structure before passing it into the function.
 * 2. User provides a valid page directory string.
 * 
 * Pseudocode:
 * 1. Initialise the max_count variable in node_counters to be 0 and the pageDir variable to
 * the string that the user provides.
 * 2. Call counters_iterate to set the max_count to the maximum count of the given
 * coutners structure.
 */
static void setNodeCounter(struct node_counter* nc, char *pageDir, counters_t *final)
{
	nc->max_count = 0;
	nc->pageDir = pageDir;
	counters_iterate(final, nc, counters_max);
}


/* ************* counters_max ******************** */
/* Helper function to setNodeCounter. Sets nodeCounter's max_count to the maximum of the
 * count of the given counter structure.
 * 
 * Pseudocode:
 * 1. Check if the current count is greater than the max_count of the node_counter structure.
 * If so, reset the max_count to this count value.
 */
static void counters_max(void *arg, const int key, int count)
{
	struct node_counter *nc = arg;
	if (count > nc->max_count){
		nc->max_count = count;
	}
}


/* ************* counters_max ******************** */
/* Function that prints the rank of the counters in decreasing order with the
 * use of a node_counter structure that has the max_count set to the maximum
 * count of the counters structure that is also given in this array.
 * 
 * Limitations: 
 * 1. Might be slow for large number of keys that do not have their counts
 * relatively close to each other.
 * 2. max_count of node_counter must have already been set to the maximum
 * count of the counters structure.
 * 
 * Pseudocode:
 * 1. Check if the max_count is more than 0. If it is 0 or less, that means
 * there were no matches.
 * 2. If the max_count is more than 0, then iterate through the counters with
 * the help of rank_counters function that will print all counters with the current
 * rank. Then the count decreases by one and the function iterates through all the counters again
 * to print the counters with this new rank. Repeat this process until the node_counter's
 * max_counters have reached 0.
 */
static void printRank(struct node_counter *nc, counters_t *final)
{
	if (nc->max_count>0){
		while (nc->max_count > 0){
			counters_iterate(final, nc, rank_counters);	
			nc->max_count--;
			}
		}

	else{
		printf("No matches found \n");
	}
}



/* ************* rank_counters ******************** */
/* Helper function to counters_max. If the counter has the same count as the
 * current value of max_count, the function prints the count, the key and the
 * URL out.
 * 
 * Limitations: 
 * 1. Might be slow for large number of keys that do not have their counts
 * relatively close to each other.
 * 2. Assume that the files are able to be opened so that the URL can be obtained.
 * 3. Assume that the user has verified that the file directory string in node_counters
 * was a valid file directory.
 * 4. Assume that the first line of the opened file is the URL.
 * 
 * Pseudocode:
 * 1. Check if the count is the same as the current value of node counter's max_count.
 * 2. If so, print out the score and key.
 * 3. Open the file with the directory name and key name to get the URL and print it out.
 * 4. Clean up.
 */

static void rank_counters(void *arg, const int key, int count)
{
	struct node_counter *nc = arg;
	if (count == nc->max_count){
		// Print score and document number.
		printf("score %3d doc %3d ", count, key);

		// Get URL and print it out.
		char filename[strlen(nc->pageDir) + 25];
		sprintf(filename, "%s/%d", nc->pageDir, key);
		FILE *fp = fopen(filename, "r");
		assertp(fp, "Failed to open file \n");
		char *line = readlinep(fp);
		printf("%s \n", line);

		// clean up.
		count_free(line);
		fclose(fp);
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


				
int main(int argc, char ** argv){
	if (argc == 3 && checkDir(argv[1])){
		FILE *indexfile;
		indexfile = fopen(argv[2], "r");

		if (indexfile != NULL){

			// Initialise the index
			index_t *index;
			int lines = lines_in_file(indexfile);
			index = index_new(0.75 * lines);		// Same as Java's load factor
			index_load(indexfile, index);
			fclose(indexfile);

			char *line;
			while(1){

				// Check for EOF
				printf("Query: \n");
				line = readlinep(stdin);
				if (line == NULL){
					printf("exiting... \n");
					break;
				}
				
				// Process the line into the word array
				int length = strlen(line);
				char *words[length];
				int wordcount = processQuery(line, words);
				if (wordcount < 0){
					count_free(line);
					continue;
				}
				
				// Check if the word array is valid. If not go to the next
				// iteration of the loop.
				if (!checkStdin(words, wordcount)){
					count_free(line);
					continue;
				}

				// Initialise the two new counters and modify one of them to be 
				// the final counters.
				counters_t *temp = counters_new();
				counters_t *final = counters_new();
				setFinalCounters(index, temp, final, words, wordcount);

				// Get the max_count of the final counters struct and print the rank.
				struct node_counter *nc = malloc(sizeof(struct node_counter));
				setNodeCounter(nc, argv[1], final);
				printRank(nc, final);
			
				//clean up
				count_free(nc);
				counters_delete(final);
				count_free(line);
				
			}
			
			// clean up in the event of EOF
			index_delete(index, itemdelete);
		}

		else{
			printf("invalid file \n");
			exit(1);
		}
	}

	else{
		printf("invalid argument parameters \n");
		exit(1);
	}
	return 0;
}