# CS50 Lab 5
## TSE Indexer Design Spec
## Erich Woo - 13 May 2020

### Indexer

The `indexer` builds an inverted-index data structure from a set of correctly-formatted documents in a specified directory, and writes this index to a specified file.

### User Interface

The indexer's only interface with the user is on the command-line; it must always have two arguments:

```bash
$ indexer pageDirectory indexFilename
```

In order to proceed with the program, there must be two arguments, `pageDirectory` must be a valid and readable directory, and `indexFilename` must be a valid and writable pathname.

### Inputs and Outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: After building the inverted-index from given webpages in the `pageDirectory`, we save the index data to a file called `indexFilename`. Within the file, we write

* one word per line, one line per word
* each line follows the syntax:
* word docID count [docID count]...
* where *word* is a string of lower-case letters,
* where *docID* is a positive non-zero integer,
* where *count* is a positive non-zero integer,
* where *word*, *docID*, and *count* are separated by spaces.

Within the file, the lines may be in any order. Within each line, the docID-count pairs may be in any order.

### Functional Decomposition into Modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *index_build*, which builds the index from a directory of crawler-created files
 3. *index_save*, which writes the index data structure into the appropriate file
 4. *NormalizeWord*, which converts all letters in a word to lower case

And some helper modules that provide data structures:

 1. *hashtable* of each word-counterset, representing the inverted index
 2. *counterset* of docID-count pairs for each word

### Pseudo-code for Logic/Algorithmic FLow

The indexer will run as follows:

 1. execute from a command line as shown in the User Interface
 2. parse the command-line, validate parameters
 3. initialize a hashtable representing the inverted-index
 4. use *index_build* to build the index using `pageDirectory`
    1. for each docID file in `pageDirectory`
       1. scan the file and extract URL, depth, and HTML
       2. make a *webpage* for the URL, depth, and HTML
       3. while the webpage has more words to scan,
       	  1. scan the next word in the webpage
 	  2. if valid word, *NormalizeWord*
	  3. find word in hashtable and increment docID's count in the counterset
	     1. create new counterset for word and insert into hashtable if not found
 5. use *index_save* to save the index/hashtable to `indexFilename`
    1. iterate over each word-counterset in the hashtable/index
       1. write the word to file
       2. iterate over the counterset of docID-count pairs
       	  1. write each docID-count pair to file
       3. write a newline to file
 6. delete/free the hashtable

### Dataflow through Modules

 1. *main* parses parameters and passes them to *index_build* and *index_save*
 2. *index_build* builds the index from `pageDirectory` and the hashtable/index
    1. uses *NormalizeWord* to cleanup words before inserting into hashtable
 3. *index_save*, writes the hashtable/index into `indexFilename`
 4. *NormalizeWord* converts all letters in a word to lower case

### Major Data Structures

Three helper modules provide data structures:

 1. *hashtable* of words maping to counterset (docID, count) structures
 2. *set* of words (indirectly used by hashtable)
 3. *counters* of docIDs (docID, count)
 
### Testing Plan

*Integration testing*. Assemble the indexer and indextest and test it as a whole using `testing.sh`

For *indexer*,

* error check command-line arguments
* Test on letters-depth-0/ thru letters-depth-5/
  * should create correct and correctly-formatted letters-index-0 thru letters-index-5
* Test on toscrape-depth-0/ thru toscrape-depth-2/
  * should create correct and correctly-formatted toscrape-index-0 thru toscrape-index-2

For *indextest*,

* error check command-line arguments
* Use created index files from *indexer* testing to test letters-index-0 thru letters-index-5
  * should create correct and correctly-formatted letters-test-0 thru letters-test-5
* Use created index files from *indexer* testing to test toscrape-index-0 thru toscrape-index-2
  * should create correct and correctly-formatted toscrape-test-0 thru toscrape-test-2