/* Author: Erich Woo
 * Date: 12 May 2020
 * Purpose: loads a correctly-formatted index from a specified file,
 * builds this index into the inverted-index data structure,
 * and writes this structure back into another specified file.
 */

#include <stdio.h>
#include "pagedir.h" // for dir_exist
#include "index.h"
#include "hashtable.h"
#include "file.h" // for lines_in_file

int main(int argc, char* argv[]) {
  if (argc != 3) { // incorrect number of arguments
    fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
    return 1;
  }
  if (path_valid(argv[1], 0) != 0) { // invalid or nonreadable oldIndexFilename
    fprintf(stderr, "Error: oldIndexFilename must exist and be readable\n");
    return 1;
  }
  if (path_valid(argv[2], 1) != 0) {// invalid or nonwritable newIndexFilename
    fprintf(stderr, "Error: newIndexFilename must be valid and writable\n");
    return 1;
  }

  /********* Initialize *************/
  FILE *fp = fopen(argv[1], "r");
  int m = 4 * lines_in_file(fp) / 3; // for lamda ~ 0.75
  fclose(fp);
  hashtable_t* index = hashtable_new(m);

  index_load(argv[1], index);
  index_save(argv[2], index);

  hashtable_delete(index, NULL);
  return 0;
}

