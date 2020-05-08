/* Author: Erich Woo
 * Date: 6 May 2020
 * Purpose: class of functions for manipulating a webpage with a URL
 */

#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"


/*
 * Helper function to check if given directory exists
 * @param dir the given directory
 * @return 0 if directory exists, 1 otherwise 
 */
int dir_exists(char* dir) {
  char* test = (char*) calloc(strlen(dir) + strlen("/.crawler") + 1, sizeof(char)); // length of char* including '/' to emcompase both cases
  FILE* fp;
  
  strcpy(test, dir);
  // if dir didn't contain a '/' at end of string, add it
  if (strcmp(test + strlen(test) - strlen("/"), "/") != 0)
    strcat(test, "/");
  strcat(test, ".crawler");
  
  if ((fp = fopen(test, "w")) != NULL) {
    fclose(fp);
    free(test);
    return 0;
  }
  free(test);
  return 1;
}

/*
 * Helper method for consilidating the error-checking for a given webpage
 *
 * Error cases:
 * 1. page is NULL
 * 2. url isn't valid and/or internal
 * 3. depth is negative
 * 4. asked for non-NULL html but given NULL html
 * 5. asked for NULL html but given nonNULL html
 *
 * @param page the given webpage
 * @param html boolean whether a page with (true) or without (false) html is desired
 * @param message whether or not user wants error messages to print to stderr
 * @return 0 on no errors found, non-zero on error
 */
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

/* 
 * Fetches the html for webpage and adds it to the struct
 * Assumes page allocated by caller
 *
 * @param page the webpage with url, depth, NULL html
 * @return 0 on successful fetch, 1 otherwise
 */
int pagefetcher(webpage_t* page) {
  if (is_valid_page(page, false, true) != 0)
    return 1;
  if (!webpage_fetch(page)) {
    fprintf(stderr, "Error fetching html for given webpage\n");
    return 1;
  }
  return 0;
}

/*
 * Writes a page's html to a file
 * Assumes fp is error-checked outside of call for writability
 *
 * @param fp the file to write into
 * @param page the page to write
 * @return 0 for success, 1 for error
 */
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

/*
 * Scans a page for embedded URLs, adding them to unseen ht and seen bag
 * @param bag the bag of unexplored webpages
 * @param ht the hashtable of seen URLs
 * @return 0 for success, 1 for error
 */
/*int pagescanner(bag_t* bag, hashtable_t* ht, webpage_t* page) {
  if (bag == NULL) {
    fprintf(stderr, "Error: NULL bag");
    return 1;
  }
  if (ht == NULL) {
    fprintf(stderr, "Error: NULL hashtable");
    return 1;
  }
  if (page == NULL) {
    fprintf(stderr, "Error: NULL page");
    return 1;
  }
  if (webpage_getHTML(page) == NULL) {
    fprintf(stderr, "Error: non-NULL html required");
    return 1;
  }

  int pos = 0;
  char *url;
  while ((url = webpage_getNextURL(page, &pos)) != NULL) {
    if (IsInternalURL(url) &&
	hashtable_insert(ht, url, "")) {
      webpage_t* new_page; // will be free'd when bag is deleted
      if ((new_page = pagefetcher(url, webpage_getDepth(page) + 1))
	  != NULL)
	bag_insert(bag, new_page);
      else {
	fprintf(stderr, "Error creating new webpage for embedded URL");
	return 1;
      }
    }
    free(url);
  }
  return 0;
}
*/

/*
 * Scans a page for the next URL, updating pos
 * @param page the webpage to scan
 * @param pos the position to start scanning
 * @return the next extracted URL, NULL otherwise
 */
char* pagescanner(webpage_t* page, int* pos) {
  if (is_valid_page(page, true, true) != 0) {
    return NULL;
  }
  return webpage_getNextURL(page, pos);
}
