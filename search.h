#ifndef STORE_H
#define STORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashmap.h"
#include "tokenizer.h"

//default max number of characters for the query, can change if a larger query is required
#define QUERYSIZE 500

/*
 * Parses a file in the format given by PA4 and adds the words and their
 * respective indexes in a hashtable
 */
hashPtr storeKeys(char *);

/*
 * Returns the size of the file given in order to allocate enough memory for the buffer
 * in case a new line is not added at the end of the indexes (or every 5 indexes)
 * This prevents tokens from being broken up, this is very important because 
 * storeKeys follows a very specific format and can break if it receives the wrong token
 */
long getFileSize(FILE *);


#endif
