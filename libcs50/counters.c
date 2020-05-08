/* Author: Erich Woo
 * Date: 25 April 2020
 * Purpose: Module for manipulating a "counter set" - a set of counters, each distinguised by an integer key
 */

#include <stdlib.h>
#include "counters.h"

typedef struct counternode {
  int key;
  int count;
  struct counternode *next;
} counternode_t;

typedef struct counters {
  struct counternode *head;
} counters_t;

/* Allocate and initialize a counternode with given integer key and count 0
 * the 'static modifier means this function is not visible outside this file
 */
static counternode_t *counternode_new(const int key) {
  counternode_t *node = (counternode_t *) calloc(1, sizeof(counternode_t));

  if (node == NULL || key < 0 )
    return NULL; // error allocating memory for node or invalid key
  node->key = key;
  node->next = NULL;
  return node;
}

/* Helper method returns the corresponding counternode give an integer key
 * if ctrs = NULL or negative key or key not found, return NULL
 * Whenever THIS method is called, I already check ctrs != NULL & key >=0 
 * in its parent method, so I can use THIS method's NULL return as a
 * pseudo for solely that the key isn't found
 * the 'static' modifier means this function is not visible outside this file
 */
static counternode_t *counters_get_node(counters_t *ctrs, const int key) {
  if (ctrs != NULL && key >= 0 ) {
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      if (node->key == key)
	return node; // return the node if found
    }
  }
  // note: also NULL if key not found
  return NULL;
}

	
counters_t *counters_new(void) {
  counters_t *counters = (counters_t *) calloc(1, sizeof(counters_t));

  if (counters == NULL)
    return NULL;  // error allocating counter
  counters->head = NULL;
  return counters; 
}

int counters_add(counters_t *ctrs, const int key) {
  if (ctrs != NULL && key >= 0) { 
    counternode_t *node = counters_get_node(ctrs, key);
    if (node == NULL) { // key doesn't exist yet
      // allocate a new node to be added to the list
      counternode_t *new = counternode_new(key);
      if (new != NULL) {
	// add it to the head of the list
	new->next = ctrs->head;
	ctrs->head = new;
	new->count++;
	return new->count;
      }
    } else {
      node->count++;
      return node->count;
    }
  }
  return 0; // also occurs if allocation of new node fails
}

int counters_get(counters_t *ctrs, const int key) {
  if (ctrs != NULL && key >= 0) {
    counternode_t *node = counters_get_node(ctrs, key);
    if (node != NULL) // if key is found
      return node->count;
  }
  return 0;
}

bool counters_set(counters_t *ctrs, const int key, const int count) {
  if (ctrs != NULL && key >= 0 && count >= 0) {
    counternode_t *node = counters_get_node(ctrs,key);
    if (node == NULL) { // key doesnt exist yet
      counternode_t *new = counternode_new(key);
      if (new != NULL) {
        // add it to the head of the list                                          
        new->next = ctrs->head;
        ctrs->head = new;
	return true;
      }
    } else {
      node->count = count;
      return true;
    }
  }
  return false;
}

void counters_print(counters_t *ctrs, FILE *fp) {
  if (fp != NULL) {
    if (ctrs == NULL)
      fputs("(null)\n", fp);
    else {
      fputc('{', fp);
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
	fprintf(fp, "%d=%d", node->key, node->count);
	if (node->next != NULL) 
	  fputs(", ", fp);
      }
      fputs("}\n", fp);
    }
  }
}

void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg,
                                       const int key, const int count)) {
  if (ctrs != NULL && itemfunc != NULL) {
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) 
      (*itemfunc)(arg, node->key, node->count);
  }
}

void counters_delete(counters_t *ctrs) {
  if (ctrs != NULL) {
    for (counternode_t *node = ctrs->head; node != NULL;) {
      counternode_t *next = node->next;
      free(node);
      node = next;
    }
    free(ctrs);
  }
}
