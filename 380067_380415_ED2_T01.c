/*
	Authors:	Rodrigo Nascimento de Carvalho 380067
				Philippe César Ramos 380415
	Description: 	This is a League of Legends Championship matches database, built using files and indexes. 
					This is the library used to manipulate the indexes and files for this application.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "380067_380415_ED2_T01.h"

FILE* fileExists (FILE *file, char* name) {
	if ((file = fopen (name, "r+")) != NULL) {
		return file;
	} else {
		return NULL;
	}
}

FILE* createFile (FILE *file, char* name) {
	file = fopen (name, "w+");
	return file;
}

int compareKeys (const void *a, const void *b) {
	const primaryIndex *primary_a = (primaryIndex*) a;
	const primaryIndex *primary_b = (primaryIndex*) b;
	return strcmp (primary_a->primaryKey, primary_b->primaryKey);
}

int compareWinnerKeys (const void *a, const void *b) {
	const winnerIndex *winner_a = (winnerIndex*) a;
	const winnerIndex *winner_b = (winnerIndex*) b;
	return strcmp (winner_a->primaryKey, winner_b->primaryKey);
}

int compareWinner (const void *a, const void *b) {
	const winnerIndex *winner_a = (winnerIndex*) a;
	const winnerIndex *winner_b = (winnerIndex*) b;
	return strcmp (winner_a->winner, winner_b->winner);
}

int compareMVPKeys (const void *a, const void *b) {
	const mvpIndex *mvp_a = (mvpIndex*) a;
	const mvpIndex *mvp_b = (mvpIndex*) b;
	return strcmp (mvp_a->primaryKey, mvp_b->primaryKey);
}

int compareMVP (const void *a, const void *b) {
	const mvpIndex *mvp_a = (mvpIndex*) a;
	const mvpIndex *mvp_b = (mvpIndex*) b;
	return strcmp (mvp_a->mvpNickname, mvp_b->mvpNickname);
}

void savePrimaryIndex (FILE *file, primaryIndex *primaryIndexArray, int size) {
	int i;
	fprintf(file, "%d\n", 1);
	for (i = 0; i < size; i += 1) {
		fprintf(file, "%s@%d@\n", primaryIndexArray[i].primaryKey, primaryIndexArray[i].offset);
	}
}

void saveWinnerIndex (FILE *file, winnerIndex *winnerIndexArray, int size) {
	int i;
	for (i = 0; i < size; i += 1) {
		fprintf(file, "%s@%s@\n", winnerIndexArray[i].winner ,winnerIndexArray[i].primaryKey);
	}
}

void saveMVPIndex (FILE *file, mvpIndex *mvpIndexArray, int size) {
	int i;
	for (i = 0; i < size; i += 1) {
		fprintf(file, "%s@%s@\n", mvpIndexArray[i].mvpNickname , mvpIndexArray[i].primaryKey);
	}
}

void loadPrimaryIndex (FILE *file, primaryIndex *primaryIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%d@\n", primaryIndexArray[i].primaryKey, &primaryIndexArray[i].offset) == 2) {
		i++;
	}
}

void loadWinnerIndex (FILE *file, winnerIndex *winnerIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%[^@]@\n", winnerIndexArray[i].winner ,winnerIndexArray[i].primaryKey) == 2) {
		i++;
	}
}

void loadMVPIndex (FILE *file, mvpIndex *mvpIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%[^@]@\n", mvpIndexArray[i].mvpNickname , mvpIndexArray[i].primaryKey) == 2) {
		i++;
	}
}

void createIndexes (FILE* dataFile, FILE* primaryFile, primaryIndex *primaryIndexArray, 
						FILE* winnerFile, winnerIndex *winnerIndexArray, 
						FILE* mvpFile, mvpIndex *mvpIndexArray) {
	int registerCount, fileSize, i, freturn;
	char* test;

	if(fseek (dataFile, 0, SEEK_END) == 0) {
		fileSize = ftell (dataFile);
	}
	registerCount = fileSize / REG_SIZE;

	for (i = 0; i < registerCount; i += 1) {
		fseek (dataFile, (REG_SIZE * i), SEEK_SET);

		fscanf (dataFile, "%[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%[^@]@%*[^@]@%*[^@]@%[^@]@", 
				primaryIndexArray[i].primaryKey, winnerIndexArray[i].winner, mvpIndexArray[i].mvpNickname);

		strcpy (winnerIndexArray[i].primaryKey, primaryIndexArray[i].primaryKey);
		strcpy (mvpIndexArray[i].primaryKey, primaryIndexArray[i].primaryKey);

		primaryIndexArray[i].offset = REG_SIZE * i;
	}
	/*
		Sort primary index, save primary index
	*/
	qsort (primaryIndexArray, registerCount, sizeof(primaryIndex), compareKeys);
	savePrimaryIndex (primaryFile, primaryIndexArray, registerCount);

	/*
		Sort winner index first by primary index, then by name, save primary index
	*/
	qsort (winnerIndexArray, registerCount, sizeof(winnerIndex), compareWinnerKeys);
	qsort (winnerIndexArray, registerCount, sizeof(winnerIndex), compareWinner);
	saveWinnerIndex (winnerFile, winnerIndexArray, registerCount);

	/*
		Sort mvp index first by primary index, then by name, save primary index
	*/
	qsort (mvpIndexArray, registerCount, sizeof(mvpIndex), compareMVPKeys);
	qsort (mvpIndexArray, registerCount, sizeof(mvpIndex), compareMVP);
	saveMVPIndex (mvpFile, mvpIndexArray, registerCount);
}

int checkIndexConsistency (FILE* primaryFile) {
	int isOk; 
	fscanf(primaryFile, "%d\n", &isOk);
	return isOk;
}

void loadIndexes (FILE* primaryFile, primaryIndex *primaryIndexArray, 
						FILE* winnerFile, winnerIndex *winnerIndexArray, 
						FILE* mvpFile, mvpIndex *mvpIndexArray) {
	loadPrimaryIndex (primaryFile, primaryIndexArray);
	loadWinnerIndex (winnerFile, winnerIndexArray);
	loadMVPIndex (mvpFile, mvpIndexArray);
}

void insertMatch (FILE* dataFile, lolMatch match) {

}

lolMatch searchMatch (FILE* dataFile, char* query, int searchOption) {

}

void removeMatch (FILE* dataFile, char* primaryKey) {

}

void printMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex **index, int searchOption) {

}

void printMatchesOrderByWinner (FILE* dataFile, winnerIndex **index, int searchOption) {

}

void printMatchesOrderByMVP (FILE* dataFile, mvpIndex **index, int searchOption) {

}

void freeSpace (FILE** dataFile, primaryIndex **primaryIndex, winnerIndex **winnerIndex, mvpIndex **mvpIndex) {

}

void updateIndexes (primaryIndex *primaryIndex, winnerIndex *winnerIndex, mvpIndex *mvpIndex) {

}
