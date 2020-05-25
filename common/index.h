/* Author: Erich Woo
 * Date: 12 May 2020
 * Purpose: Header file for index.c
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file.h"
#include "pagedir.h"  // for build_dir_id and dir_exists method
#include "hashtable.h"
#include "counters.h"
#include "webpage.h"
#include "word.h"

/* Similar to dir_exists in pagedir.c, but directly
 * tests validity of a filename
 *
 * @param path the pathname of the file
 * @param type checking either readable or writable
 *         0 for checking readability, must exist
 *         1 for checking writability
 * @return 0 if directory exists, 1 otherwise
 */
int path_valid(char* path, int type);

/* Scans each file in directory and
 * builds the inverted-index data structure as a hashtable,
 * mapping words to a counterset of (docID, count) pairs.
 * Words that are nonalphabetic or fewer than 3 characters are ignored.
 * 
 * Assumes: directory is formatted correctly. With valid input,
 * method should always exit cleanly. 
 *
 * @param dir the pageDirectory to build from
 * @param index the hashtable/index data structure
 * @return false if NULL index or invalid/unreadable dir, true otherwise
 */
bool index_build(char* dir, hashtable_t* index);

/* Writes the given inverted-index data structure 
 * to a valid and writeable file. Iterates over the
 * hashtable, writing to file with syntax:
 *    - one line per word
 *    - word docID count [docID count]...
 * Returns false if NULL file/index or invalid
 * or unwriteable file
 *
 * Note: each key's (word's) item (counterset) is
 * deleted/free'd after writtem with index_save. 
 * The hashtable will still need to be deleted/free'd. 
 * 
 * @param file the index file to write into
 * @param index the index to extract data for writing
 * @return false if NULL index or unable to write file, true otherwise
 */
bool index_save(char* file, hashtable_t* index);

/* Loads the inverted-index data from an existing,
 * readable index file (previously created using
 * index_save), and builds back the data structure into
 * a hashtable.
 *
 * Assumes: the content of the given index file
 * is formatted correctly. 
 *
 * @param file the existing indexfile to read from
 * @param index the index to build back
 * @return false if NULL index or unable to read file, true otherwise
 */
bool index_load(char* file, hashtable_t* index);

#endif // __INDEX_H
