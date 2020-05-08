/* Author: Erich Woo
 * Date: 25 April 2020
 * Purpose: Module for manipulating a set
 */

#include <stdlib.h>
#include <string.h>
#include "set.h"

typedef struct setnode {
  char* key;
  void* item;
  struct setnode *next;
} setnode_t;

typedef struct set {
  struct setnode *head;
} set_t;

/* Allocate and initialize a setnode with given char* key and void* item
 * the 'static modifier means this function is not visible outside this file
 */
static setnode_t *setnode_new(const char *key, void *item) {
  setnode_t *node = (setnode_t *) calloc(1, sizeof(setnode_t));

  if (node == NULL || key == NULL || item  == NULL)
    return NULL; // error allocating memory for node or invalid key/item
  node->key = (char *) calloc(strlen(key) + 1, sizeof(char)); 
  strcpy(node->key, key);
  node->item = item;
  node->next = NULL;
  return node;
}

set_t *set_new(void) {
  set_t *set = (set_t*) calloc(1, sizeof(set_t));

  if (set == NULL)
    return NULL;
  set->head = NULL;
  return set;
}

bool set_insert(set_t *set, const char *key, void *item) {
  if (set != NULL && key != NULL && item  != NULL) {
    if (set_find(set, key) == NULL) { // key not found
      setnode_t *new = setnode_new(key, item);
      if (new != NULL) {
	new->next = set->head;
	set->head = new;
	return true;
      }
    }
  }
  return false;
}

void *set_find(set_t *set, const char *key) {
  if (set != NULL && key != NULL) {
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      if (strcmp(node->key, key) == 0) // if key strings match
	return node->item;
    }
  }
  return NULL;
}

void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item)) {
  if (fp != NULL) {
    if (set == NULL)
      fputs("(null)\n", fp);
    else {
      fputc('{', fp);
      if (itemprint != NULL) {
	for (setnode_t *node = set->head; node != NULL; node = node->next) {
	  (*itemprint)(fp, node->key, node->item);
	  if (node->next != NULL)
	    fputs(", ", fp);
	}
      }
      fputs("}\n", fp);
    }
  }
}

void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item)) {
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t *node = set->head; node != NULL; node = node->next)
      (*itemfunc)(arg, node->key, node->item);
  }
}

void set_delete(set_t *set, void (*itemdelete)(void *item)) {
  if (set != NULL) {
    for (setnode_t *node = set->head; node != NULL;) {
      free(node->key); // free the key
      if (itemdelete != NULL) 		    // if possible...
        (*itemdelete)(node->item);	    // delete node's item
      setnode_t *next = node->next;	    // remember what comes next
      free(node);			    // free the node
      node = next;			    // and move on to next
    }
    free(set);
  }
}
