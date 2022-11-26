https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
#include "utils.h"
#include <sys/ipc.h>

#define PERM 0666


//recieve next tagged chunk from the queue
char *getChunkData(int mapperID) {

  key_t key;
  int msgid;
  struct msgBuffer msg;
  key = ftok(".", 5226197);

	//open message queue
	if ((msgid = msgget(key, PERM | IPC_CREAT)) < 0) {
    printf("msg queue opening error\n");
    exit(1);
  }

	//recieve chunk from the master
	if(msgrcv(msgid, &msg, sizeof(msg.msgText), mapperID, 0) < 0) {
		printf("msgrcv error in reciever\n");	//is this the END msg?
		exit(0);
	}

  if (strcmp(msg.msgText, "END") == 0)
    return NULL;

  return 0;
}
	//check for END msg and send ACK to master
	// if(chunkdata == END){
	// 	//send ACK message
	// }


// requires more detailed coding to correctly split words
void sendChunkData(char *inputFile, int nMappers) {

  key_t key;
  int msgid;
  struct msgBuffer msg;
  key = ftok(".", 5226197);

	//open message queue
	if ((msgid = msgget(key, PERM | IPC_CREAT)) < 0) {
    printf("msg queue opening error\n");
    exit(1);
  }

	FILE *fp = fopen(inputFile, "r");
	if (fp == NULL) {
		printf("file open error\n");
    exit(1);
  }

	char wordBuff[101];
  char chunkBuff[1024];

	int wordcount = 0;
	int chunkcount = 0;
	int spaceleft = 1024;
  int c;

  fseek(fp, 0, SEEK_SET);

	while (1) {
    c = fgetc(fp);
    if (c == EOF) {
      break;
    }

    // check if c is an alphabet.
    // if yes, append c to wordBuff, and increment wordcount
		if (isalpha(c)) {
      wordBuff[wordcount] = c;
      wordcount++;
      printf("%s", wordBuff);
    }

    // if no, check if remaining space is enough to contain the next word
    else {
      // if the space left is large enough,
      // then append the next word into chunkBuff
      if (spaceleft >= wordcount) {
        spaceleft -= wordcount;
        strncpy(chunkBuff, wordBuff, wordcount);
        if (spaceleft - wordcount > 0) {
          sprintf(chunkBuff, "%c", c);
          spaceleft--;
        }
        printf("%s\n", chunkBuff);
        wordcount = 0;
        memset(wordBuff, '\0', 101 * sizeof(wordBuff[0]));
      }

      // if the space left is not enough
      // then msgsnd the current chunkData, and empty chunkBuff
      else {
        strcat(msg.msgText, chunkBuff);
        spaceleft = 1024;
        memset(chunkBuff, '\0', 1024 * sizeof(chunkBuff[0]));
        strncpy(chunkBuff, wordBuff, wordcount);
        sprintf(chunkBuff, "%c", c);
        memset(wordBuff, '\0', 101 * sizeof(wordBuff[0]));

        //msgsnd, msgctl, msgsnd
		     //if ((msgsnd(msgid, &msg, sizeof(msg.msgText), IPC_NOWAIT)) < 0) {
        if ((msgsnd(msgid, &msg, sizeof(msg.msgText), 0)) < 0) {
			    printf("msgsnd error in sender 1 \n");
			    exit(0);
		    }

        strcpy(msg.msgText, "END");
        msgsnd(msgid, &msg, sizeof(msg.msgText), 0);
        msgctl(msgid, IPC_RMID, 0);

        for (int i = 0; i < nMappers; i++) {
          memset(msg.msgText, '\0', MSGSIZE);
          msg.msgType = i + 1;
          strcat(msg.msgText, "END");
          printf("[%d] Data sent is : %s \n", i, msg.msgText);

          if ((msgsnd(msgid, &msg, sizeof(msg.msgText), IPC_NOWAIT)) < 0) {
            printf("msgsnd error in sender 1\n");
            exit(0);
          }
          // display the message
          printf("[%d] Data sent is : %s \n", i, msg.msgText);
        }
      }
      fclose(fp);
    }
  }
}


// hash function to divide the list of word.txt files across reducers
//http://www.cse.yorku.ca/~oz/hash.html
int hashFunction(char* key, int reducers){
	unsigned long hash = 0;
    int c;
    while ((c = *key++)!='\0')
        hash = c + (hash << 6) + (hash << 16) - hash;
    return (hash % reducers);
}


int getInterData(char *key, int reducerID) {

  key_t k;
  int msgid;
  struct msgBuffer msg;
  k = ftok(".", 5226197);

  //open message queue
  if ((msgid = msgget(k, PERM | IPC_CREAT)) < 0) {
    printf("msg queue opening error\n");
    return -1;
  }

	// recieve chunk from the master
	if(msgrcv(msgid, &msg, sizeof(msg.msgText), reducerID, 0) < 0) {
		printf("msgget error in getInterData\n");
		return -1;
	}

	// check for END msg
  if (strcmp(msg.msgText, "END") == 0)
    return 0;

  return 0;
}


// void shuffle(int nMappers, int nReducers) {
//
//   key_t key;
//   int msgid;
//   struct msgBuffer msg;
//   key = ftok(".", 5226197);
//
//   //open message queue
//   if (msgid = msgget(key, PERM | IPC_CREAT) < 0) {
//     printf("msg queue opening error\n");
//     exit(1);
//   }
//
// 	int i,j;
// 	char path[15] = "output/MapOut";
// 	DIR* mapOutDir = opendir(path);
//   // allocating buffer for path name of mapper folders
//   char mapdir[51];
//
//   // open MapOut dir
// 	if ((msgid = msgget (key, PERM | IPC_CREAT )) < 0) {		//error checking for msgget
// 		printf("msgget error in shuffle\n");
// 	}
//   if (mapOutDir == NULL) {											//error checking for opening directory
//     printf("The path passed is invalid");
//   }
// 	struct dirent* entry;
//
// 	for (i = 0; i < nMappers; i++) {
// 		sprintf(mapdir, "output/MapOut/Map_%d", i + 1); // constructing file path towards map_i th file
// 		DIR* mapperdir = opendir(mapdir);			//opening the map_i th dir
//
//     // loop through every Mapper folder
// 		while((entry = readdir(mapperdir)) != NULL) {
//       // error checking
// 			if (!strcmp(entry -> d_name, ".") || !strcmp(entry -> d_name, ".."))
// 			continue;
//       // select reducer using a hash function
// 			int reducerID = hashFunction(entry -> d_name, nReducers) + 1;
//       msg.msgType = reducerID;
//       strcpy(msg.msgText, mapdir);
//
//       if ((msgsnd(msgid, &msg, sizeof(msg.msgText), IPC_NOWAIT)) < 0) {
//         printf("msgsnd error in shuffle\n");
//         exit(0);
//       }
//     }
//     mapdir[0] = '\0';
// 		closedir(mapperdir);						//closing map_i th dir
// 	}
// 	closedir(mapOutDir);
// }




void shuffle(int nMappers, int nReducers) {
  key_t key;
  int msgid;
  struct msgBuffer msg;
  key = ftok(".", 5226197);

  //open message queue
  if ((msgid = msgget(key, PERM | IPC_CREAT)) < 0) {
    printf("msg queue opening error\n");
    exit(1);
  }

	int i,j;
	char path[15] = "output/MapOut";
	int reducerID;
  //  buffer for path name of mapper folders
  char mapdir[51];
	DIR* mapOutDir = opendir(path);

  if (mapOutDir == NULL) {											//error checking for opening directory
      printf("The path passed is invalid");
  }
	struct dirent* entry;
	for (i = 0; i < nMappers; i++) {
		sprintf(mapdir, "output/MapOut/Map_%d", i); // constructing file path towards map_i th file
		DIR* mapperdir = opendir(mapdir);			//opening the map_i th dir

		while((entry = readdir(mapperdir)) != NULL) {		//inside map_i
			if (!strcmp(entry->d_name, ".") || !strcmp(entry -> d_name, "..")) //error checking
			continue;
      // if its a directory its an unexpected error
			if(entry->d_type == DT_DIR) {
				printf("unexpected filetype (This should be a file type not a dir)");
      }
      // if its a regular file then it must be a wordfile
			else if (entry->d_type ==DT_REG){
        // creating a buffer for wordfilepath
				char wordFilePath[strlen(mapdir)+strlen(entry->d_name)+2];
        // putting word filepath to the buffer wordFilePath
				sprintf(wordFilePath,"output/MapOut/Map_%d/%s",i,entry->d_name);
        // select reducer using a hash function
				reducerID = hashFunction(entry -> d_name,nReducers)+1;
				memset(msg.msgText, '\0', MSGSIZE);
				strcpy(msg.msgText, wordFilePath);
				msg.msgType = reducerID;
				if ((msgsnd(msgid, &msg, sizeof(msg.msgText),0) )< 0) {
					printf("msgsnd error in sender\n");
					exit(0);
				}
			} else {
				printf("unexpected error");
			}
   	}
    mapdir[0] = '\0';
		closedir(mapperdir);						//closing map_i th dir
	}
	closedir(mapOutDir);
}


// check if the character is valid for a word
int validChar(char c){
	return (tolower(c) >= 'a' && tolower(c) <='z') ||
					(c >= '0' && c <= '9');
}


char *getWord(char *chunk, int *i){
	char *buffer = (char *)malloc(sizeof(char) * chunkSize);
	memset(buffer, '\0', chunkSize);
	int j = 0;
	while((*i) < strlen(chunk)) {
		// read a single word at a time from chunk
		// printf("%d\n", i);
		if (chunk[(*i)] == '\n' || chunk[(*i)] == ' ' || !validChar(chunk[(*i)]) || chunk[(*i)] == 0x0) {
			buffer[j] = '\0';
			if(strlen(buffer) > 0){
				(*i)++;
				return buffer;
			}
			j = 0;
			(*i)++;
			continue;
		}
		buffer[j] = chunk[(*i)];
		j++;
		(*i)++;
	}
	if(strlen(buffer) > 0)
		return buffer;
	return NULL;
}


void queueDestory() {
  key_t key;
  int msgid;
  struct msgBuffer msg;
  key = ftok(".", 5226197);

  //open message queue
  if ((msgid = msgget(key, PERM | IPC_CREAT)) < 0) {
    printf("msg queue opening error\n");
    exit(1);
  }

	msgctl(msgid, IPC_RMID, NULL);
}


void createOutputDir() {
	mkdir("output", ACCESSPERMS);
	mkdir("output/MapOut", ACCESSPERMS);
	mkdir("output/ReduceOut", ACCESSPERMS);
}

char *createMapDir(int mapperID){
	char *dirName = (char *) malloc(sizeof(char) * 100);
	memset(dirName, '\0', 100);
	sprintf(dirName, "output/MapOut/Map_%d", mapperID);
	mkdir(dirName, ACCESSPERMS);
	return dirName;
}

void removeOutputDir(){
	pid_t pid = fork();
	if(pid == 0){
		char *argv[] = {"rm", "-rf", "output", NULL};
		if (execvp(*argv, argv) < 0) {
			printf("ERROR: exec failed\n");
			exit(1);
		}
		exit(0);
	} else{
		wait(NULL);
	}
}

void bookeepingCode(){
	removeOutputDir();
	sleep(1);
	createOutputDir();
}
