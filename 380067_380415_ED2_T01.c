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

int checkDataFile (void) {
	FILE* file;
	if ((file = fopen ("matches.dat", "r+")) != NULL) {
		return 1;
	} else {
		return 0;	
	}
}

int checkPrimaryIndex (void) {
	FILE* indexFile;
	if ((indexFile = fopen ("iprimary.idx", "r+")) != NULL) {
		return 1;
	} else {
		return 0;	
	}
}

int checkWinnerIndex (void) {
	FILE* winnerFile;
	if ((winnerFile = fopen ("iwinner.idx", "r+")) != NULL && ) {
		return 1;
	} else {
		return 0;	
	}
}

int checkMVPIndex (void) {
	FILE* mvpFile;
	if ((mvpFile = fopen ("iwinner.idx", "r+")) != NULL && ) {
		return 1;
	} else {
		return 0;	
	}
}

void createPrimaryIndex (FILE* dataFile) {
	
}

void createSecondaryIndexes (FILE* dataFile) {

}

void loadIndexes (primaryIndex **primaryIndex, winnerIndex **winnerIndex, mvpIndex **mvpIndex) {

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
