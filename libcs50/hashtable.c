/* Author: Erich Woo
 * Date: 25 April 2020
 * Purpose: Moduel for manipulating a hashtable
 */

#include <stdlib.h>
#include "jhash.h"
#include "set.h"
#include "hashtable.h"

typedef struct hashtable {
  set_t **hashset;
  int m;
} hashtable_t;

/* Helper method returns the set for specific hash(key) */
set_t *hash_loc(hashtable_t *ht, const char *key) {
   return ht->hashset[(int)JenkinsHash(key, ht->m)];
}

hashtable_t *hashtable_new(const int num_slots) {
  hashtable_t *ht = (hashtable_t *) calloc(1, sizeof(hashtable_t));
  
  if (ht == NULL)
    return NULL;
  ht->hashset = (set_t **) calloc(num_slots, sizeof(set_t*));
  ht->m = num_slots;

  for (int i = 0; i < ht->m; i++)
    ht->hashset[i] = set_new();
  return ht;
}

bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
  if (ht != NULL && key != NULL && item != NULL) {
    if (hashtable_find(ht, key) == NULL) { // key not found
      return set_insert(hash_loc(ht, key), key, item);
    }
  }
  return false;
}

void *hashtable_find(hashtable_t *ht, const char *key) {
  if (ht != NULL && key != NULL) 
    return set_find(hash_loc(ht, key), key);
  return NULL;
}

void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item\
				       )) {
  if (fp != NULL) {
    if (ht == NULL) 
      fputs("(null)\n", fp);
    else {
      for (int i = 0; i < ht->m; i++)
	set_print(ht->hashset[i], fp, itemprint);
    }
  }
}

void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item)) {
  if (ht != NULL && itemfunc != NULL) {
    for (int i = 0; i < ht->m; i++)
      set_iterate(ht->hashset[i], arg, itemfunc);
    // for (set_t *set = ht->hashset->head; set != NULL; set = set->next)
    //set_iterate(set, arg, itemfunc);
  }
}

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
  if (ht != NULL) {
    for (int i = 0; i < ht->m; i++) {
      set_delete(ht->hashset[i], itemdelete);
    }
    free(ht->hashset);
    free(ht);
  }
}
