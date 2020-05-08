/* Author: Erich Woo
 * Date: 6 May 2020
 * Purpose: header file for pagedir module
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"

/******************* functions ******************/

/******************** dir_exists ************************/
/* Helper function to check if given directory exists                                       
 * @param dir the given directory                                                           
 * @return 0 if directory exists, 1 otherwise                                               
 */
int dir_exists(char* dir);

/************** is_valid_page ****************/                                                    
/* Helper method for consilidating the error-checking for a given webpage                   
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
int is_valid_page(webpage_t* page, bool html, bool message);

/**************** pagefetcher ****************/
/* Fetches the html for webpage and adds it to the struct                                   
 * Assumes page allocated by caller                                                         
 *                                                                                          
 * @param page the webpage with url, depth, NULL html                                       
 * @return 0 on successful fetch, 1 otherwise                                               
 */
int pagefetcher(webpage_t* page);

/******************* pagesaver ********************/
/* Writes a page's html to a file                                                           
 * Assumes fp is error-checked outside of call for writability                              
 *                                                                                          
 * @param fp the file to write into                                                         
 * @param page the page to write                                                            
 * @return 0 for success, 1 for error                                                       
 */
int pagesaver(FILE* fp, webpage_t* page);

/**************** pagescanner *********************/
/* Scans a page for the next URL, updating pos                                              
 * @param page the webpage to scan                                                          
 * @param pos the position to start scanning                                                
 * @return the next extracted URL, NULL otherwise                                           
 */
char* pagescanner(webpage_t* page, int* pos);

#endif // __PAGEDIR_H
