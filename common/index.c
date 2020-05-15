/* Author: Erich Woo
 * Date: 12 May 2020
 * Purpose: utility module for building, loading,
 * and saving an inverted indexer for tse usage.
 *
 * See 'index.h' for method documentation
 */

#include "index.h"

int path_valid(char* path, int type) {
  FILE* fp;
  char* check = (char*) calloc(strlen("r") + 1, sizeof(char));
  if (type == 0)
    strcpy(check, "r");
  else if (type == 1)
    strcpy(check, "w");
  
  if (strcmp(check, "") != 0 && path != NULL) {
    if ((fp = fopen(path, check)) != NULL) {
      fclose(fp);
      free(check);
      return 0;
    }
  }
  free(check);
  return 1;
}

bool index_build(char* dir, hashtable_t* index) {
  if (dir_exists(dir, 0) == 1 || index == NULL)
    return false;
  //loop thorugh directory
  int doc_id = 1;
  bool search_dir = true; // while fopen != null, keep incrementing for files
  while (search_dir) {
    char* str_id = (char*) calloc(numDigits(doc_id) + 1, sizeof(char));
    sprintf(str_id, "%d", doc_id); 
    char* file = (char*) calloc(strlen(dir) + strlen("/") +
                                    strlen(str_id) + 1, sizeof(char));
    strcpy(file, dir);
    if (strcmp(file + strlen(file) - strlen("/"), "/") != 0)
      strcat(file, "/"); // add '/' if page_dir doesnt already have it                   
    strcat(file, str_id);

    FILE* fp;
    if ((fp = fopen(file, "r")) != NULL) {
      char* url = freadlinep(fp);       // 1st line
      char* str_depth = freadlinep(fp); // 2nd line
      int depth = atoi(str_depth);      //  - create str to free later
      char* html = freadfilep(fp);      // Remaining lines
      webpage_t* page = webpage_new(url, depth, html);
      
      int pos = 0;
      char* next;
      while ((next = webpage_getNextWord(page, &pos)) != NULL) {
	if (strlen(next) >= 3) {
	  NormalizeWord(next);
	  counters_t* counter;
	  // get the counters_t for the given word
	  // if a new word, create new word-counter pair and add to index
	  // then increment the docID's count (or create if node for ID not created)
	  if ((counter = hashtable_find(index, next)) == NULL) {
	    counter = counters_new();
	    hashtable_insert(index, next, counter);
	  }
	  counters_add(counter, doc_id);
	}
	free(next);
	next = NULL;
      }
      fclose(fp);
      free(str_depth);
      webpage_delete(page);
      doc_id++;
    }
    else { // if fopen is null, no more files left to read/increment
      search_dir = false;
    }
    free(str_id);
    free(file);
  }
  return true;
}

/* Internal method for writing an individual docID-count pair
 * to a file pointer. 
 *
 * Assumes non-NULL params and FILE* type arg
 * Because internal, no need to error check assumptions
 *
 * @param arg the filepointer to write to
 * @param key the docID
 * @param count the word count for the docID
 */
static void write_idcount_pair(void* arg, const int key, const int count) {
  FILE* fp = arg;
  fprintf(fp, " %d %d", key, count);
}

/* Internal method for writing an individual word-counterset pair
 * to a file pointer. Calls a counterset iterator on the counterset item
 * for writing each docID-count pair of the counterset.
 *
 * Note: method deletes/frees the counter after writing its contents
 *
 * Assumes non-NULL params, FILE* type arg, and counters_t* type item
 * Because internal, no need to error check assumptions
 *
 * @param arg the filepointer to write to
 * @param key the word
 * @param item the counterset containing all docID-count pairs
 */
static void write_wordcounter_pair(void* arg, const char* key, void* item) {
  FILE* fp = arg;
  fprintf(fp, "%s", key);
  counters_iterate(item, fp, write_idcount_pair);
  fprintf(fp, "\n");
  counters_delete(item); // free counter after done saving it
}

bool index_save(char* file, hashtable_t* index) {
  FILE* fp;
  if (index != NULL && (fp = fopen(file, "w")) != NULL) {
    hashtable_iterate(index, fp, write_wordcounter_pair);
    fclose(fp);
    return true;
  }
  return false;
}

bool index_load(char* file, hashtable_t* index) {
  FILE* fp;
  if (index != NULL && (fp = fopen(file, "r")) != NULL) {
    int num_lines = lines_in_file(fp);
    for (int i = 0; i < num_lines; i++) { // for each line in indexfile
      // syntax: word docID count [docID count]...
      int ret, doc_id, count;
      char* word = freadwordp(fp);
      counters_t* counter = counters_new();
      hashtable_insert(index, word, counter);
      while ((ret = fscanf(fp, "%d %d ", &doc_id, &count)) == 2)
	counters_set(counter, doc_id, count);
      free(word);
      word = NULL;
    }
    fclose(fp);
    return true;
  }
  return false;
}
