#ifndef HASHMAP_H
#define HASHMAP_H

//credit for the hashmap backend is given to UTHash from: http://troydhanson.github.io/uthash/
#include "uthash.h"
#include "index.h"

//holds the key and a LinkedList of Indexes, from index.c
//UT_hash_handle is what uthash uses to create our hashmap
struct Hashmap{
	char * key;
	IndexPtr indexList;
	UT_hash_handle hh;
};
typedef struct Hashmap * hashPtr;

/*
 * Used to clear the hash and free all memory when caller is done with it
 */
void clearHash(hashPtr *);

/*
 * Adds an index to a hash, given a key (the token)
 */
void addIndex(hashPtr *, char *, IndexPtr);

/*
 * A get function for the hashmap
 * Returns the linked list of indexes given a certain key (a word)
 */
IndexPtr getValue(char *, hashPtr *);

 /*
 * Used for debugging purposes only
 */
void printTable(hashPtr *);


#endif