#include "index.h"

//used along with find indexFind
int sameIndexes(IndexPtr i1, IndexPtr i2){
	return (strcmp(i1->filename, i2->filename) == 0) ? 1 : 0;
}

//compare function to sort by word frequency
int sortFrequencies(IndexPtr i1, IndexPtr i2){
	return (i1->wordCount < i2->wordCount) ? 1 : (i1->wordCount > i2->wordCount) ? -1 : 0;
}

IndexPtr indexCreate(char * file){
	char * fileptr = NULL;

	IndexPtr index = (IndexPtr) malloc(sizeof(struct IndexItem));

	//must allocate memory for the file char or else it'll be lost
	if(file != NULL){
		fileptr = (char *) malloc(strlen(file)+1);
		strcpy(fileptr, file);
	}

	//illegal path names
	if(fileptr == NULL || strcmp(fileptr, "") == 0){
		free(index);
		return NULL;
	}
	
	index->filename = fileptr;
	index->wordCount = 1;
	index->next = NULL;

	return index;
}

IndexPtr indexFind(IndexPtr listOfIndexes, IndexPtr indexToFind){
	IndexPtr traverse = listOfIndexes; //create a separate pointer so list isn't lost

	if(listOfIndexes == NULL || listOfIndexes->filename == NULL ) //if the list is empty
		return NULL;
	else{
		while(traverse->next != NULL){
			if(sameIndexes(traverse, indexToFind))
				return traverse;
			traverse=traverse->next;
		}
	}
	
	//if it falls outside of loop it wasn't found, it returns the address to the last item so that it could be added
	return traverse;
}

void removeIndex(IndexPtr *listRef, IndexPtr toRemove){
	IndexPtr currNode = *listRef;
	
	if(currNode == NULL || toRemove == NULL) //either list or item to add are empty
		return;
	else if(sameIndexes(currNode, toRemove)){ //compare it with head
		*listRef = (*listRef)->next;
		return;
	}
	else{
		IndexPtr nextNode = currNode->next;

		while(nextNode != NULL){
			if(sameIndexes(nextNode, toRemove)){
				currNode->next = nextNode->next;
				free(nextNode->filename);
				free(nextNode);
				return;
			}
			currNode = nextNode;
			nextNode = nextNode->next;
		}
	}
}

IndexPtr copyList(IndexPtr list){
	if(list == NULL)
		return NULL;
	
	IndexPtr listCopy = indexCreate(list->filename); //copy the head of the other list
	IndexPtr tail = listCopy; //tail will always be at the end of the list
	
	IndexPtr traverse = list->next; //head was already added so make traverse next item

	while(traverse != NULL){
		tail->next = indexCreate(traverse->filename);
		tail = tail->next;
		traverse = traverse->next;	
	}
	return listCopy;
}

void printListPA4(IndexPtr list){
	IndexPtr printer = list;
	if(printer == NULL){
		printf("List is empty\n");
		return;
	}

	while(printer->next != NULL){
		printf("%s, ", printer->filename);
		printer = printer->next;
	}
	printf("%s\n", printer->filename);
}

void printListforRead(IndexPtr list){
	IndexPtr printer = list;
	if(printer == NULL){
		printf("List is empty\n");
		return;
	}

	while(printer != NULL){
		printf("%s\n", printer->filename);
		printer = printer->next;
	}
}

void destroyIndexList(IndexPtr head){
	IndexPtr deleter;
	while(head!=NULL){
		deleter = head;
		head = head->next;
		free(deleter->filename);
		free(deleter);
	}
}