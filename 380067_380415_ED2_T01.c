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

int compareKeyWithWinner(const void *a, const void *b) { 
	const primaryIndex *primary_a = (primaryIndex*) a;
	const winnerIndex *winner_b = (winnerIndex*) b;
	return strcmp (primary_a->primaryKey, winner_b->primaryKey);	
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

/*
	Generic binarySearch, hopefully reusable for the future =D 
*/
int binarySearch (void* array, void* key, int start, int end, size_t size, int (*compare)(const void*, const void*)) {
	int middle;
	void *middleElement;
	if (end < start) { 
		return -1;
	} else {
		middle = start + ((end - start)/2);
		middleElement = array + middle * size;
		if ((*compare)(middleElement, key) < 0) { 
			binarySearch (array, key, middle + 1, end, size, compare);
		} else if ((*compare)(middleElement, key) > 0) { 
			binarySearch (array, key, start, middle - 1, size, compare);
		} else { 
			return middle;
		}
	}
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

int loadPrimaryIndex (FILE *file, primaryIndex *primaryIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%d@\n", primaryIndexArray[i].primaryKey, &primaryIndexArray[i].offset) == 2) {
		i++;
	}
	return i;
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

int createIndexes (FILE* dataFile, FILE* primaryFile, primaryIndex *primaryIndexArray, 
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

	return registerCount;
}

int checkIndexConsistency (FILE* primaryFile) {
	int isOk; 
	fscanf(primaryFile, "%d\n", &isOk);
	return isOk;
}

int loadIndexes (FILE* primaryFile, primaryIndex *primaryIndexArray, 
						FILE* winnerFile, winnerIndex *winnerIndexArray, 
						FILE* mvpFile, mvpIndex *mvpIndexArray) {
	int registerCount;
	registerCount = loadPrimaryIndex (primaryFile, primaryIndexArray);
	loadWinnerIndex (winnerFile, winnerIndexArray);
	loadMVPIndex (mvpFile, mvpIndexArray);
	return registerCount;
}

void insertMatch (FILE* dataFile, lolMatch match) {

}

lolMatch searchMatch (FILE* dataFile, char* query, int searchOption) {

}

void removeMatch (FILE* dataFile, char* primaryKey) {

}

void printMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, int size) {
	int i;
	lolMatch match;
	for (i = 0; i < size; i += 1) { 
		fseek (dataFile, index[i].offset, SEEK_SET);
		fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
				match.primaryKey, match.blueTeam, match.redTeam, match.date,
				match.matchDuration, match.winnerTeam, match.blueTeamScore,
				match.redTeamScore, match.mvpNickname);

		printf("%s\n", match.primaryKey);
		printf("%s\n", match.blueTeam);
		printf("%s\n", match.redTeam);
		printf("%s\n", match.date);
		printf("%s\n", match.matchDuration);
		printf("%s\n", match.winnerTeam);
		printf("%s\n", match.blueTeamScore);
		printf("%s\n", match.redTeamScore);
		printf("%s\n", match.mvpNickname);
		printf("\n");	
	}
}

void printMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, int size) {
	int i, primaryPosition;
	primaryIndex element;
	lolMatch match;
	for (i = 0; i < size; i += 1) {
		strcpy (element.primaryKey, index[i].primaryKey);
		primaryPosition = binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
		if (primaryPosition != -1) {
			fseek (dataFile, primaryArray[primaryPosition].offset, SEEK_SET);
			fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
					match.primaryKey, match.blueTeam, match.redTeam, match.date,
					match.matchDuration, match.winnerTeam, match.blueTeamScore,
					match.redTeamScore, match.mvpNickname);

			printf("%s\n", match.primaryKey);
			printf("%s\n", match.blueTeam);
			printf("%s\n", match.redTeam);
			printf("%s\n", match.date);
			printf("%s\n", match.matchDuration);
			printf("%s\n", match.winnerTeam);
			printf("%s\n", match.blueTeamScore);
			printf("%s\n", match.redTeamScore);
			printf("%s\n", match.mvpNickname);
			printf("\n");
		}
	}
}

void printMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, int size) {
	int i, primaryPosition;
	primaryIndex element;
	lolMatch match;
	for (i = 0; i < size; i += 1) {
		strcpy (element.primaryKey, index[i].primaryKey);
		primaryPosition = binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
		if (primaryPosition != -1) {
			fseek (dataFile, primaryArray[primaryPosition].offset, SEEK_SET);
			fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
					match.primaryKey, match.blueTeam, match.redTeam, match.date,
					match.matchDuration, match.winnerTeam, match.blueTeamScore,
					match.redTeamScore, match.mvpNickname);

			printf("%s\n", match.primaryKey);
			printf("%s\n", match.blueTeam);
			printf("%s\n", match.redTeam);
			printf("%s\n", match.date);
			printf("%s\n", match.matchDuration);
			printf("%s\n", match.winnerTeam);
			printf("%s\n", match.blueTeamScore);
			printf("%s\n", match.redTeamScore);
			printf("%s\n", match.mvpNickname);
			printf("\n");
		}
	}
}

void searchMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, primaryIndex element, int size) {
	int primaryPosition;
	lolMatch match;

	primaryPosition = binarySearch (index, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
 	
 	if (primaryPosition != -1) { 
 		fseek (dataFile, index[primaryPosition].offset, SEEK_SET);
 		fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
 				match.primaryKey, match.blueTeam, match.redTeam, match.date,
 				match.matchDuration, match.winnerTeam, match.blueTeamScore,
 				match.redTeamScore, match.mvpNickname);

 		printf("%s\n", match.primaryKey);
 		printf("%s\n", match.blueTeam);
 		printf("%s\n", match.redTeam);
 		printf("%s\n", match.date);
 		printf("%s\n", match.matchDuration);
 		printf("%s\n", match.winnerTeam);
 		printf("%s\n", match.blueTeamScore);
 		printf("%s\n", match.redTeamScore);
 		printf("%s\n", match.mvpNickname);
 		printf("\n");
 	} else { 
 		printf("Registro não encontrado!\n");
 	}
}

void searchMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, winnerIndex element, int size) {
	int primaryPosition, winnerPosition;
	lolMatch match;

	winnerPosition = binarySearch (index, &element, 0, size - 1, sizeof(winnerIndex), compareWinner);
 	primaryPosition = binarySearch (primaryArray, &index[0], 0, size - 1, sizeof(primaryIndex), compareKeyWithWinner);

 	if (primaryPosition != -1) { 
 		fseek (dataFile, primaryArray[primaryPosition].offset, SEEK_SET);
 		fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
 				match.primaryKey, match.blueTeam, match.redTeam, match.date,
 				match.matchDuration, match.winnerTeam, match.blueTeamScore,
 				match.redTeamScore, match.mvpNickname);

 		printf("%s\n", match.primaryKey);
 		printf("%s\n", match.blueTeam);
 		printf("%s\n", match.redTeam);
 		printf("%s\n", match.date);
 		printf("%s\n", match.matchDuration);
 		printf("%s\n", match.winnerTeam);
 		printf("%s\n", match.blueTeamScore);
 		printf("%s\n", match.redTeamScore);
 		printf("%s\n", match.mvpNickname);
 		printf("\n");
 	} else { 
 		printf("Registro não encontrado!\n");
 	}	
}

void searchMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, mvpIndex element, int size) {
	
}

void listMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size) { 
	char menuOption;
	printListOptions();
	scanf(" %c", &menuOption);
	switch(menuOption) { 
		case '1':
			printMatchesOrderByPrimaryKey (dataFile, pIndex, size);
			break;
		case '2':
			printMatchesOrderByWinner (dataFile, wIndex,  pIndex, size);
			break;
		case '3':
			printMatchesOrderByMVP (dataFile, mIndex, pIndex, size);
			break;
		default:
			break;
	}
}

void searchMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size) { 
	char menuOption;
	char search[40];
	primaryIndex pElement;
	winnerIndex wElement;
	mvpIndex mElement;

	printListOptions();
	scanf(" %c\n", &menuOption);
	switch(menuOption) { 
		case '1':
			scanf("%[^\n]", search);
			strcpy (pElement.primaryKey, search);
			searchMatchesOrderByPrimaryKey (dataFile, pIndex, pElement, size);
			break;
		case '2':
			scanf("%[^\n]", search);
			strcpy (wElement.winner, search);
			searchMatchesOrderByWinner (dataFile, wIndex, pIndex, wElement, size);
			break;
		case '3':
			scanf("%[^\n]", search);
			strcpy (mElement.primaryKey, search);
			searchMatchesOrderByMVP (dataFile, mIndex, pIndex, mElement, size);
			break;
		default:
			break;
	}
}

void freeSpace (FILE** dataFile, primaryIndex **primaryIndex, winnerIndex **winnerIndex, mvpIndex **mvpIndex) {

}

void updateIndexes (primaryIndex *primaryIndex, winnerIndex *winnerIndex, mvpIndex *mvpIndex) {

}

void printOptions()	{ 
	printf("1. Cadastrar.\n");
	printf("2. Alteração.\n");
	printf("3. Remoção.\n");
	printf("4. Busca.\n");
	printf("5. Listagem.\n");
	printf("6. Liberar Espaço.\n");
	printf("7. Finalizar.\n");
}

void printListOptions() {
	printf("1. por código:\n");
	printf("2. por nome da equipe vencedora:\n");
	printf("3. por apelido do MVP:\n");
}
