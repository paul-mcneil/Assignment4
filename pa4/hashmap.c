#include "hashmap.h"

void clearHash(hashPtr *hash){
	hashPtr currentIndex, tempIndex;

	HASH_ITER(hh, *hash, currentIndex, tempIndex){
		destroyIndexList(currentIndex->indexList);
	}
	//uthash says that it frees the hash pointer once all items are deleted
}

IndexPtr getValue(char *key, hashPtr *hash_t){
	hashPtr keyExists;

	HASH_FIND_STR(*hash_t, key, keyExists);

	if(keyExists){
		return keyExists->indexList;
	}
	else
		return NULL;
}

void addIndex(hashPtr *hash, char * findKey, IndexPtr indexToAdd){
	hashPtr keyExists;
	IndexPtr indexFound;

	HASH_FIND_STR(*hash, findKey, keyExists); //checks hash if key is in there and gives it back to us in keyExists
	
	if(keyExists){ //keyExists is a pointer to a list of indexes
		indexFound = indexFind(keyExists->indexList, indexToAdd); //function from index.c
		//a pointer to the index or the end of the list is returned

		if(sameIndexes(indexFound, indexToAdd)){ //if the index was found (existed in the list already)
			indexFound->wordCount++;
			destroyIndexList(indexToAdd); //we don't need indexToAdd if it already exists in the list
		}
		else{ //if it wasn't found it returns a ptr to the end of the list
			indexFound->next = indexToAdd;
		}
	}
	else{ //if they key was not found in the hash we can store it in the bucket
		keyExists = (hashPtr) malloc(sizeof(struct Hashmap));
		keyExists->key = findKey;
		keyExists->indexList = indexToAdd;
		HASH_ADD_KEYPTR(hh, *hash, keyExists->key, strlen(keyExists->key),keyExists);
	}
}

//For debugging purposes
void printTable(hashPtr *hash_t){
	hashPtr printer;
	IndexPtr traverse;

	for(printer = *hash_t; printer!=NULL; printer=(hashPtr)(printer->hh.next)){
		printf("KEY \"%s\": ", printer->key);

		traverse = printer->indexList;
		while(traverse!=NULL){
			printf("(%d, %s) ", traverse->wordCount, traverse->filename);
			traverse = traverse->next;
		}
		printf("\n");
	}
}
