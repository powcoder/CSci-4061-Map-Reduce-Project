https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
#include "reducer.h"

finalKeyValueDS *createFinalKeyValueNode(char *word, int count){
	finalKeyValueDS *newNode = (finalKeyValueDS *)malloc (sizeof(finalKeyValueDS));
	strcpy(newNode -> key, word);
	newNode -> value = count;
	newNode -> next = NULL;
	return newNode;
}

finalKeyValueDS *insertNewKeyValue(finalKeyValueDS *root, char *word, int count){
	finalKeyValueDS *tempNode = root;
	if(root == NULL)
		return createFinalKeyValueNode(word, count);
	while(tempNode -> next != NULL){
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value += count;
			return root;
		}
		tempNode = tempNode -> next;
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value += count;
	} else{
		tempNode -> next = createFinalKeyValueNode(word, count);
	}
	return root;
}

void freeFinalDS(finalKeyValueDS *root) {
	if(root == NULL) return;

	finalKeyValueDS *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		free(tempNode);
	}
}

// reduce function
void reduce(char *key) {

	char* wordFileName = key;
	int fd = open(wordFileName, O_RDONLY);
	if (fd < 0){
		printf("ERROR: Cannot open the file %s\n", wordFileName);
		exit(0);
	}

	char buffer[MAXKEYSZ];
	char word[MAXKEYSZ];
	memset(word, '\0', sizeof(char) * MAXKEYSZ);
	int j = 0;
	int foundKey = 0;
	while(read(fd, &buffer[j], 1) == 1) {
		// read one word at a time
		if (buffer[j] == ' ' ||buffer[j] == '\n' || buffer[j] == 0x0) {
			buffer[j] = '\0';

			// just a safety code to avoid empty files or blank lines
			if(strlen(buffer) == 0){
				j = 0;
				continue;
			}

			int count;
			if(!foundKey){
				// found the word
				foundKey = 1;
				strcpy(word, buffer);
				count = 0;
			} else {
				// convert string count to integer count
				count = strtol(buffer, NULL, 10);
			}
			finalDS = insertNewKeyValue(finalDS, word, count);
			j = 0;
			continue;
		}
		j++;
	}
	close(fd);
}

void writeFinalDS(int reducerID){
	char reduceFileName[MAXKEYSZ];
	memset(reduceFileName, '\0', MAXKEYSZ);
	sprintf(reduceFileName, "output/ReduceOut/Reduce_%d.txt", reducerID);

	int fdReduce = open(reduceFileName, O_CREAT | O_WRONLY, 0777);
	if (fdReduce < 0){
		printf("ERROR: Cannot open the file %s\n", reduceFileName);
		exit(0);
	}

	finalKeyValueDS *tempNode = finalDS;
	while(tempNode){
		char writeKeyValue[2 * MAXKEYSZ];
		memset(writeKeyValue, '\0', 2 * MAXKEYSZ);
		strcpy(writeKeyValue, tempNode -> key);
		strcat(writeKeyValue, " ");
		char valueStr[MAXKEYSZ];
		memset(valueStr, '\0', MAXKEYSZ);
		sprintf(valueStr, "%d", tempNode -> value);
		strcat(writeKeyValue, valueStr);
		writeKeyValue[strlen(writeKeyValue)] = '\n';

		// word write
		int ret = write(fdReduce, writeKeyValue, strlen(writeKeyValue));
		if(ret < 0){
			printf("ERROR: Cannot write to file %s\n", reduceFileName);
			exit(0);
		}
		tempNode = tempNode -> next;
	}
	close(fdReduce);
}

int main(int argc, char *argv[]) {

	if(argc < 2){
		printf("Less number of arguments.\n");
		printf("./reducer reducerID");
	}

	// initialize 
	int reducerID = strtol(argv[1], NULL, 10);
	finalDS = NULL;

	// master will continuously send the word.txt files alloted to the reducer
	char key[MAXKEYSZ];
	while(getInterData(key, reducerID))
		reduce(key);

	writeFinalDS(reducerID);

	freeFinalDS(finalDS);
	return 0;
}