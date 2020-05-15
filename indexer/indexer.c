/* Author: Erich Woo
 * Date: 12 May 2020
 * Purpose: builds an inverted-index data structure from 
 * a set of correctly-formatted documents in a specified directory, 
 * and writes this index to a specified	file.
 */

#include <stdio.h>
#include "pagedir.h" // for dir_exist
#include "index.h"
#include "hashtable.h"

int main(int argc, char* argv[]) {
  if (argc != 3) { // incorrect number of arguments
    fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
    return 1;
  }
  if (dir_exists(argv[1], 0) != 0) { // invalid or nonreadable directory
    fprintf(stderr, "Error: given pageDirectory must be valid and readable directory\n");
    return 1;
  }
  if (path_valid(argv[2], 1) != 0) {// invalid or nonwritable pathname 
    fprintf(stderr, "Error: given indexFilename must be valid and writable pathname\n");
    return 1;
  }

  /********* Initialize *************/
  hashtable_t* index = hashtable_new(500);

  index_build(argv[1], index);
  index_save(argv[2], index);

  hashtable_delete(index, NULL);
  return 0;
}
