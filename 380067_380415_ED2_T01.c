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

int compareKeyWithMatch (const void *a, const void *b) {
	const primaryIndex *primary_a = (primaryIndex*) a;
	const lolMatch *match_b = (lolMatch*) b;
	return strcmp (primary_a->primaryKey, match_b->primaryKey);
}

int compareKeyWithWinner (const void *a, const void *b) { 
	const primaryIndex *primary_a = (primaryIndex*) a;
	const winnerIndex *winner_b = (winnerIndex*) b;
	return strcmp (primary_a->primaryKey, winner_b->primaryKey);	
}

int compareKeyWithMVP (const void *a, const void *b) {
	const primaryIndex *primary_a = (primaryIndex*) a;
	const mvpIndex *mvp_b = (mvpIndex*) b;
	return strcmp (primary_a->primaryKey, mvp_b->primaryKey);	
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

int compareInt (const void *a, const void*b) {
	const int *n_a = (int*) a;
	const int *n_b = (int*) b;
	return *n_a - *n_b;
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

/*
	Binary search that returns more than one result
*/
int binarySearchAll (void* array, void* key, int start, int end, int *result, size_t size, int (*compare)(const void*, const void*)) {
	int middle;
	int i = 0;
	void *middleElement;
	void *nextElement;
	void *prevElement;
	if (end < start) { 
		return -1;
	} else {
		middle = start + ((end - start)/2);
		middleElement = array + middle * size;
		if ((*compare)(middleElement, key) < 0) { 
			binarySearchAll (array, key, middle + 1, end, result, size, compare);
		} else if ((*compare)(middleElement, key) > 0) { 
			binarySearchAll (array, key, start, middle - 1, result, size, compare);
		} else {
			result[i] = middle;
			nextElement = middleElement + size;
			prevElement = middleElement - size;
			while ((*compare)(nextElement, key) == 0) {
				i += 1;
				middle += 1;
				nextElement = nextElement + size;
				result[i] = middle; 
			}
			while ((*compare)(prevElement, key) == 0) {
				i += 1;
				middle -= 1;
				prevElement = prevElement - size;
				result[i] = middle;
			}
			return i + 1;
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
		primaryIndexArray[i].isNewElement = 0;
		i++;
	}
	return i;
}

void loadWinnerIndex (FILE *file, winnerIndex *winnerIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%[^@]@\n", winnerIndexArray[i].winner ,winnerIndexArray[i].primaryKey) == 2) {
		winnerIndexArray[i].isNewElement = 0;
		i++;
	}
}

void loadMVPIndex (FILE *file, mvpIndex *mvpIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%[^@]@\n", mvpIndexArray[i].mvpNickname , mvpIndexArray[i].primaryKey) == 2) {
		mvpIndexArray[i].isNewElement = 0;
		i++;
	}
}

void persistFile (FILE* file, char* name) {
	fclose (file);
	file = fopen (name, "r+");
}

void saveIndexFiles (FILE * primaryFile, FILE* winnerFile, FILE* mvpFile, 
						primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size) {
	savePrimaryIndex (primaryFile, primaryArray, size);
	saveWinnerIndex (winnerFile, winnerArray, size);
	saveMVPIndex (mvpFile, mvpArray, size);

	persistFile (primaryFile, "iprimary.idx");
	persistFile (winnerFile, "iwinner.idx");
	persistFile (mvpFile, "imvp.idx");

	setIndexConsistency (primaryFile, 1);
}

void updateIndexFiles (FILE * primaryFile, FILE* winnerFile, FILE* mvpFile, 
						primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size) {
	int i;
	
	fseek (primaryFile, 0, SEEK_END);
	for (i = 0; i < size; i += 1) {
		if (primaryArray[i].isNewElement) {
			fprintf(primaryFile, "%s@%d@\n", primaryArray[i].primaryKey, primaryArray[i].offset);
		}
	}

	fseek (winnerFile, 0, SEEK_END);
	for (i = 0; i < size; i += 1) {
		if (winnerArray[i].isNewElement) {
			fprintf(winnerFile, "%s@%s@\n", winnerArray[i].winner, winnerArray[i].primaryKey);
		}
	}

	fseek (mvpFile, 0, SEEK_END);
	for (i = 0; i < size; i += 1) {
		if (mvpArray[i].isNewElement) {
			fprintf(mvpFile, "%s@%s@\n", mvpArray[i].mvpNickname, mvpArray[i].primaryKey);	
		}
	}

	setIndexConsistency (primaryFile, 1);
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

		primaryIndexArray[i].isNewElement = 0;
		winnerIndexArray[i].isNewElement = 0;
		mvpIndexArray[i].isNewElement = 0;

		primaryIndexArray[i].offset = REG_SIZE * i;
	}

	sortIndexes (primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);

	saveIndexFiles (primaryFile, winnerFile, mvpFile, primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);

	return registerCount;
}

int checkIndexConsistency (FILE* primaryFile) {
	int isOk; 
	fscanf (primaryFile, "%d\n", &isOk);
	return isOk;
}

void setIndexConsistency (FILE* primaryFile, int isOk) {
	fseek (primaryFile, 0, SEEK_SET);
	fprintf(primaryFile, "%d\n", isOk);
	persistFile (primaryFile, "iprimary.idx");
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


void createPrimaryKey (lolMatch *element) {
	element->primaryKey[0] = toupper (element->blueTeam[0]);
	element->primaryKey[1] = toupper (element->redTeam[0]);
	element->primaryKey[2] = toupper (element->mvpNickname[0]);
	element->primaryKey[3] = toupper (element->mvpNickname[1]);
	element->primaryKey[4] = element->date[0];
	element->primaryKey[5] = element->date[1];
	element->primaryKey[6] = element->date[3];
	element->primaryKey[7] = element->date[4];
	element->primaryKey[8] = '\0';
}

void printMatch (lolMatch match) {
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

void scanScore (char *score) {
	char invalidData;
	char digitFormatter[3];
	while (scanf (" %2[-0123456789][^\n]", score) == 0) {
		printf ("Campo inválido! Informe novamente:");
		while (invalidData = getchar() != '\n' && invalidData != EOF);
	}
	if (strlen (score) == 1) {
		strcpy (digitFormatter, "0");
		strcat (digitFormatter, score);
		strcpy (score, digitFormatter);
	}
	while (invalidData = getchar() != '\n' && invalidData != EOF);
}

void scanWinnerTeam (lolMatch *element) {
	int isEqualToBlue, isEqualToRed;
	char winnerTeam[200];
	scanf (" %[^\n]", winnerTeam);

	if (strlen (winnerTeam) > 39) {
		printf("Campo inválido! Informe novamente:");
		scanWinnerTeam (element);
	} else {
		strcpy (element->winnerTeam, winnerTeam);
	}

	isEqualToBlue = strcmp (element->winnerTeam, element->blueTeam);
	isEqualToRed = strcmp (element->winnerTeam, element->redTeam);
	if (isEqualToRed != 0 && isEqualToBlue != 0) {
		printf("Campo inválido! Informe novamente:");
		scanWinnerTeam (element);
	}
}

void scanBlueTeam (lolMatch *element) {
	char blueTeam[200];
	scanf (" %[^\n]", blueTeam);

	if (strlen (blueTeam) > 39) {
		printf("%d", (int)strlen (blueTeam));
		printf("Campo inválido! Informe novamente:");
		scanBlueTeam (element);
	} else {
		strcpy (element->blueTeam, blueTeam);
	}
}

void scanRedTeam (lolMatch *element) {
	char redTeam[200];
	scanf (" %[^\n]", redTeam);

	if (strlen (redTeam) > 39) {
		printf("Campo inválido! Informe novamente:");
		scanRedTeam (element);
	} else {
		strcpy (element->redTeam, redTeam);
	}
}

void scanMVP (lolMatch *element) {
	char nickname[200];
	scanf (" %[^\n]", nickname);

	if (strlen (nickname) > 39) {
		printf("Campo inválido! Informe novamente:");
		scanRedTeam (element);
	} else {
		strcpy (element->mvpNickname, nickname);
	}
}

void scanTeams (lolMatch *element) {
	scanBlueTeam (element);
	scanRedTeam (element);

	if (strcmp (element->blueTeam, element->redTeam) == 0) {
		printf("Campo inválido! Informe novamente:");
		scanTeams (element);
	}
}

void scanDate (lolMatch *element) {
	char date[11];
	int day, month, year;

	scanf (" %[^\n]", date);
	
	if (sscanf (date, "%2d/%2d/%4d", &day, &month, &year) == 3) {
		if (day < 1 || day > 31) {
			printf("Campo inválido! Informe novamente:");
			scanDate (element);
		}
		if (month < 1 || month > 12) {
			printf("Campo inválido! Informe novamente:");
			scanDate (element);	
		}
		if (year < 2011 || year > 2015) {
			printf("Campo inválido! Informe novamente:");
			scanDate (element);	
		}
		strcpy (element->date, date);
	} else {
		printf("Campo inválido! Informe novamente:");
		scanDate (element);
	}
}

void scanMatchDuration (char* element) {
	char duration[100];
	int hours, minutes;

	scanf (" %[^\n]", duration);
	
	if (sscanf (duration, "%2d:%2d", &hours, &minutes) == 2 && strlen (duration) == 5) {
		strcpy (element, duration);
	} else {
		printf("Campo inválido! Inforne novamente:");
		scanMatchDuration (element);
	}
}

void readMatch (lolMatch *element) {
	scanTeams (element);
	scanDate (element);
	scanMatchDuration (element->matchDuration);
	scanWinnerTeam (element);
	scanScore (element->blueTeamScore);
	scanScore (element->redTeamScore);
	scanMVP (element);

	createPrimaryKey (element);
}

int insertMatch (FILE *dataFile, FILE *primaryFile, FILE *winnerFile, FILE *mvpFile,
				primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size, lolMatch element) {
	char reg_match[REG_SIZE + 1];
	int i;

	snprintf (reg_match, 192, "%s@%s@%s@%s@%s@%s@%s@%s@%s@", element.primaryKey, element.blueTeam,
				element.redTeam, element.date, element.matchDuration, element.winnerTeam,
				element.blueTeamScore, element.redTeamScore, element.mvpNickname);
	
	for (i = (int) strlen(reg_match); i < REG_SIZE; i++) {
		reg_match[i] = '#';
	}
	reg_match[REG_SIZE] = '\0';

	fseek (dataFile, size * REG_SIZE, SEEK_SET);
	fprintf (dataFile, "%s", reg_match);

	persistFile (dataFile, "matches.dat");


	strcpy (primaryArray[size].primaryKey, element.primaryKey);
	primaryArray[size].offset = size * REG_SIZE;
	primaryArray[size].isNewElement = 1;

	strcpy (winnerArray[size].primaryKey, element.primaryKey);
	strcpy (winnerArray[size].winner, element.winnerTeam);
	winnerArray[size].isNewElement = 1;

	strcpy (mvpArray[size].primaryKey, element.primaryKey);
	strcpy (mvpArray[size].mvpNickname, element.mvpNickname);
	mvpArray[size].isNewElement = 1;

	size += 1;

	sortIndexes (primaryArray, winnerArray, mvpArray, size);

	return size;
}

int addMatch (FILE *dataFile, FILE *primaryFile, FILE *winnerFile, FILE *mvpFile,
				primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size) {
	lolMatch element;

	readMatch (&element);

	if (binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeyWithMatch) != -1) {
		printf ("ERRO: Já existe um registro com a chave primária: %s.\n", element.primaryKey);
	} else {
		setIndexConsistency (primaryFile, 0);
		size = insertMatch (dataFile, primaryFile, winnerFile, mvpFile,
					primaryArray, winnerArray, mvpArray, size, element);
	}

	return size;
}

void removeMatch (FILE* dataFile, char* primaryKey) {

}

void printSearchMatch (FILE *dataFile, primaryIndex *index, int primaryPosition) {
	lolMatch match;
	fseek (dataFile, index[primaryPosition].offset, SEEK_SET);
	fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
			match.primaryKey, match.blueTeam, match.redTeam, match.date,
			match.matchDuration, match.winnerTeam, match.blueTeamScore,
			match.redTeamScore, match.mvpNickname);

	printMatch (match);
}

void printMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, int size) {
	int i;

	for (i = 0; i < size; i += 1) { 
		printSearchMatch (dataFile, index, i);	
	}
}

void printMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, int size) {
	int i, primaryPosition;
	primaryIndex element;

	for (i = 0; i < size; i += 1) {
		strcpy (element.primaryKey, index[i].primaryKey);
		primaryPosition = binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
		if (primaryPosition != -1) {
			printSearchMatch (dataFile, primaryArray, primaryPosition);
		}
	}
}

void printMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, int size) {
	int i, primaryPosition;
	primaryIndex element;

	for (i = 0; i < size; i += 1) {
		strcpy (element.primaryKey, index[i].primaryKey);
		primaryPosition = binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
		if (primaryPosition != -1) {
			printSearchMatch (dataFile, primaryArray, primaryPosition);
		}
	}
}

void searchMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, primaryIndex element, int size) {
	int primaryPosition;

	primaryPosition = binarySearch (index, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
 	
 	if (primaryPosition != -1) { 
 		printSearchMatch (dataFile, index, primaryPosition);
 	} else { 
 		printf("Registro não encontrado!\n");
 	}
}

void searchMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, winnerIndex element, int size) {
	int primaryPosition, i;
	int *result, resultSize;

	result = malloc (sizeof(int) * size);
	resultSize = binarySearchAll (index, &element, 0, size - 1, result, sizeof(winnerIndex), compareWinner); 

	if (resultSize != -1) {
		qsort (result, resultSize, sizeof(int), compareInt);
		
		for (i = 0; i < resultSize; i++) { 
			primaryPosition = binarySearch (primaryArray, &index[result[i]], 0, size - 1, sizeof(primaryIndex), compareKeyWithWinner);

			if (primaryPosition != -1) {
				printSearchMatch (dataFile, primaryArray, primaryPosition);
			} else {
				printf("Registro não encontrado!\n");
			}
		}
	} else {
		printf("Registro não encontrado!\n");
	}
}

void searchMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, mvpIndex element, int size) {
	int primaryPosition, i;
	int *result, resultSize;

	result = malloc (sizeof(int) * size);
	resultSize = binarySearchAll (index, &element, 0, size - 1, result, sizeof(mvpIndex), compareMVP); 

	if (resultSize != -1) {
		qsort (result, resultSize, sizeof(int), compareInt);
		
		for (i = 0; i < resultSize; i++) { 
			primaryPosition = binarySearch (primaryArray, &index[result[i]], 0, size - 1, sizeof(primaryIndex), compareKeyWithMVP);

			if (primaryPosition != -1) {
				printSearchMatch (dataFile, primaryArray, primaryPosition);
			} else {
				printf("Registro não encontrado!\n");
			}
		}
	} else {
		printf("Registro não encontrado!\n");
	}	
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
			strcpy (mElement.mvpNickname, search);
			searchMatchesOrderByMVP (dataFile, mIndex, pIndex, mElement, size);
			break;
		default:
			break;
	}
}

void sortIndexes (primaryIndex *primaryArray, winnerIndex *winnerArray, mvpIndex *mvpArray, int registerCount) {

	qsort (primaryArray, registerCount, sizeof(primaryIndex), compareKeys);

	qsort (winnerArray, registerCount, sizeof(winnerIndex), compareWinnerKeys);
	qsort (winnerArray, registerCount, sizeof(winnerIndex), compareWinner);

	qsort (mvpArray, registerCount, sizeof(mvpIndex), compareMVPKeys);
	qsort (mvpArray, registerCount, sizeof(mvpIndex), compareMVP);
}

void updateMatch (FILE* dataFile, FILE* primaryFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size) { 
	char search[9], matchDuration[6];
	int primaryPosition, winnerPosition, mvpPosition;
	primaryIndex element;
	lolMatch match;

	scanf(" %[^\n]", search);
	strcpy (element.primaryKey, search);

	primaryPosition = binarySearch (pIndex, &element, 0, size - 1, sizeof (primaryIndex), compareKeys);
	winnerPosition = binarySearch (wIndex, &element, 0, size - 1, sizeof (winnerIndex), compareKeyWithWinner);
	mvpPosition = binarySearch (mIndex, &element, 0, size - 1, sizeof (mvpIndex), compareKeyWithMVP);

	if (primaryPosition != -1) {
		scanMatchDuration (matchDuration);

		setIndexConsistency (primaryFile, 0);
		
		fseek (dataFile, pIndex[primaryPosition].offset, SEEK_SET);
		fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
			match.primaryKey, match.blueTeam, match.redTeam, match.date,
			match.matchDuration, match.winnerTeam, match.blueTeamScore,
			match.redTeamScore, match.mvpNickname);

		strcpy (match.matchDuration, matchDuration);

		fseek (dataFile, pIndex[primaryPosition].offset, SEEK_SET);
		fprintf (dataFile, "%s@%s@%s@%s@%s@%s@%s@%s@%s@", 
			match.primaryKey, match.blueTeam, match.redTeam, match.date,
			match.matchDuration, match.winnerTeam, match.blueTeamScore,
			match.redTeamScore, match.mvpNickname);

		persistFile (dataFile, "matches.dat");
	} else {
		printf ("Registro não encontrado!");
	}
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
