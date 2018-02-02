# CS50 Tiny Search Engine (TSE) crawler implementation notes

Kevin Tan, June 2017

This is the crawler module that supports the TSE project. 

## Implementation
The crawler runs as follows:

1. Execute from a command line as shown in the User Interface. 
2. parse the command line, validate parameters with the checkArg function. Then check if the maxdepth parameter is too large and limit it if necessary. If there are any errors with the arguments, exit with status 1.
3. Call the crawler function to initialise the pagedir functions.
	1. make a webpage for the seedURL, marked with depth=0
	2. add that page to the bag of webpages to crawl
	3. add that URL to the hashtable of URLs seen. Size of 101 chosen because of potentially many webpages that might be seen. Hashtable used because checking if the files have already been added is faster. Item for the hashtable entry will be the same as the key. IF there are errors in the bag and hashtable function, return false here and print the error to user interface.
	4. while there are more webpages to crawl,
		
		1. Give the webpage struct, hashtable, bag and maxdepth parameters to the function pagescanner.
		2. Check if the there are URL links in the HTML code and if webpage depth < maxdepth, for every URL found, it will check if it is a valid URL by first normalizing it and using the isInternalURL function and will check if already exists in the hashtable. webpage_fetch is called here which gives the 1 second pause before the server is requested for further requests.
		3. Ignore the URL if it is not valid, not internal or already exists in the hashtable.
		4. If it is a valid URL and already exists in the hashtable, create a new webpage struct for this webpage and add it into the bag with depth equals to the currently extracted webpage's depth  + 1. Create a new hashtable entry with this URL.
		5. Loop back to 4.2 for the next URL, until there are no more URLs.

	
		6. After pageScanner, we now call pageSaver. We have already checked if the items in the bag are less than max depth when we added them so we don't have to check them again when we extract them to write to the file. This function takes the directory name argument, a document id argument, and a webpage to write.
		7. create a new file in the directory the user gives. I use the sprints function to contantante the directory name and document id number before calling fopens.
		8. Check if fopens is able to create the file
		9. If not, log the error by printing to the user interface that file failed to be created for document id number and move onto the next iteration of the loop.
		10. Call webpage_getURL and webpage_getHTML to get the webpage's URL and HTML.
		11. If they are null, log the error by printing to the user interface that either one of them was empty.
		12. Put the URL on the first line, depth on the second, and HTML on the 3rd.
		13. close the file

	5. Delete all the items in the bag and hashtable before deleting the bag and hashtable themselves by calling the bag_delete function and hashtable_delete function. Return true.

4. If there were errors in the hashtable and bag creation in crawler, exit the with status 2 as the rest of the programs rely on the creation of these 2 structures. Else return 0.

