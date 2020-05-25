# CS50 Lab 6
## TSE Querier Implementation Spec
## Erich Woo - 23 May 2020

### Implementation

 1. execute from command-line as shown in the User Interface
 2. use *validate* to parse the command-line, validate parameters 
    1. if incorrect # of arguments, print error and exit
    2. if invalid/nonreadable/noncrawler `pageDirectory`, print error and exit
       1. uses *dir_exists* from `pagedir.c` module
    3. if invalid/nonreadable `indexFilename`, print error and exit
       1. uses *path_valid* from `index.c` module
 3. use *index_init* to initialize a hashtable representing the inverted-index
    1. initialized with a load factor lambda ~0.75, n = lines in file
    2. uses *index_load* from `index.c` module
    3. prints error and exits if failed to load index
 4. use *query* to continuously query the user until EOF:
    1. in a while loop, use *prompt* to prompt the user for a query (only if a keyboard stdin), and retrieve the line with a file buffer. For each line,
       1. use *clean_query* to parse/clean and validate the query/line into a char* array words[]
       	  1. initialize a `temp*` pointer at the start of the line, which tracks the start of a new word
	  2. initialize an int index `i` which tracks where to place the next word into words[], also acts as the # of words in words[] after the most recent word has been placed
	  3. step through the line incrementing char* `cp` forward until `cp` reaches '\0'
	     1. if `cp` is whitespace, change it '\0'
	     	1. if `temp` points to 'and'/'or',
		   1. if it is the first word in line print error and return `i`, aka 0
		   2. if the previous word was also an 'and'/'or', print error and return `-1 * i`, negative for error and `i` for # of calloc'd words
		2. if `temp` isn't '\0', allocate and assign words[i] to `temp`
		3. while the char in front of `cp` is also whitespace, step `cp` forward
		4. set `temp` to the start of the next word, aka `cp + 1`
             2. else if `cp` isn't alphabetic, print error and return `-1 * i`
	  4. after `cp` reaches '\0', check if `temp*` is still point to a word, and if so, allocate and assign the last words[i] to `temp`
	  5. if the last word assigned was 'and'/'or', print error and return `-1 * 1`
	  6. At this point, only valid queries reach this point; print the clean query
	  7. return `i`
       2. if *clean_query* returned non-zero (not empty/error) and non-negative (no error), do the following
       	  1. use *satisfy_query* to satisfy the cleaned query, getting a `counters_t*` of doc-score pairs
	     1. Based on Backus-Naur Form, tracking with two `counters_t*`, a `or sequence1 (running total, will represent final result) and an `and sequence` (running product)
	     2. initiate a binary 1/0 int tracker `and_init` signifying whether/not to start a new `and sequence`.
	     3. for each word in words[] from left to right,
	     	1. if `and_init` = 1, initiate new `and sequence`, *merge* `and sequence` with the *counters_t* found in hashtable index of that word, and set `and_init` = 0
		2. else if word is 'or', *merge* `or sequence` with `and sequence`, delete/free the `and sequence`, and set `and_init` = 1
		3. else if word is NOT 'and', *intersect* `and sequence` with the *counters_t* found in hashtable index of that word
		4. do nothing if word is 'and'
	     4. *merge* `or sequence` with the final `and sequence`
	     5. delete/free the final `and sequence`
	     6. return the finalized `or sequence` 
       	  2. use *sort_scores* to rank the resulting counterset of doc-score in descending order
             1. iterate over counterset to get number of documents
	     	1. needed to allocate enough space for final doc-score array
	     2. initiate a *data_t* struct, allocating its *doc_score_t* array as a doublepointer to *doc_score_t* 
             2. iterate over counterset to place each doc-score into the *doc_score_t* array via *insertion sort* algorithm
       	  3. use *print_scores* to print the sorted scores to stdout
             1. loop over doc-score array and print each one
	  4. delete/free the 'raw results' counterset, and the sorted *data_t* struct
       3. if *clean_query* returned negative, turn it back positive
       4. free each word that was calloc'd in words[], based on value returned from *clean_query*
       5. free the calloc'd line
 5. delete/free hashtable

### Definition of extra functions not desribed above

 1. *merge* takes in two countersets. Iterates over the 2nd set, with the 1st passed in as data. Iterates with *merge_helper* as the helper function.
    1. *merge_helper* works with an individual doc-score pair from 2nd set, and the entire 1st set. Gets the 1st set's corresponding score according to the 2nd set's doc/key (may be 0 if not found). Resets the 1st set's corresponding score to the sum of the two set's scores, ONLY IF THE SUM IS NOT 0. Important *if* for my *intersect* function.
 2. *intersect* takes in two countersets. Initiates a new struct *both_t* which contains both countersets. Iterates over the 1st set, with *both_t* countersets passed in as data. Iterates with *intersect_helper* as the helper function.
    1. *intersect_helper* works with an individual doc-score pair from 1st set, and the entirety of both countersets. Gets the 2nd set's corresponding score according to the 1st set's doc/key (may be 0 if not found, thus no intersection). Resets the 1st set's corresponding score according to the minimum score between the two sets, EVEN IF THE MINIMUM IS 0. Which means, the 1st set may contain doc-0 pairs within it after calling *intersect*.

Important for understanding the functionality passed between *merge* and *intersect*: because *intersect* is only called on the `and sequence` (not the result/or sequence), only the current `and sequence` will possibly contain doc-0 pairs. Because of that, when *merge*-ing happens between `or sequence` (1st set) and `and sequence` (2nd set) aka 'uploading' the 'and's to the result, *merge* will weed out these doc-0 pairs in the 2nd set, and only 'upload' non-zero pairs; thus, the updated `or sequence` will be clean!

### Testing Plan

See DESIGN.md for querier Testing Plan.