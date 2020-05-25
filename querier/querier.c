/* Author: Erich Woo
 * Date: 21 May 2020
 * Purpose:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "pagedir.h" // for build_dir_id & dir_exist
#include "index.h" // for path_valid
#include "hashtable.h"
#include "counters.h"
#include "file.h"
#include "word.h"

/***************** local types ****************/

typedef struct both {    // used to help intersections
  counters_t* a;         // pointer to 1st counterset
  counters_t* b;         // pointer to 2nd counterset
} both_t;

typedef struct doc_score {
  int id;                // document id
  int score;             // document score
} doc_score_t;

typedef struct data {
  doc_score_t** scores;  // pointer to pointer for doc_score_t pair
  int index;             // index tracking which pointer to place score into
} data_t;                //    and at the end, acts as total size

/******************** internal functions *********************/

/* Validates args for main of the querier
 * as specified by the Requirements spec
 *
 * Leverages validation functions from 'pagedir' and 'index'
 * 
 * @param argc number of args
 * @param argv the array of args
 * @return 0 if successful validation, 1 otherwise
 */
static int validate(int argc, char* argv[]) {
  if (argc != 3) { // incorrect number of arguments
    fprintf(stderr, "Usage: ./querier pageDirectory indexFilename\n");
    return 1;
  }
  if (dir_exists(argv[1], 0) != 0) { // invalid or nonreadable .crawler directory
    fprintf(stderr, "Error: pageDirectory must be a existing readable crawler-created directory.\n");
    return 1;
  }
  if (path_valid(argv[2], 0) != 0) { // invalid or nonreadable pathname
    fprintf(stderr, "Error: indexFilename must be a readable pathname.\n");
    return 1;
  }
  return 0;
}

/* Prompts the user to enter a query
 * only if stdin is atty/keyboard
 * 
 * Taken from https://www.cs.dartmouth.edu/~cs50/Labs/Lab6/#turning-off-the-prompt
 *
 * @return always 1, for conciseness in 'query'
 *         method.
 */
static int prompt(void) {
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
  return 1;
}

/* Cleans and parses a single query line
 *
 * Trims out whitespace. Error checks invalid queries 
 * according to the Implementation spec.
 * Prints an error or the clean query at the end.
 *
 * Returns an integer representing the number of words found, and thus
 * the first n-1 words in words array that must be later free'd by the caller.
 * If the integer is negative, an error was found, but first -1 * (n-1) words
 * still must be later free'd by the caller. If an integer is 0,
 * either no words in query or an error at beginning of parsing.
 *
 * Assumes: words[] array allocates enough space for the maximum
 * possible number of words depending on line character length
 *
 * @param line the query to clean and parse
 * @param words the words array to place individual parsed-words
 * @return a signed integer representing the number of words allocated 
 *         and thus requiring later freeing. 
 */
static int clean_query(char* line, char* words[]) {
  NormalizeWord(line);
  // iniatialize a temp pointer which tracks
  // the start of a new word
  char* temp = line;
  // loop through line, assigning words along the way
  int i = 0;
  for (char* cp = line; *cp != '\0'; cp++) {
    // hitting whitespace
    if (*cp == ' ') { 
      *cp = '\0';
      // check potential errors if seeing keyword 'and' or 'or'
      if (strcmp(temp, "and") == 0 || strcmp(temp, "or") == 0) {
	// is first
	if (i == 0) {
	  fprintf(stderr, "Error: '%s' cannot be first.\n", temp);
	  return i;
	}
	// is adjacent
	else if (strcmp(words[i - 1], "and") == 0 || strcmp(words[i - 1], "or") == 0) {
	  fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent.\n", words[i - 1], temp);
	  return -1 * i;
	}
      }
      // allocate and assign next available space in words[]
      // with string from temp* to '\0' (created in line 72)
      // if-statement prevents leading whitespace in 'line'
      if (*temp != '\0') {
	words[i] = (char*) calloc(strlen(temp) + 1, sizeof(char));
	strcpy(words[i++], temp);  // also increment index
      }
      // skip past consecutive whitespaces in forloop
      // then set temp* to start of next word
      while (*(cp + 1) == ' ')
	cp++;
      temp = cp + 1; 
    }
    // error check if char is non-alphabetic
    else if (isalpha(*cp) == 0) { 
      fprintf(stderr, "Error: bad character '%c' in query.\n", *cp);
      return -1 * i;
    }
    // does nothing if alphabetic, moves on
  }
  // after loop, if there is temp* is still pointing to word
  // finish allocation and assignment. Won't happen if
  // 'line' had trailing whitespace
  if (*temp != '\0') { 
    words[i] = (char*) calloc(strlen(temp) + 1, sizeof(char));
    strcpy(words[i++], temp);
  }
  // error check if last word assigned was keyword 'and' or 'or'
  if (strcmp(words[i - 1], "and") == 0 || strcmp(words[i - 1], "or") == 0) {
    char* fl = "last";
    if (i == 1) // if single word query of 'and'/'or'
      fl = "first";
    fprintf(stderr, "Error: '%s' cannot be %s.\n", words[i - 1], fl);
    return -1 * i;
  }
  // print the clean query if success
  printf("Query: ");
  for (int j = 0; j < i; j++)
    printf("%s ", words[j]);
  printf("\n");
  
  return i;
}

/* Merges an individual doc-count pair from the 2nd counterset
 * into the 1st counterset. Used by counterset iterator in 'merge',
 * iterating over the 2nd counterset.
 *
 * Sums the the counts of specified key/document from both countersets
 * and puts the value into the 1st counterset.
 * If specified key doesn't exist in 1st, but does exist as count=0 in 2nd,
 * key's counter is NOT added to 1st, thus weeded out. This is good
 * for cleaning the final output of the 1st counterset.
 *
 * The counters_t* arg is edited for the caller,
 * and must be later free'd by caller.
 *
 * @param arg the dereferenced 1st counterset to edit
 * @param key the key of the pair in 2nd counterset
 * @param count the count of the pair in 2nd counterset
 */
static void merge_helper(void* arg, const int key, const int count) {
  counters_t* a = arg;
  int a_count = counters_get(a, key); // 0 if not found
  if (count + a_count != 0)   // weeds out 0'd counters from non-intersections
    counters_set(a, key, count + a_count); // implicitly inserts if not exist
}

/* Iterates over the 2nd counterset and merges
 * counts of the each doc with those of the 1st counterset
 * to create a union'd score, placed in the 1st counterset
 *
 * The 1st counterset is edited for the caller.
 * Both countersets must be later free'd by the caller.
 *
 * @param a the 1st counterset
 * @param b the 2nd counterset
 */
static void merge(counters_t* a, counters_t* b) {
  /* UNCOMMENT FOR VERBOSE MERGE CHECKING
  printf("first set: ");
  counters_print(a,stdout);
  printf("second set: ");
  counters_print(b,stdout);
  */
  counters_iterate(b, a, merge_helper);
  /*
  printf("resulting set: ");
  counters_print(a, stdout);
  printf("\n");
  */
}

/* Intersects an individual doc-count pair from the 1st counterset
 * with that of the 2nd counterset and places into the 1st counterset.
 * Called by counterset iterator in 'intersect'
 *
 * Finds matches between specified key/document and the 2nd counterset.
 * If a match is found, the new score is the minimum count 
 * found between the two countersets, and is set in 1st counterset.
 * If a match is NOT found, the 1st counterset's document is set to count 0.
 *
 * A struct containing both countersets is passed into this method for use.
 * Only the 1st one is edited. The two countersets within the struct,
 * as well as the struct itself must later be free'd by the caller.
 *
 * @param arg the dereferenced 1st counterset to edit
 * @param key the key of the pair in 2nd counterset
 * @param count the count of the pair in 2nd counterset
 */
static void intersect_helper(void* arg, const int key, const int count) {
  // set a-score as the minimum between two
  // okay if both are 0, will set a-score to 0
  // and 'merge' will later purge these 0 scores
  both_t* both = arg;
  int b_score = counters_get(both->b, key);
  int a_score = (b_score == 0) ? 0 : ((count < b_score) ? count : b_score);
  counters_set(both->a, key, a_score);
}

/* Iterates over the 1st counterset and intersects
 * the each doc's count with that of a matching doc in the 2nd counterset,
 * with new scores placed into the 1st counterset. 
 * Makes use of a struct 'both' containing 
 * both countersets; struct is passed into coutnerset
 * iterator for manipulation of both countersets
 *
 * The 1st counterset is edited for the caller.
 * Both countersets must be later free'd by the caller.
 * the struct is free'd by this function.
 *
 * @param a the 1st counterset
 * @param b the 2nd counterset
 */
static void intersect(counters_t* a, counters_t* b) {
  /* UNCOMMENT FOR VERBOSE INTERSECT CHECKING
  printf("first set: ");
  counters_print(a,stdout);
  printf("second set: ");
  counters_print(b,stdout);
  */
  both_t* both = (both_t*) calloc(1, sizeof(both_t));
  both->a = a;
  both->b = b;
  counters_iterate(a, both, intersect_helper);
  free(both);
  /*
  printf("resulting set: ");
  counters_print(a, stdout);
  printf("\n");
  */
}

/* Takes the clean query and returns a counterset 
 * of documents (and their scores) that satisfy
 * said query. Uncomment printf lines for verbose
 * bugchecking. 
 *
 * the index, each word in words[], and the returned 
 * 'result' counterset must all be later free'd by caller.
 *
 * @param index the index to match the query with
 * @param words the clean query
 * @param num_words the # of words in the array
 */
static counters_t* satisfy_query(hashtable_t* index, char* words[], int num_words) {
  counters_t* result = counters_new(); // running total (all the 'or's)
  counters_t* and_seq; // running product (the 'and's)
  int and_init = 1; // 1 signifies to start a new andsequence
  for (int i = 0; i < num_words; i++) {
    if (and_init) {
      //printf("starting merge with new 'andsequence' and '%s'\n", words[i]);
      and_seq = counters_new();
      merge(and_seq, hashtable_find(index, words[i]));
      and_init = 0;
    }
    else if (strcmp(words[i], "or") == 0) {
      //printf("'or' spotted. Merging 'result' with 'andsequence'\n");
      merge(result, and_seq);
      counters_delete(and_seq);
      and_init = 1;
    }
    else if (strcmp(words[i], "and") != 0) {
      //printf("intersecting 'andsequence' with '%s'\n", words[i]);
      intersect(and_seq, hashtable_find(index, words[i]));
    }
    //else { printf("'and' spotted. Moving on.\n\n"); }
  }
  //printf("end of query. Merging 'result' with final 'andsequence'\n");
  merge(result, and_seq);
  counters_delete(and_seq);
  return result;
}

/* Increments the count (arg) every time this function 
 * is called. Used by counters iterator to count 
 * the number of counters in the counterset.
 * key and count are unused. 
 * The arg/count is edited for caller's use.
 *
 * @param arg the counters count
 * @param key unused
 * @param count unused
 */
static void countercount(void* arg, const int key, const int count) {
  int* num_counters = arg;
  (*num_counters)++;
}

/* Takes an individual doc-score pair and sorts it 
 * into an array of doc-score pairs in descending order
 * based on score. Called by counterset iterator.
 * Utilizes insertion sort algorithm. Uncomment printf for bugchecking.
 *
 * Makes use of a doc_score_t struct and a data_t struct.
 * doc_score_t struct contains a single document id and its score.
 * data_t struct contains an array of doc-score_t pointers, and an
 * index indicating the next index that must be calloc'd and filled
 * in the array of doc-score's.
 *
 * The data struct is edited for caller's use.
 * The arg/data struct and all its internal structs (doc_score_t**
 * and each doc_score_t* within it) must be later free'd by caller.
 *
 * @param arg the data struct
 * @param key the doc_id of the current counter
 * @param count the score of the current counter
 */
static void score_sort(void* arg, const int key, const int count) {
  data_t* data = arg;
  doc_score_t** scores = data->scores;
  int curr = data->index;
  // insertion sorting
  scores[curr] = (doc_score_t*) calloc(1, sizeof(doc_score_t));
  while (curr - 1 >= 0 && count > scores[curr - 1]->score) {
    scores[curr]->id = scores[curr - 1]->id;
    scores[curr]->score = scores[curr - 1]->score;
    curr--;
  }
  //printf("index %d with doc_id %d and score %d placed at %d\n\n", data->index, key, count, curr);
  scores[curr]->id = key;
  scores[curr]->score = count;
  data->index++;
}

/* Sorts a counterset of doc-score pairs in descending order
 * into an array of doc-score's. Iterates twice over the 
 * counterset, first to get the number of counters for
 * creating an appropriate-sized array of doc-score's,
 * second to do the sorting. Insertion sort algorithim is
 * used. Utilizes data_t struct and doc_score_t struct as described 
 * in 'score_sort'.
 *
 * Both raw_scores and the returned sorted data_t* 
 * (along with the doc_score_t** and each doc_score_t* within it) 
 * must be later free'd by the caller.
 *
 * @param raw_scores the unsorted counterset of doc-score pairs
 * @return the pointer to the sorted data_t struct
 */
static data_t* sort_scores(counters_t* raw_scores) {
  // first iterator to get number of documents for array
  int num_docs = 0;
  counters_iterate(raw_scores, &num_docs, countercount);
  // initialize and assign (with iterator) doc-score array
  data_t* data = (data_t*) calloc(1, sizeof(data_t));
  data->scores = (doc_score_t**) calloc(num_docs, sizeof(doc_score_t*));
  data->index = 0;
  counters_iterate(raw_scores, data, score_sort);
  return data;
}

/* Prints the scores of the ranked results
 *
 * data_t struct is unchanged and must 
 * be later free'd by caller.
 *
 * Assumes page_dir is valid readable crawler dir
 *
 * @param data the sorted data of documents
 * @param dir the page_dir to get url from
 */
static void print_scores(data_t* data, char* dir) {
  if (data->index == 0)
    printf("No documents match\n");
  else
    printf("Matches %d documents (ranked):\n", data->index);
  for (int i = 0; i < data->index; i++) {
    doc_score_t* temp = data->scores[i];
    char* file = build_dir_id(dir, temp->id);
    FILE* fp = fopen(file, "r");
    char* url = freadlinep(fp);
    fclose(fp);
    
    printf("score %3d doc %3d: %s\n", temp->score, temp->id, url);
    free(file);
    free(url);
  }
  printf("-----------------------------------------------\n");
}

/* Helper function for free'ing a data_t struct
 * and all its internally calloc'd data
 *
 * @param data the data_t struct to free
 */
static void data_delete(data_t* data) {
  for (int i = 0; i < data->index; i++) 
    free(data->scores[i]);
  free(data->scores);
  free(data);
}

/* Parses queries from stdin and generates
 * the ranked results of the query using
 * the given index data structure
 * 
 * Everything but the index data structure 
 * is free'd. caller must later free index.
 *
 * @param index the inverted-index data structure
 */
static void query(char* dir, hashtable_t* index) {
  char* line;
  // read lines from stdin
  while (prompt() && (line = readlinep()) != NULL) { 
    char* words[(strlen(line) + 1) / 2]; // if 'w w\0', need 4/2 not 3/2
    int num_words;
    if ((num_words = clean_query(line, words)) > 0) {
      counters_t* raw_scores = satisfy_query(index, words, num_words);
      data_t* sorted_data = sort_scores(raw_scores);
      print_scores(sorted_data, dir);
      
      counters_delete(raw_scores);
      data_delete(sorted_data);
    }
    else
      num_words *= -1; // for freeing words

    for (int i = 0; i < num_words; i++) 
      free(words[i]);
    free(line);
  }
  printf("\n");
}

/* Initializes and returns the inverted-index
 * data structure from a given indexfile
 * Load facter (lambda) ~0.75
 *
 * Returned hashtable must be later free'd by caller.
 *
 * @param indexfile the index file
 * @return a hashtable of the index, NULL if error loading index
 */
hashtable_t* index_init(char* indexfile) {
  FILE* fp = fopen(indexfile, "r");
  int m = 4 * lines_in_file(fp) / 3; // for lambda ~0.75
  fclose(fp);
  hashtable_t* index = hashtable_new(m);
  if (!index_load(indexfile, index)) {
    fprintf(stderr, "Error: failed to load index from file");
    return NULL;
  }
  return index;
}

/******************* main **********************/
int main(int argc, char* argv[]) {
  /******** Validate Params *********/
  if (validate(argc, argv) != 0)
    return 1;
  
  /********** Initialize Index ************/
  hashtable_t* index;
  if ((index = index_init(argv[2])) == NULL) 
    return 1;
 
  /******** Begin Querying ************/
  query(argv[1], index);

  /******** Delete/Free Index ***********/
  index_save("/dev/null", index); // delete countersets but suppress the saving
  hashtable_delete(index, NULL);

  return 0;
}
