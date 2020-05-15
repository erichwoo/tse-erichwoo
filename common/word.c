/* Author: Erich Woo
 * Date: 12 May 2020
 * Purpose: utility module for manipulating a string/word. 
 * See 'word.h' for documentation.
 */

#include "word.h"

bool NormalizeWord(char* word) {
  for (int i = 0; i < strlen(word); i++) {
    word[i] = tolower(word[i]);
  }
  return true;
}
