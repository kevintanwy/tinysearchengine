# CS50 Tiny Search Engine (TSE) querier implementation notes

Kevin Tan, June 2017

This is the querier module that supports the TSE project. 

## Implementation for querier
The querier runs as follows:

1. Execute from a command line as shown in the User Interface. 
2. Parse the command line, validate parameters by checking the argument count and the checkDirectory function (which checks if the directory is exists, is readable and was created by the crawler module). ry to open the file with the inverted index structure with the name given in argv[2] for reading. If there are any errors with the arguments, exit with status.
3. Initialise the index data structure with 0.75 times the number of words found in the index file. This is the same as Java's load factor and is optimised for both search speed and space. The index file is no longer needed so we can close it. 
4. Continually read stdin from the user:
	1. If the user inputs EOF (i.e. cntrl-d), exit the loop, delete the index and exit with status 0.
	2. Else, process the user input and split the line with " " as a delimiter to an array of words.
	3. Check the words for any non-alphabet characters or incorrect placements of "and"/"or". If there are errors, skip the rest of the steps and go back to the start of the loop.
	4. Create a final counters structure and a temporary (temp) counters structure. Union the first word's counters with temp and then loop through the rest of the words.
		1. If the word is "and", skip as we can treat sequences with no ands in them as an entire "and" sequence.
		2. If the word is "or", skip it for now.
		3. If the previous word is "or", flush the temp list into the final list by unioning both of them. Delete and reinitialise the temp list and union it with the counters of the current word.
		4. If the word is a regular word, treat it as a regular "and" sequence and intersect the word's counters with temp.
	5. At the end of the loop, flush temp to final one more time (by unioning both of them) and then delete temp.
	6. Create a new struct called node_counter. Store the final counter's maximum count in it and the crawler-created page directory's name in it.
	7. If node_counter's maximum count value is 0, that means the sequence that the user input was not found in the files. Skip to step 9.
	8. Loop through final counters and print the count, key and url of counters that have the same count as the node_counter's maximum count value. Then decrement node_counter's maximum count value and loop through the final counters again to print the count, key and url of counters that have the same count as the node_counter's current maximum count value. Repeat this process until node_counter's current maximum count value is 0. This way, the files will be printed in decreasing order of rank importance and files that do not have the word sequence are ignored.
	9. Clean up all the data structures that were created and loop back to start to receive the next user input.


## Assumptions
1. Webpages and directory are created by crawler module, which should leave a .crawler file.
2. Files saved by crawler follow the format as specified in the crawler requirements spec.
3. Files saved by crawler have a naming convention that starting with 1 and increases by 1 for the next file.
4. The output file after calling indexer will be as specified in the index requirement spec and indexer was called on the pageDirectory created by crawler. Within the file, the lines may be of any order and within a line the docIDs may be of any order.


## Testing plan
1. Use crawler to create a directory with a small number of files. Then immediately use indexer print the index to another file. Run querier with normal inputs.
2. Give an EOF input. Should exit with status 0.
3. Give querier wrong number of arguments. Should print an error message and exit with status 1.
4. Give querier a non-existent directory. Should exit with status 1.
5. Give querier a non-existent index file. Should exit with status 1.