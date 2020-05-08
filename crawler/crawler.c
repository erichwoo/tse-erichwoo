/* Author: Erich Woo
 * Date: 6 May 2020
 * Purpose: crawls pages for urls to a set max depth, saving to a directory
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"

/* Global max_depth threshold max */
const int max_threshold = 10;

/* Helper method for converting int to string dynamically
 * Counts number of digits in integer
 * Assumes positive integers only
 *
 * @param int the integer to count
 * @return the number of digits
 */
int numDigits(int x) {
  if (x < 10)
    return 1;
  return 1 + numDigits(x / 10);
}

int main(int argc, char* argv[]) {
  /**************** INITIALIZE VARIABLES ***************/
  int max_depth, id, ht_size;
  char* seed_URL;
  char* page_dir;
  webpage_t* start;
  webpage_t* curr_page;
  webpage_t* new_page;
  bag_t* bag;
  hashtable_t* ht;

  /***************** INITIAL ERROR CHECKING COMMANDLINE ARGS *******************/
  if (argc != 4) { // incorrect number of arguments
    fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
    return 1;
  }
  if (dir_exists(argv[2]) != 0) { // invalid pageDirectory
    fprintf(stderr, "Error: given pageDirectory must be a valid writable directory\n");
    return 1;
  }
  
  if (sscanf(argv[3], "%d", &max_depth) == 0 || max_depth < 0 || max_depth > max_threshold) {
    fprintf(stderr, "Error: given maxDepth must be non-negative integer\n");
    return 1;
  }

  /***************** RENAMING COMMANDLINE ARGS FOR EASE ****************/
  // start URL must be dynamically allocated for insertion into ht for consistency
  seed_URL = (char*) calloc(strlen(argv[1])+1, sizeof(char));
  strcpy(seed_URL, argv[1]);
  page_dir = argv[2]; // all three arguments now renamed seed_URL, page_dir, max_depth

  start = webpage_new(seed_URL, 0, NULL);
  if (is_valid_page(start, false, true) != 0) {
    webpage_delete(start);
    return 1;
  }

  /***************** INITIALIZE START CONDITIONS ******************/
  // initialize bag and ht
  bag = bag_new();
  ht_size = (int)pow(5, max_depth); // bc no rehashing, aim for final lambda ~ 1
  ht = hashtable_new(ht_size); // estimate ~5 urls per page, so 5^max_depth
  bag_insert(bag, start);
  hashtable_insert(ht, seed_URL, "");

  /**************** BEGIN CRAWLING ********************/
#ifdef TEST
  printf("begin crawling...\n\n");
#endif
  id = 1;
  while ((curr_page = bag_extract(bag)) != NULL) {
    // allocate enough space for storing id in string type
    char* str_id = (char*) calloc(numDigits(id) + 1, sizeof(char));    
    sprintf(str_id, "%d", id); // convert int id to str id for concatenation
    // allocate enough space for storing the full filepath
    char* filename = (char*) calloc(strlen(page_dir) + strlen("/") +
				    strlen(str_id) + 1, sizeof(char));
    FILE* fp;
    
    strcpy(filename, page_dir);
    if (strcmp(filename + strlen(filename) - strlen("/"), "/") != 0) 
      strcat(filename, "/"); // add '/' if page_dir doesnt already have it
    strcat(filename, str_id);
    
    if ((fp = fopen(filename, "w")) != NULL) {
      if (pagefetcher(curr_page) != 0 || pagesaver(fp, curr_page) != 0) {
	fclose(fp);
	unlink(filename); // remove filepath if any error writing to file
      }
      else {
#ifdef TEST
	printf("%d. ADDED %s to %s...\n", id, webpage_getURL(curr_page), filename);
#endif
	fclose(fp);
	id++;
	// if not at max_depth yet, scan this page for embedded URLs 
	if (webpage_getDepth(curr_page) < max_depth) {
	  int pos = 0;
	  char* extracted_url;
#ifdef TEST
	  printf("---------------------Looking Deeper---------------------\n");
#endif
	  while ((extracted_url = pagescanner(curr_page, &pos)) != NULL) {
#ifdef TEST
	    printf("\tExtracted URL: %s\n", extracted_url);
#endif
	    if (IsInternalURL(extracted_url) && hashtable_insert(ht, extracted_url, "")) {
#ifdef TEST
	      printf("\tURL inserted into hashtable...\n");
#endif
	      // create and insert newpage to bag; will be freed in bag_delete
	      new_page = webpage_new(extracted_url, webpage_getDepth(curr_page) + 1, NULL);
	      if (new_page != NULL) {
		bag_insert(bag, new_page);
#ifdef TEST
		printf("\tWebpage inserted into bag for exploration...\n\n");
#endif
	      }
	      else {
		webpage_delete(new_page); // manually free new_page if unused
		new_page = NULL;
		fprintf(stderr, "Error creating new webpage for url\n\n");
	      }
	    }
	    else {
#ifdef TEST
	      printf("\tURL is duplicate and/or not internal; NOT inserted into hashtable and bag\n\n");
#endif
	      free(extracted_url); // if url extracted but not added to bag
	      extracted_url = NULL; // must manually free it
	    }
	  }
	}
#ifdef TEST
	else
	  printf("-----------------Max Depth Reached-----------------\n\n");
#endif
      }
    }
    else { // error opening fp; in this case, id NOT incremented
      fprintf(stderr, "Error opening filepath for writing\n");
    }
    free(filename);
    free(str_id);
    webpage_delete(curr_page); // free page after done using it
    curr_page = NULL; // must manually because still allocated although extracted
  }
  printf("bag is now empty. crawling is complete.\n");
  
  /****************** DELETION ********************/
  bag_delete(bag, webpage_delete);
  hashtable_delete(ht, NULL);
  return 0;
}
