# CS50 Lab 5
## TSE Indexer Implementation Spec
## Erich Woo - 13 May 2020

### Implementation

A compiled `indexer` runs as follows:

 1. execute command-line arguments
 2. parse the command-line, validate parameters
    1. if incorrect # of arguments, print error and exit
    2. if invalid/nonreadable `pageDirectory`, print error and exit
       1. uses `dir_exists` from `pagedir.c` module
    3. if invalid/nonwriteable `indexFilename`, print error and exit
       1. uses `path_valid` from `index.c` module
 3. initialize a hashtable representing the inverted-index
 4. use `index_build` from `index.c` to build index using `pageDirectory` and hashtable
    1. light error-checking of params
    2. initiate docID = 1
    3. build docID's *pathname* given `pageDirectory` and docID
    4. for each docID file in `pageDirectory` (while `fopen` on *pathname* is not NULL),
       1. open a reading *filepointer* on the file
       2. read and store the 1st line as the URL
       3. read and store the 2nd line as the depth
       4. read and store the rest of the file as the HTML
       5. make a *webpage* for the URL, depth, and HTML
       6. while the webpage has more words to scan,
       	  1. scan the next word in the webpage
          2. if the word is at least 3 characters long,
             1. use *NormalizeWord* to convert to lower case
             2. get the counterset for the word from the hashtable
                1. if no counterset found, create a new counterset for the word and insert it into the hashtable
             3. increment the count for the docID in the counterset
	     	1. implicitly initiates a new docID-count pair if it doesnt exist yet
       7. free unneeded malloc'd memory and close *filepointer*
       8. increment docID + 1
 5. use *index_save* to save the index/hashtable to `indexFilename`
    1. light error-checking of params 
    2. open a writing *filepointer* for `indexFilename`
    2. iterate over each word-counterset in the hashtable/index
       1. write the word to file
       2. iterate over the counterset of docID-count pairs
          1. write the docID-count pair to file
       3. write a newline to file
    3. close the *filepointer*
 6. free the hashtable

### Definition of functions (internal, helper, or other)

In `index.c`:

 1. *index_load* loads an old index file and generates the hashtable representation of its inverted-index. This function is implemented in `indextest.c`.
    * parses each line of `word docID count [docID count]`
    * for each line,
      * use `file.h` module to scan next word
      * create and keep track of a new counterset for word, insert into hashtable
      * scanf until no more `int int` pairs 
      	* for each pair, set/initiate a counter for the pair
 2. *write_wordcounter_pair* is an internal method implemented by *index_save*, which writes an individual word-counterset pair to a filepointer. It is called by `hashtable_iterate` as the function to iterate over, writing each word-counterset pair in index hashtable.
    * first writes the word 
    * then calls `counters_iterate` to iterate over the counterset
      * leverages *write_idcount_pair* as the function that `counters_iterate` iterates over to write each individual docID-count pair
    * writes a newline to complete the word's line
 3. *write_id_count_pair* is an internal method implemented by *write_wordcounter_pair* (and in turn by *index_save*), which writes an individual docID-pair to a filepointer 
 4. *path_valid* does readable or writeable validation for a specified filepath. Implemented by `indexer.c` to validate `indexFilename` parameter.
    *The specified filepath must already exist for a readable validation, but not for writeable validation.
    * If successful writeable validation, the file is created at specified filepath.

In `word.c`:

 1. *NormalizeWord* converts all letters in a word to lowercase. Ignores nonalphabetic characters. Implemented by *index_build* as described in Implementation.

In `pagedir.c`:

 1. *dir_exists* does readable or writeable validation for a specified directory. Implemented by *index_build* as dexcribed in Implementation.
    * writeable validation creates a `.crawler` file in specified directory if successful.
    * readable validation requires existing `.crawler` file in order to check readability.

 2. *numDigits* counts the number of digits in an integer. Implemented by *index_build* to convert an docID integer to a string through dynamic allocation of memory.
 
### Testing Plan

See DESIGN.md for indexer Testing Plan