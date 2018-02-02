# CS50 Tiny Search Engine (TSE) indexer implementation notes

Kevin Tan, June 2017

This is the indexer module that supports the TSE project. 

## Implementation for indexer
The indexer runs as follows:

1. Execute from a command line as shown in the User Interface. 
2. Parse the command line, validate parameters by checking the argument count and the checkDirectory function (which checks if the directory is exists, is readable and was created by the crawler module). If there are any errors with the arguments, exit with status. Try to open the first file with the name given in argv[1] for reading and open/create the second file for writing with the name given in argv[2]. If there are any errors in the file opening process, exit with status 2.
3. Initialise the index data structure with a large number of slots (1001) to accomodate the possibility of large number of files and words being processed.
4. Call the index_build function to build this index with the file that is being read.
	1. Keep a counter of the current file number and contenate the directory name with this number to get the path to get to this file.
	2. Check if it is possible to open this file for reading, else the loop will skip to the next file number.
	3. Assuming that the file and directory was created by the crawler module, we know that the URL will be the first line of the file. Nevertheless, check for any errors in writing by checking if the first line is a non-empty and contains valid URL. If not, move onto the next file.
	4. Initialise a webpage structure with the URL and insert 0 as depth and NULL as HTML as they are not necessary information for this initiation.
	5. Assuming that there is an internet connection, call the webpage_fetch function to fill the webpage->html with the HTML of the code. Then we call the webpage_NextWord function to read the words in the file.
	6. While there are more words in the webpage:
		1. Ignore any words with less than 3 characters.
		2. Normalize the word and try to find its counters structure in the index.
		3. If it does not exist, then create one for the word, add one count to it for the existing file key, and then insert it to the index. If it already exists, then add one count to it for the existing file key.
		4. Free the word and move onto the next word (go back to 6.1).
	7. Free the webpage created, free the URL, close the file being read and clear the array that was used to hold the path name to the file. Increment the file number and contentanate the directory name with this number and try to open this file. If it is able to open, loop back to 4.3.
5. Now after all the words have been accounted for, write the index to the file with argv[2] as its name with the index_save function.
6. Close the file and free the memory allocated for the index.

## Implementation for index
1. Index is implemented as a special version of hashtable. The keys are words and the items are counters which keys represent the file number and count represent the number of times that particular word appears in that file.
2. Most of the functions return what hashtable would return, with the exception of two additional functions index_load and index_save.
3. index_load takes in a pointer to an opened file that can be read that contains the inverted-index and an already initialized index. The onus is on the caller to initialise both and check that both have been intialised properly. They will have to delete the index and close the file properly too. The method repopulates the index with the keys and counters as found in the file.
4. index_save calls index_iterate to print out the inverted index data structure onto an output file. It takes in a pointer to an opened file that can be written and an index that will be written to the file. The onus is on the caller to initialise both and check that both have been intialised properly. They will have to delete the index and close the file properly too. The method gives index_iterate a print function called counters_print, which in turn iterates through the counters for a given key to print the key on each line and which in turn is given another print function called simple_print to print out all the counters for that key. The resulting file contaisn the inverted index data structure.


## Assumptions
1. Webpages and directory are created by crawler module, which should leave a .crawler file.
2. Files saved by crawler follow the format as specified in the crawler requirements spec.
3. Files saved by crawler have a naming convention that starting with 1 and increases by 1 for the next file.
4. The output file after calling index_save will be as specified in the index requirement spec. Within the file, the lines may be of any order and within a line the docIDs may be of any orde.r
5. There is an available server connection so that webpage_fetch will be able to fetch the HTML code of the url to be stored in the webpage data structure as described in "Implementation for indexer" step 4.5.


## Testing plan
1. Use crawler to create a directory with a small number of files. Then immediately use indexer print the index to another file. Use indextest to load this index from the file and output it to another file. Sort the data in the files and output the sorted data into a separate file. Use diff to compare the two files. There should be no difference.
2. Give indexer wrong number of arguments. Should print an error message and exit with status 1.
3. Give indexer a non-existent directory. Should exit with status 1.
4. Delete the .crawler file and give this directory to indexer. Should exit with status 1.