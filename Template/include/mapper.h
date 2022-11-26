https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
#ifndef MAPPER_H
#define MAPPER_H

#include "utils.h"

// ###### DO NOT REMOVE ######
#define MAXKEYSZ 100
#define MAXVALUESZ 100

// ###### DO NOT REMOVE ######
char *mapOutDir;
int mapperID;


// You are free to change the intermediate data structure as it suits you
// If you do so, ensure the provided utility functions are also changed
// 1 1 1...
typedef struct valueList {
	// MAXVALUESZ can be reduced to a small value as you are only storing "1"
	char value[MAXVALUESZ];
	struct valueList *next;
}valueList;

// word 1 1 1...
typedef struct intermediateDS{
	char key[MAXKEYSZ];
	valueList *value;
	struct intermediateDS *next;
}intermediateDS;

intermediateDS *interDS = NULL;

// ###### DO NOT REMOVE ######
valueList *createNewValueListNode(char *value);
valueList *insertNewValueToList(valueList *root, char *count);
void freeValueList(valueList *root);

// ###### DO NOT REMOVE ######
intermediateDS *createNewInterDSNode(char *word, char *count);
intermediateDS *insertPairToInterDS(intermediateDS *root, char *word, char *count);
void freeInterDS(intermediateDS *root);

// ###### DO NOT REMOVE ######
void emit(char *key, char *value);
void map(char *chunkData);
void writeIntermediateDS();

#endif