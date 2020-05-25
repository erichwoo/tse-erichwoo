/* Author: Erich Woo
 * Date: 6 May 2020
 * Purpose: class of functions for manipulating a webpage with a URL.
 * See 'pagedir.h' for documentation.
 */

#include "pagedir.h"

/* Helper method for converting int to string dynamically
 * Counts number of digits in integer
 * Assumes positive integers only
 *
 * @param int the integer to count
 * @return the number of digits
 */
static int numDigits(int x) {
  if (x < 10)
    return 1;
  return 1 + numDigits(x / 10);
}

char* build_dir_id(char* dir, int id) {
  char* str_id = (char*) calloc(numDigits(id) + 1, sizeof(char));
  sprintf(str_id, "%d", id);
  char* file = (char*) calloc(strlen(dir) + strlen("/") +
                              strlen(str_id) + 1, sizeof(char));
  strcpy(file, dir);
  if (strcmp(file + strlen(file) - strlen("/"), "/") != 0)
    strcat(file, "/"); // add '/' if page_dir doesnt already have it
  strcat(file, str_id);

  free(str_id);
  return file;
}

int dir_exists(char* dir, int type) {
  if (dir == NULL)
    return 1;
  char* test = (char*) calloc(strlen(dir) + strlen("/.crawler") + 1, sizeof(char)); // length of char* including '/' to emcompase both cases
  FILE* fp;
  char* check = (char*) calloc(strlen("r") + 1, sizeof(char));
  
  strcpy(test, dir);
  // if dir didn't contain a '/' at end of string, add it
  if (strcmp(test + strlen(test) - strlen("/"), "/") != 0)
    strcat(test, "/");
  strcat(test, ".crawler");

  if (type == 0)
    strcpy(check, "r");
  else if (type == 1)
    strcpy(check, "w");
   
  if (strcmp(check, "") != 0) {
    if ((fp = fopen(test, check)) != NULL) {
      fclose(fp);
      free(test);
      free(check);
      return 0;
    }
  }
  free(test);
  free(check);
  return 1;
}

 int is_valid_page(webpage_t* page, bool html, bool message) {
  if (page == NULL) {
    if (message) 
      fprintf(stderr, "Error: given webpage is NULL");
    return 1;
  }  
  if (!IsInternalURL(webpage_getURL(page))) {
    if (message)
      fprintf(stderr, "Error: given webpage has invalid url; must be valid and internal to cs50 server\n");
    return 2;
  }
  if (webpage_getDepth(page) < 0) {
    if (message)
      fprintf(stderr, "Error: given webpage has invalid depth; must be non-negative\n");
    return 3;
  }
  if (html && webpage_getHTML(page) == NULL) {
    if (message)
      fprintf(stderr, "Error: given webpage requires a non-NULL html\n");
    return 4;
  }
  if (!html && webpage_getHTML(page) != NULL) {
    if (message)
      fprintf(stderr, "Error: given webpage requires a NULL html\n");
    return 5;
  }
  return 0;
}

int pagefetcher(webpage_t* page) {
  if (is_valid_page(page, false, true) != 0)
    return 1;
  if (!webpage_fetch(page)) {
    fprintf(stderr, "Error fetching html for given webpage\n");
    return 1;
  }
  return 0;
}

int pagesaver(FILE* fp, webpage_t* page) {
  if (fp == NULL) {
    fprintf(stderr, "Error: NULL file pointer\n");
    return 1;
  }
  if (is_valid_page(page, true, true) != 0)
    return 1;
  
  fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
  return 0;
}

char* pagescanner(webpage_t* page, int* pos) {
  if (is_valid_page(page, true, true) != 0) {
    return NULL;
  }
  return webpage_getNextURL(page, pos);
}
