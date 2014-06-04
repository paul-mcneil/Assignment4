#include "search.h"

long getFileSize(FILE *fp){
	long fileSize;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);

	return fileSize;
}

hashPtr storeKeys(char * filename){
	FILE * fp;
	hashPtr invertedHash = NULL;

	if((fp = fopen(filename, "r")) == NULL){
		perror("Error trying to open file");
		printf("Program terminated\n");
		exit(1);
	}
	else{
		long fileSize = getFileSize(fp);
		char buffer[fileSize]; //in case long index lists from file dont contain the required new line every 5 indexes
		TokenizerT *tk;
		char *data; //string read from tokenizer
		char *word, *path; //the word (after the <list> tag) and its path (which will be reset for every index the word contains)
		int frequency; //the frequency of the word in path
		IndexPtr list, tail, tempIndex; //list is the linked list of indexes for the current word
		//tail and tempIndex are used to help insert it into the list

		//nothing to do if file is empty
		//program can exit
		if(fileSize == 0){
			printf("File given was empty, please input another file.\nProgram exiting.\n");
			exit(1);
		}

		while(fgets(buffer, fileSize, fp) != NULL){
			tk = TKCreate(" \n", buffer); //In the indexed file we care about everything but spaces and newlines

			while((data = TKGetNextToken(tk)) != NULL){
				//the end of our previous word, our list for that word is filled at this point
				if(strcmp(data, "<\\list>") == 0 || strcmp(data, "</list>") == 0){ //either <\list> or </list> is taken as the closing tag
					//delete the place holder used
					tail = list; 
					list = list->next;
					free(tail);
					
					//add word and its list to the hash
					addIndex(&invertedHash, word, list);
					
					break;
				}
				else if(strcmp(data, "<list>") == 0){ //beginning of our list, create new list for new word
					word = TKGetNextToken(tk); //the next token is the word (it is separated by a space)
					
					list = indexCreate("PLACE HOLDER"); //so we dont have to worry about which is the first call to add to list
					tail = list; //tail will be at the end of list so we don't have to traverse it everytime
				}
				else{ //all other tokens will be either the path or the wordcount
					path = data;

					//since no wordcount should be 0 we can use it to catch errors such as
					//a file in the incorrect format or if buffer was to small and tokens got broken up
					//although we used the filesize as the buffer size to avoid this problem
					//a conditional assingment was used since atoi crashes if given a NULL string
					frequency = ((data = TKGetNextToken(tk)) != NULL) ? (atoi(data)) : (0);

					if(frequency == 0){
						printf("Error: File given is in the incorrect format or buffer was too small and a token got broken up\n");
						printf("Program exiting\n");
						exit(1);
					}
					
					if((tempIndex = indexCreate(path)) == NULL) //NULL is returned from indexCreate if invalid path is given
						continue; //dont add it to the list

					tempIndex->wordCount = frequency;

					tail->next = tempIndex; //add it to the end of the list
					tail = tempIndex; 
				}
			}
		}	
	}
	if(fp != NULL)
		fclose(fp);
	return invertedHash;
}

void printUsage(){
	printf("\nSearch Usage:\n\tsa <term> ... \"logical AND\" query.\n");
	printf("\tso <term> ... \"logical OR\" query.\n\tq to exit program.\n\n");	
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Illegal number of arguments\n");
		printf("Interface: ./search <Name of indexed file>\n");
		exit(1);
	}
	else{
		char * inputFileName = argv[1];

		hashPtr indexHash = NULL; //hash storing the keys mapping to their respective paths
		IndexPtr MasterList = NULL; // Global Index List holds all paths of a given query
		IndexPtr tempList = NULL; // the value which a given word(key) points to - a series of indexes store for a word
		IndexPtr indexFound = NULL; //index returned when looking for a specific index

		indexHash = storeKeys(inputFileName);

		if(indexHash == NULL){
			printf("Error: File either contained all delimiters or contained no words\n");
			printf("Hash is empty, exiting program\n");
			exit(1);
		}
		else{
			printf("Successfully Loaded Inverted Index.");
			printUsage();
		}

		while(1) //prompts user for query until q is entred
		{	
			char * data;
			char inputLine[QUERYSIZE]; //QUERYSIZE can be cahnged in the header file
			char *searchOption; //either q, so, sa or invalid
			TokenizerT * tk;
			bool isFirstInstance = true; //used to set up the master index list
			printf("Enter a query option:\n");

			fgets(inputLine, QUERYSIZE, stdin); //get query from standard input
			
			//checks if query was longer than buffer size (words may have gotten cut off)
			if(strlen(inputLine) == QUERYSIZE-1){
				printf("Size of query exceeded buffer size, program will run but might not be accurate.\n");
				printf("Query size can be increased in the search.h header file.\n");
				printf("Continue? [y/n] > ");

				//fgets still holds the extra characters in memory
				//get rid of the extra characters that didnt fit in the buffer
				//so stdin is clear (needed if user wants to continue with the queries)
				int opt;
				while((opt = getc(stdin)) != '\n');

				if((opt = getc(stdin)) != 'y' && opt != 'Y'){
					printf("program exiting.\n");
					exit(0);
				}
			}
			
			tk = TKCreate(" \n", inputLine); //tokenize the input by spaces
			searchOption = TKGetNextToken(tk); //search option will be the first token

			if(searchOption == NULL)
			{
				printf("No Search Option Given.\n");
				printUsage();	
			}
			else if(strcmp(searchOption, "q") == 0)
			{
				printf("Program terminated\n");
				clearHash(&indexHash); //clear the hash before program exits
				exit(0);
			}
			else if(strcmp(searchOption, "sa") == 0)
			{
				IndexPtr MasterTraverse = NULL; //traverses the Master List so reference to master list is not lost

				while((data = TKGetNextToken(tk)) != NULL) //gets the words given from standard input
				{
					if((tempList = getValue(data, &indexHash)) == NULL){ // key given wasn't found, we can quit because query is logical and
						printf("One of the words given was not found in our stored dictionary.\n");
						destroyIndexList(MasterList);
						MasterList = NULL;
						break;
					}	
					else{
						//If this is the first instance of this current query fill the list 
						//Since for "SA" if the file is not in the first word, then it will not be in "all" of them
						//copyList makes a separate list in memory so we aren't altering what the hash points to directly
						if(isFirstInstance){
							MasterList = copyList(tempList);
							isFirstInstance = false;
						}
						else{
							MasterTraverse = MasterList;
							/* Traverse through MasterList indexes */
							//if index is in master list, but not in other word's indexes remove it from master list
							//since for "SA" the index must be included in all the words
							while(MasterTraverse != NULL){	
								//try to find the index from master list in the index list for a different word
								indexFound = indexFind(tempList, MasterTraverse);
								/* If index was not found in the list of indexes from MasterList */
								if(!sameIndexes(MasterTraverse, indexFound)) //not the same						
									removeIndex(&MasterList,MasterTraverse);			
								
								MasterTraverse = MasterTraverse->next;
							}
						}
					}					

				}
				//UNCOMMENT THIS IF YOU WANT THE FORMAT ASKED FOR IN PA4
				//printListPA4(MasterList);
				//printf("\n");

				//added new print format for easier reading
				//comment this out and uncomment the other one for the other format
				printListforRead(MasterList);
				printf("\n");

				//clear the list from this query and prepare it for the next one
				destroyIndexList(MasterList);
				MasterList = NULL;		
			}
			else if(strcmp(searchOption, "so") == 0)
			{
				while((data = TKGetNextToken(tk)) != NULL) //get the words given from std input
				{
					if((tempList = getValue(data, &indexHash)) == NULL) //key given wasnt found
						continue; //ignore it and move to the next word
					else{
						//if this is the first word copy the entire index list from the first word into master list
						if(isFirstInstance){
							MasterList = copyList(tempList);
							isFirstInstance = false;
						}
						else{ //not the first word
							while(tempList != NULL){
								//if the index from the new list does not yet exist in the master list, add it
								indexFound = indexFind(MasterList, tempList);
								if(!sameIndexes(indexFound, tempList)){
									indexFound->next = indexCreate(tempList->filename);
								}			
								tempList = tempList->next;
							}
						}
					}
				} 
				
				//UNCOMMENT THIS IF YOU WANT THE FORMAT ASKED FOR IN PA4
				//printListPA4(MasterList);
				//printf("\n");

				//added new print format for easier reading
				//comment this out and uncomment the other one for the other format
				printListforRead(MasterList);
				printf("\n");
				
				//clear master list and prepare it for the next query
				destroyIndexList(MasterList);
				MasterList = NULL;					
			}	
			else{
				printf("Invalid Option!\n");	
				printUsage();					
			}
		}
	}	
	return 0;
}
