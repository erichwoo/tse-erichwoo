# CS50 Lab 6
## TSE Querier Design Spec
## Erich Woo - 23 May 2020

Some Design Spec contents are directly taken from https://www.cs.dartmouth.edu/~cs50/Lectures/querier/

### Querier

The `querier` reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user.

### User Interface

The querier has two interfaces, both with the user.

The first is on the command-line; it must always have two arguments:

```bash
$ querier pageDirectory indexFilename
```

In order to proceed with the program, there must be two arguments, `pageDirectory` must be a readable crawler-created directory, and `indexFilename` must be a valid readable pathname.

The second interface is with the querier, which continually prompts the user for queries, handling robustly for invalid input or alternate forms of input. To exit the querying, EOF must be reached.

### Inputs and Outputs

Input: command-line inputs are self-explanatory as above. For correct query input, query must NOT contain non-alphabetic characters. Use keywords 'and' and 'or' to form intersections and unions of words, respectively. If two words in the query are adjacent, an implicit 'and' intersection is read. Keywords 'and' and 'or' must NOT be the first or last words in the query, and must NOT be adjacent with each other.

Output: the `querier` writes to standard output as follows:

* the clean query (all lowercase and trimmed of whitespace) or an error message if invalid input
* the number of documents matching the query
* for each matching document, its score, docID, and URL
  * printed in descending order of score

For scores that are equal, the documents may be in any order.

### Functional Decomposition into Modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modues
 2. *querier*, which continuously queries the user and prints ranked documents
 3. *clean_query*, which parses and error-checks the query
 4. *satisfy_query*, which satisfies the intersections and unions of the query
 5. *intersect*, which performs an intersection of two sets
 6. *merge*, which performs a union of two sets
 7. *sort_scores*, which sorts the document scores in descending order
 8. *print_scores*, which prints the scores to stdout

And some helper modules that provide data structures:

 1. *hashtable* of each word-counterset, representing the inverted index
 2. *counterset* of docID-score pairs for 'and/or' sequences

### Pseudo-code for Logic/Algorithmic Flow

The `querier` will run as follows:

 1. execute from command-line as shown in the User Interface
 2. parse the command-line, validate parameters
 3. initialize a hashtable representing the inverted-index
 4. continuously query the user until EOF:
    1. use *clean_query* to parse/clean and validate the query
    2. if valid query,
       1. use *satisfy_query* to satisfy the query of intersections and unions, grabbing word-countersets from the hashtable
       	  1. use *intersection* and *merge* to step through the query with the *Backus-Naur Form* technique
       2. use *sort_scores* to rank the resulting counterset of doc-score in descending order
       	  1. iterate over counterset to get number of documents
	  2. iterate again over counterset to place into doc-score array via *insertion sort* algorithm
       3. use *print_scores* to print the sorted scores to stdout
       	  1. loop over doc-score array and print each one
 5. delete/free hashtable and any other malloc'd memory

### Dataflow through Modules

 1. *main* parses parameters and passes them to *querier*
 2. *querier* continuously queries the user,
    1. passing it to be cleaned by *clean_query*
    2. passing cleaned queries to be satisfied by *satisfy_query*
    3. passing those results to be sorted by *sort_scores*
    4. passing those sorted scores to be printed by *print_scores*
 3. *satisfy_query* walks the query left to right, choosing between *intersection* and *merge* depending on Backus-Naur Form technique

### Major Data Structures

Within `querier`, three helper data structures are created:

 1. *both*, containing both countersets, to be passed to an iterator for *intersect* usage
 2. *doc_score* of docID and score, of which to be used as an array
 3. *data*, containing an array of *doc_score* (more precisely double pointer), and an integer index for tracking; to be used to sort documents by score

Important: the Implementation Spec will specify how these data structures are used in more detail. *both* is important for passing more than one 'thing' into the 1st iterator in *sort_scores*. *data* is mostly just a representation of *doc_score*, but is important also for passing more than one 'thing' into the 2nd iterator in *sort_scores*. 

Three helper modules provide data structures:

 1. *hashtable* of words maping to counterset (docID, count) structures
 2. *set* of words (indirectly used by hashtable)
 3. *counters* of docIDs (docID, count/score)

### Testing Plan

* error check command-line arguments
* error check query input
* test empty query input
* test queries with no match
* test variable input styles (capitalization, implicit 'and's, extra whitespace)
* regression testing with an example from REQUIREMENTS.md
* fuzztesting with an example from 'TSE Querier' Lecture Notes
* fuzztesting with wikipedia-index-1