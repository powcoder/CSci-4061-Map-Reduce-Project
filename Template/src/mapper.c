https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
#include "mapper.h"

// combined value list corresponding to a word <1,1,1,1....>
valueList *createNewValueListNode(char *value){
	valueList *newNode = (valueList *)malloc (sizeof(valueList));
	strcpy(newNode -> value, value);
	newNode -> next = NULL;
	return newNode;
}

// insert new count to value list
valueList *insertNewValueToList(valueList *root, char *count){
	valueList *tempNode = root;
	if(root == NULL)
		return createNewValueListNode(count);
	while(tempNode -> next != NULL)
		tempNode = tempNode -> next;
	tempNode -> next = createNewValueListNode(count);
	return root;
}

// free value list
void freeValueList(valueList *root) {
	if(root == NULL) return;

	valueList *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		free(tempNode);
	}
}

// create <word, value list>
intermediateDS *createNewInterDSNode(char *word, char *count){
	intermediateDS *newNode = (intermediateDS *)malloc (sizeof(intermediateDS));
	strcpy(newNode -> key, word);
	newNode -> value = NULL;
	newNode -> value = insertNewValueToList(newNode -> value, count);
	newNode -> next = NULL;
	return newNode;
}

// insert <word, value> to intermediate DS
intermediateDS *insertPairToInterDS(intermediateDS *root, char *word, char *count){
	intermediateDS *tempNode = root;
	if(root == NULL)
		return createNewInterDSNode(word, count);
	while(tempNode -> next != NULL) {
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value = insertNewValueToList(tempNode -> value, count);
			return root;
		}
		tempNode = tempNode -> next;
		
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value = insertNewValueToList(tempNode -> value, count);
	} else {
		tempNode -> next = createNewInterDSNode(word, count);
	}
	return root;
}

// free intermediate DS
void freeInterDS(intermediateDS *root) {
	if(root == NULL) return;

	intermediateDS *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		freeValueList(tempNode -> value);
		free(tempNode);
	}
}

// emit the <key, value> into intermediate DS 
void emit(char *key, char *value) {
	// printf("To be emitted: %s, %s\n", (char *)key, value);
	interDS = insertPairToInterDS(interDS, key, value);
}

// map function
void map(char *chunkData){
	char *cData = chunkData;
	char *buffer;
	int i = 0;

	while((buffer = getWord(cData, &i)) != NULL){
		emit(buffer, "1");
		free(buffer);
		buffer = NULL;
	}
}

// generate the file name for word.txt
char *generateWordFileName(char *word){
	char fileName[MAXKEYSZ];
	memset(fileName, '\0', MAXKEYSZ);
	strcpy(fileName, word);
	strcat(fileName, ".txt");
	fileName[strlen(fileName)] = '\0';

	char *wordFileName = (char*)malloc(sizeof(char) * (2 * MAXKEYSZ));
	memset(wordFileName, '\0', (2 * MAXVALUESZ));
	strcpy(wordFileName, mapOutDir);
	strcat(wordFileName, "/");
	strcat(wordFileName, fileName);
	wordFileName[strlen(wordFileName)] = '\0';

	return wordFileName;
	
}

// write intermediate data to separate word.txt files
// Each file will have only one line <word 1 1 1 1 1 ...>
void writeIntermediateDS() {
	intermediateDS *travNode = interDS;

	while(travNode != NULL) {
		// create file word.txt
		// content : word 1 1 1 1 1 1.....
		char *wordFileName = generateWordFileName(travNode -> key);
		int fd = open(wordFileName, O_CREAT | O_WRONLY, 0777);
		if (fd < 0){
			printf("ERROR: Cannot open the file %s\n", wordFileName);
			exit(0);
		}

		// word write
		int ret = write(fd, travNode -> key, strlen(travNode -> key));
		if(ret < 0){
			printf("ERROR: Cannot write to file %s\n", wordFileName);
			exit(0);
		}

		// append space
		ret = write(fd, " ", 1);
		if(ret < 0){
			printf("ERROR: Cannot write to file %s\n", wordFileName);
			exit(0);
		}

		// append value list
		valueList *tNode = travNode -> value;
		while(tNode -> next != NULL){
			ret = write(fd, tNode -> value, strlen(tNode -> value)); // space after the word
			if(ret < 0){
				printf("ERROR: Cannot write to file %s\n", wordFileName);
				exit(0);
			}

			// append space
			ret = write(fd, " ", 1);
			if(ret < 0){
				printf("ERROR: Cannot write to file %s\n", wordFileName);
				exit(0);
			}
			tNode = tNode -> next;
		}

		// last value
		ret = write(fd, tNode -> value, strlen(tNode -> value)); // space after the word
		if(ret < 0){
			printf("ERROR: Cannot write to file %s\n", wordFileName);
			exit(0);
		}

		// append newline
		ret = write(fd, "\n", 1);
		if(ret < 0){
			printf("ERROR: Cannot write to file %s\n", wordFileName);
			exit(0);
		}
		//close file
		close(fd);

		// go to next word in intermediate DS
		travNode = travNode -> next;
	}
}

int main(int argc, char *argv[]) {
	
	if (argc < 2) {
		printf("Less number of arguments.\n");
		printf("./mapper mapperID\n");
		exit(0);
	}

	// initializing global variables
	mapperID = strtol(argv[1], NULL, 10);
	interDS = NULL;

	//create folder specifically for this mapper in output/MapOut
	mapOutDir = createMapDir(mapperID);


	int count = 0;
	while(1) {
		char chunkData[chunkSize + 1];
		memset(chunkData, '\0', chunkSize + 1);

		char *retChunk = getChunkData(mapperID);
		if(retChunk == NULL) {
			break;
		}
		count++;
		strcpy(chunkData, retChunk);
		free(retChunk);

		map(chunkData);
	}

	//student code
	writeIntermediateDS();
	freeInterDS(interDS);

	return 0;
}