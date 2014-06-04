#ifndef INDEX_H
#define INDEX_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//simply a linkedlist of index items, which holds the filename (full path) and the frequency of
//the word in that file
struct IndexItem{
	char * filename; 
	int wordCount;
	struct IndexItem * next;
};
typedef struct IndexItem * IndexPtr;

/*
* compares indexes so that the frequency can be incremented
* which simply means if there are more than one of this word in this file
* 1 means they are the same, 0 means they arent (true or false)
* Only the path is compared
*/
int sameIndexes(IndexPtr, IndexPtr);

/*
* Compare function used in mergesort.c, sorts by wordcount
*/
int sortFrequencies(IndexPtr, IndexPtr);

/*
* Creates an index item with the given path, allocates it in the heap
* Caller function is responsible for freeing the memory
*/
IndexPtr indexCreate(char *);

/*
 * Removes an index from a list of indexes
 * Uses sameIndexes as tbe compare function (compares by pathnames)
 * Function also frees the deleted index
 */
void removeIndex(IndexPtr*, IndexPtr);

/*
 * Makes an exact copy of the list and returns it
 * Only copies the pathname and not the wordcount, as it is not required for PA4
 * List is allocated dynamically in the heap so caller must free it
 * This is used so that original list (from the hash table) is not altered
 */
IndexPtr copyList(IndexPtr);

/*
 * Prints the list in the format asked for in PA4
 */
void printListPA4(IndexPtr);

/*
 * Added a new line character at the end of each index for easier reading
 */
void printListforRead(IndexPtr);

/*
 * Returns a pointer to an index being looked for in a given list
 * or a pointer to the last item in the list if it was not found (so that it could be added to the end of the list)
 * IMPORTANT: must use same the sameIndexes compare function on returned index
 */
IndexPtr indexFind(IndexPtr, IndexPtr);

/*
 * Destroys ALL the indexes given a list, used to delete the copy of the list in the copyList function
 * is also used to clear the hash table when caller is done with it
 */
void destroyIndexList(IndexPtr);

#endif