/* Author: Erich Woo
 * Date: 12 May 2020
 * Purpose: header file for word.c
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* Converts all letters in a word to lowercase
 *
 * Assumes word is malloc'd pointer
 *
 * @param word the word to convert
 * @return true on succes; false otherwise
 */
bool NormalizeWord(char* word);

#endif // __WORD_H
