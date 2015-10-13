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


/*
	A simple function to check if a file exists, if the file exists then it returns the file in r+ mode.
*/
FILE* fileExists (FILE *file, char* name) {
	if ((file = fopen (name, "r+")) != NULL) {
		return file;
	} else {
		return NULL;
	}
}

/*
	A simple function to create a file and return it, it creates the file in w+ mode.
*/
FILE* createFile (FILE *file, char* name) {
	file = fopen (name, "w+");
	return file;
}

/*
	From this point on, all of the comparators are defined, comparators are used in the binarySearch generic functions.
	Their purpose is to avoid rewriting the binary search functions.
	The comparators are also used for the quick sort calls.
*/

/*
	Compares primary keys from the primaryIndex.
*/
int compareKeys (const void *a, const void *b) {
	const primaryIndex *primary_a = (primaryIndex*) a;
	const primaryIndex *primary_b = (primaryIndex*) b;
	return strcmp (primary_a->primaryKey, primary_b->primaryKey);
}

/*
	Compares a primaryIndex key with a match structure key.
*/
int compareKeyWithMatch (const void *a, const void *b) {
	const primaryIndex *primary_a = (primaryIndex*) a;
	const lolMatch *match_b = (lolMatch*) b;
	return strcmp (primary_a->primaryKey, match_b->primaryKey);
}

/*
	Compares a primaryIndex key with a winnerIndex key.
*/
int compareKeyWithWinner (const void *a, const void *b) { 
	const primaryIndex *primary_a = (primaryIndex*) a;
	const winnerIndex *winner_b = (winnerIndex*) b;
	return strcmp (primary_a->primaryKey, winner_b->primaryKey);	
}

/*
	Compares a primaryIndex key with a MVPIndex key.
*/
int compareKeyWithMVP (const void *a, const void *b) {
	const primaryIndex *primary_a = (primaryIndex*) a;
	const mvpIndex *mvp_b = (mvpIndex*) b;
	return strcmp (primary_a->primaryKey, mvp_b->primaryKey);	
}

/*
	Compares just winnerIndex keys.
*/
int compareWinnerKeys (const void *a, const void *b) {
	const winnerIndex *winner_a = (winnerIndex*) a;
	const winnerIndex *winner_b = (winnerIndex*) b;
	return strcmp (winner_a->primaryKey, winner_b->primaryKey);
}

/*
	Compares just winnerIndex winners.
*/
int compareWinner (const void *a, const void *b) {
	const winnerIndex *winner_a = (winnerIndex*) a;
	const winnerIndex *winner_b = (winnerIndex*) b;
	return strcasecmp (winner_a->winner, winner_b->winner);
}

/*
	Compares just MVPIndex primary keys.
*/
int compareMVPKeys (const void *a, const void *b) {
	const mvpIndex *mvp_a = (mvpIndex*) a;
	const mvpIndex *mvp_b = (mvpIndex*) b;
	return strcmp (mvp_a->primaryKey, mvp_b->primaryKey);
}

/*
	Compares just MVPIndex mvpNicknames.
*/
int compareMVP (const void *a, const void *b) {
	const mvpIndex *mvp_a = (mvpIndex*) a;
	const mvpIndex *mvp_b = (mvpIndex*) b;
	return strcasecmp (mvp_a->mvpNickname, mvp_b->mvpNickname);
}

/*
	Compares ints.
*/
int compareInt (const void *a, const void*b) {
	const int *n_a = (int*) a;
	const int *n_b = (int*) b;
	return *n_a - *n_b;
}

/*
	Generic binary search function, based on the prototype of the qsort stantard C function. 
	Instead of dereferencing the pointer, the values are always found by iterating on the pointer's address by adding
	the size of the structure sent to the function, because void pointers cannot be dereferenced.
	The comparison between key and array always needs to be defined as a separate function and be sent to the binary
	search function, this allows us to avoid recreating this function if we want to make comparisons between the 
	different structures we have in this project.
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
			return binarySearch (array, key, middle + 1, end, size, compare);
		} else if ((*compare)(middleElement, key) > 0) { 
			return binarySearch (array, key, start, middle - 1, size, compare);
		} else { 
			return middle;
		}
	}
}

/*
	Generic binary search function that returns all key ocurrences.
	After finding the key, it searches the other key ocurrences to the right and left of the array.
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
			return binarySearchAll (array, key, middle + 1, end, result, size, compare);
		} else if ((*compare)(middleElement, key) > 0) { 
			return binarySearchAll (array, key, start, middle - 1, result, size, compare);
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

/*
	A function to save the primary Index into a file.
*/
void savePrimaryIndex (FILE *file, primaryIndex *primaryIndexArray, int size) {
	int i;

	fclose (file);
	file = fopen ("iprimary.idx", "w+");

	fprintf(file, "%d\n", 1);
	for (i = 0; i < size; i += 1) {
		fprintf(file, "%s@%d@\n", primaryIndexArray[i].primaryKey, primaryIndexArray[i].offset);
	}
}

/*
	A function save the winner Index into a file.
*/
void saveWinnerIndex (FILE *file, winnerIndex *winnerIndexArray, int size) {
	int i;

	fclose (file);
	file = fopen ("iwinner.idx", "w+");

	for (i = 0; i < size; i += 1) {
		fprintf(file, "%s@%s@\n", winnerIndexArray[i].winner ,winnerIndexArray[i].primaryKey);
	}
}

/*
	A function to save the MVP Index into a file.
*/
void saveMVPIndex (FILE *file, mvpIndex *mvpIndexArray, int size) {
	int i;

	fclose (file);
	file = fopen ("imvp.idx", "w+");

	for (i = 0; i < size; i += 1) {
		fprintf(file, "%s@%s@\n", mvpIndexArray[i].mvpNickname , mvpIndexArray[i].primaryKey);
	}
}

/*
	A function to load the primary Index into the memory.
*/
int loadPrimaryIndex (FILE *file, primaryIndex *primaryIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%d@\n", primaryIndexArray[i].primaryKey, &primaryIndexArray[i].offset) == 2) {
		i++;
	}
	return i;
}

/*
	A function to load the winner Index into the memory.
*/
void loadWinnerIndex (FILE *file, winnerIndex *winnerIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%[^@]@\n", winnerIndexArray[i].winner ,winnerIndexArray[i].primaryKey) == 2) {
		i++;
	}
}

/*
	A function to load the MVP Index into the memory.
*/
void loadMVPIndex (FILE *file, mvpIndex *mvpIndexArray) {
	int i = 0;
	while (fscanf (file, "%[^@]@%[^@]@\n", mvpIndexArray[i].mvpNickname , mvpIndexArray[i].primaryKey) == 2) {
		i++;
	}
}

/*
	Sometimes we need to guarantee the files are persisted into the file system.
	This function exists to fulfill that purpose.
*/
void persistFile (FILE* file, char* name) {
	fclose (file);
	file = fopen (name, "r+");
}

/*
	This is a function that saves all of the indexes and persists them on the file system.
	By the end of this process, the index consistency is correct.
*/
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

/*
	This is a function to create all indexes.
	Seeks each register using its REG_SIZE and position in the file,
	scans the relevant values from the file,
	creates their index entry for all indexes,
	sorts the indexes and save them.
	It also return the registerCount that will be used by the program as a whole.
*/
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

		if (strstr (primaryIndexArray[i].primaryKey, "*|") == NULL) {
			primaryIndexArray[i].offset = REG_SIZE * i;
		} else {
			primaryIndexArray[i].offset = -1;
		}
	}

	sortIndexes (primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);

	saveIndexFiles (primaryFile, winnerFile, mvpFile, primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);

	return registerCount;
}

/*
	A function to check the indexes consistency.
*/
int checkIndexConsistency (FILE* primaryFile) {
	int isOk; 
	fscanf (primaryFile, "%d\n", &isOk);
	return isOk;
}

/*
	A function to set the index consistency to a given value.
*/
void setIndexConsistency (FILE* primaryFile, int isOk) {
	fseek (primaryFile, 0, SEEK_SET);
	fprintf(primaryFile, "%d\n", isOk);
	persistFile (primaryFile, "iprimary.idx");
}

/*
	Load indexes from the file system into the memory.
	Getting the register count is necessary because the program uses it.
*/
int loadIndexes (FILE* primaryFile, primaryIndex *primaryIndexArray, 
						FILE* winnerFile, winnerIndex *winnerIndexArray, 
						FILE* mvpFile, mvpIndex *mvpIndexArray) {
	int registerCount;
	registerCount = loadPrimaryIndex (primaryFile, primaryIndexArray);
	loadWinnerIndex (winnerFile, winnerIndexArray);
	loadMVPIndex (mvpFile, mvpIndexArray);
	return registerCount;
}

/*
	A simple function to create the primary key as specified.
	A primary key is composed of:
		- The first letter of the blue team
		- The first letter of the red team
		- The two first letters of the MVP's nickname
		- The day and month of the match.
*/
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

/*
	A simple function to print a registered match.
*/
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

/*
	From this point on, there are some functions defined to grab user input. 
*/

/*
	A function to scan the score value, it checks if the input is a number. And if the size of the input was 2.
	If the score doesn't follow the format, it asks for the same input again. 
*/
void scanScore (char *score) {
	char buffer[10];

	scanf (" %[^\n]", buffer);

	if (sscanf (buffer, "%[-0123456789]", score) != 1 || strlen (buffer) != 2) {
		printf ("Campo inválido! Informe novamente: ");
		scanScore (score);
	}
}

/*
	A function to scan the winner team value, it checks if the input is smaller than 39 characters. 
	And if the winner team is the blue or red team, it needs to be one of them. 
	It asks for the same input again in case of error. 
*/
void scanWinnerTeam (lolMatch *element) {
	int isEqualToBlue, isEqualToRed;
	char winnerTeam[200];
	scanf (" %[^\n]", winnerTeam);

	if (strlen (winnerTeam) > 39) {
		printf("Campo inválido! Informe novamente: ");
		scanWinnerTeam (element);
	} else {
		strcpy (element->winnerTeam, winnerTeam);
	}

	isEqualToBlue = strcmp (element->winnerTeam, element->blueTeam);
	isEqualToRed = strcmp (element->winnerTeam, element->redTeam);
	if (isEqualToRed != 0 && isEqualToBlue != 0) {
		printf("Campo inválido! Informe novamente: ");
		scanWinnerTeam (element);
	}
}

/*
	A function to scan the blue team value, it checks if the input is smaller than 39 characters.
	It asks for the same input again in case of error.
*/
void scanBlueTeam (lolMatch *element) {
	char blueTeam[200];
	scanf (" %[^\n]", blueTeam);

	if (strlen (blueTeam) > 39) {
		printf("%d", (int)strlen (blueTeam));
		printf("Campo inválido! Informe novamente: ");
		scanBlueTeam (element);
	} else {
		strcpy (element->blueTeam, blueTeam);
	}
}

/*
	A function to scan the red team value, it checks if the input is smaller than 39 characters.
	It asks for the same input again in case of error.
*/
void scanRedTeam (lolMatch *element) {
	char redTeam[200];
	scanf (" %[^\n]", redTeam);

	if (strlen (redTeam) > 39) {
		printf("Campo inválido! Informe novamente: ");
		scanRedTeam (element);
	} else {
		strcpy (element->redTeam, redTeam);
	}
}

/*
	A function to scan the MVP value, it checks if the input is smaller than 39 characters.
	It asks for the same input again in case of error.
*/
void scanMVP (lolMatch *element) {
	char nickname[200];
	scanf (" %[^\n]", nickname);

	if (strlen (nickname) > 39) {
		printf("Campo inválido! Informe novamente: ");
		scanRedTeam (element);
	} else {
		strcpy (element->mvpNickname, nickname);
	}
}

/*
	A function to scan both teams. 
	It verifies if the teams are different, in case they are equal, the input should be restarted.
*/
void scanTeams (lolMatch *element) {
	scanBlueTeam (element);
	scanRedTeam (element);

	if (strcmp (element->blueTeam, element->redTeam) == 0) {
		printf("Campo inválido! Informe novamente: ");
		scanTeams (element);
	}
}

/*
	A function to scan the date.
	Checks if the date format is correct.
	It asks for the same input again in case of error.	
*/
void scanDate (lolMatch *element) {
	char date[11];
	int day, month, year;

	scanf (" %[^\n]", date);
	
	if (sscanf (date, "%2d/%2d/%4d", &day, &month, &year) == 3) {
		if (day < 1 || day > 31) {
			printf("Campo inválido! Informe novamente: ");
			scanDate (element);
		} else	if (month < 1 || month > 12) {
			printf("Campo inválido! Informe novamente: ");
			scanDate (element);	
		} else if (year < 2011 || year > 2015) {
			printf("Campo inválido! Informe novamente: ");
			scanDate (element);	
		} else {
			strcpy (element->date, date);	
		}
	} else {
		printf("Campo inválido! Informe novamente: ");
		scanDate (element);
	}
}

/*
	A function to scan the date.
	Checks if the match duration format is correct.
	It asks for the same input again in case of error.	
*/
void scanMatchDuration (char* element) {
	char duration[100];
	int hours, minutes;

	scanf (" %[^\n]", duration);
	
	if (sscanf (duration, "%2d:%2d", &hours, &minutes) == 2 && strlen (duration) == 5) {
		strcpy (element, duration);
	} else {
		printf("Campo inválido! Informe novamente: ");
		scanMatchDuration (element);
	}
}

/*
	A function to read a match and create the match primary key.
*/
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

/*
	A function to insert a match.
	Prints the register format to the reg_match string, and then fills the remaining space with #.
	Seeks the end of file, and writes the reg_match to the file.
	Persists file and updates all indexes and the size of the DB.
*/
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

	strcpy (winnerArray[size].primaryKey, element.primaryKey);
	strcpy (winnerArray[size].winner, element.winnerTeam);

	strcpy (mvpArray[size].primaryKey, element.primaryKey);
	strcpy (mvpArray[size].mvpNickname, element.mvpNickname);

	size += 1;

	sortIndexes (primaryArray, winnerArray, mvpArray, size);

	return size;
}

/*
	A function to add a match. It reads a match,
	Verifies if the primary key already exists,
	If it doesn't, then the index consistency is set to 0
	and the match is inserted. 
*/
int addMatch (FILE *dataFile, FILE *primaryFile, FILE *winnerFile, FILE *mvpFile,
				primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size) {
	lolMatch element;
	int primaryPosition;

	readMatch (&element);
	primaryPosition = binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeyWithMatch);
	if (primaryPosition != -1 && primaryArray[primaryPosition].offset != -1) {
		printf ("ERRO: Já existe um registro com a chave primária: %s.\n", element.primaryKey);
	} else {
		setIndexConsistency (primaryFile, 0);
		size = insertMatch (dataFile, primaryFile, winnerFile, mvpFile,
					primaryArray, winnerArray, mvpArray, size, element);
	}

	return size;
}

/*
	A function to remove a match. 
	Finds the primary key and marks the registered match as removed.
*/
void removeMatch (FILE* dataFile, FILE* primaryFile, primaryIndex *primaryArray, winnerIndex *winnerArray, mvpIndex *mvpArray, int size) {
	primaryIndex match;
	int primaryPosition;

	scanf (" %[^\n]", match.primaryKey);
	primaryPosition = binarySearch (primaryArray, &match, 0, size - 1, sizeof (primaryIndex), compareKeys);
	if (primaryPosition != -1  && primaryArray[primaryPosition].offset != -1) {
		setIndexConsistency (primaryFile, 0);

		fseek (dataFile, primaryArray[primaryPosition].offset, SEEK_SET);
		fprintf (dataFile, "*|");
			
		persistFile (dataFile, "matches.dat");

		/*
			Guarantees that if the user adds exactly the same register again,
			we don't need to modify the way the program searches for the registers.
		*/
		primaryArray[primaryPosition].primaryKey[0] = '*';
		primaryArray[primaryPosition].primaryKey[1] = '|';
		primaryArray[primaryPosition].offset = -1;

		sortIndexes (primaryArray, winnerArray, mvpArray, size);
	} else {
		printf ("Registro não encontrado!\n");
	}
}

/*
	A function to print a match after scanning it from a file.
*/
void printSearchMatch (FILE *dataFile, primaryIndex *index, int primaryPosition) {
	lolMatch match;
	fseek (dataFile, index[primaryPosition].offset, SEEK_SET);
	fscanf (dataFile, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@", 
			match.primaryKey, match.blueTeam, match.redTeam, match.date,
			match.matchDuration, match.winnerTeam, match.blueTeamScore,
			match.redTeamScore, match.mvpNickname);

	printMatch (match);
}

/*
	A function to print all matches ordered by primary key.
*/
void printMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, int size) {
	int i;

	for (i = 0; i < size; i += 1) {
		if (index[i].offset != -1) {
			printSearchMatch (dataFile, index, i);	
		}
	}
}

/*
	A function to print all matches ordered by winner.
	It is necessary to find the position of the winner ordered register in the primaryIndex.
*/
void printMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, int size) {
	int i, primaryPosition;
	primaryIndex element;

	for (i = 0; i < size; i += 1) {
		strcpy (element.primaryKey, index[i].primaryKey);
		primaryPosition = binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
		if (primaryPosition != -1 && primaryArray[primaryPosition].offset != -1) {
			printSearchMatch (dataFile, primaryArray, primaryPosition);
		}
	}
}

/*
	A function to print all matches ordereb by mvp.
	It is necessary to find the position of the mvp ordered register in the primaryIndex.
*/
void printMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, int size) {
	int i, primaryPosition;
	primaryIndex element;

	for (i = 0; i < size; i += 1) {
		strcpy (element.primaryKey, index[i].primaryKey);
		primaryPosition = binarySearch (primaryArray, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
		if (primaryPosition != -1 && primaryArray[primaryPosition].offset != -1) {
			printSearchMatch (dataFile, primaryArray, primaryPosition);
		}
	}
}

/*
	A function to search a match by primary key.
	It only uses the binary search to grab the correct match.
*/
void searchMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, primaryIndex element, int size) {
	int primaryPosition;

	primaryPosition = binarySearch (index, &element, 0, size - 1, sizeof(primaryIndex), compareKeys);
 	
 	if (primaryPosition != -1 && index[primaryPosition].offset != -1) { 
 		printSearchMatch (dataFile, index, primaryPosition);
 	} else { 
 		printf("Registro não encontrado!\n");
 	}
}

/*
	A function to search matches by winner. At first, it needs to have a result set of winners found.
	And then, it just gets the primary position and prints it using the print search match.
*/
void searchMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, winnerIndex element, int size) {
	int primaryPosition, i;
	int *result, resultSize;

	result = malloc (sizeof(int) * size);
	resultSize = binarySearchAll (index, &element, 0, size - 1, result, sizeof(winnerIndex), compareWinner); 

	if (resultSize != -1) {
		qsort (result, resultSize, sizeof(int), compareInt);
		
		for (i = 0; i < resultSize; i++) { 
			primaryPosition = binarySearch (primaryArray, &index[result[i]], 0, size - 1, sizeof(primaryIndex), compareKeyWithWinner);

			if (primaryPosition != -1 && primaryArray[primaryPosition].offset != -1) {
				printSearchMatch (dataFile, primaryArray, primaryPosition);
			} else {
				printf("Registro não encontrado!\n");
			}
		}
	} else {
		printf("Registro não encontrado!\n");
	}
}

/*
	A function to search matches by MVP. At first, it needs to have a result set of MVPs found.
	And then, it just gets the primary position and prints it using the print search match.
*/
void searchMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, mvpIndex element, int size) {
	int primaryPosition, i;
	int *result, resultSize;

	result = malloc (sizeof(int) * size);
	resultSize = binarySearchAll (index, &element, 0, size - 1, result, sizeof(mvpIndex), compareMVP); 

	if (resultSize != -1) {
		qsort (result, resultSize, sizeof(int), compareInt);
		
		for (i = 0; i < resultSize; i++) { 
			primaryPosition = binarySearch (primaryArray, &index[result[i]], 0, size - 1, sizeof(primaryIndex), compareKeyWithMVP);

			if (primaryPosition != -1 && primaryArray[primaryPosition].offset != -1) {
				printSearchMatch (dataFile, primaryArray, primaryPosition);
			} else {
				printf("Registro não encontrado!\n");
			}
		}
	} else {
		printf("Registro não encontrado!\n");
	}	
}

/*
	A function to manage the options of listing matches.
*/
void listMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size) { 
	char menuOption;
	/*printListOptions();*/
	getchar();
	scanf("%c", &menuOption);
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

/*
	A function to manage the options of searching matches. 
*/
void searchMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size) { 
	char menuOption;
	char search[40];
	primaryIndex pElement;
	winnerIndex wElement;
	mvpIndex mElement;

	/*printListOptions();*/
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

/*
	A function that sorts the indexes.
*/
void sortIndexes (primaryIndex *primaryArray, winnerIndex *winnerArray, mvpIndex *mvpArray, int registerCount) {

	qsort (primaryArray, registerCount, sizeof(primaryIndex), compareKeys);

	qsort (winnerArray, registerCount, sizeof(winnerIndex), compareWinnerKeys);
	qsort (winnerArray, registerCount, sizeof(winnerIndex), compareWinner);

	qsort (mvpArray, registerCount, sizeof(mvpIndex), compareMVPKeys);
	qsort (mvpArray, registerCount, sizeof(mvpIndex), compareMVP);
}

/*
	A function to update a match. Firsst it scans the primary key. 
	Then it verifies if the register exists using the primary Index.
	If it exists, then the consistency is set to 0,
	the register is seeked and updated and the database file is persisted in the disk.
*/
void updateMatch (FILE* dataFile, FILE* primaryFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size) { 
	char search[9], matchDuration[6];
	int primaryPosition;
	primaryIndex element;
	lolMatch match;

	scanf(" %[^\n]", search);
	strcpy (element.primaryKey, search);

	primaryPosition = binarySearch (pIndex, &element, 0, size - 1, sizeof (primaryIndex), compareKeys);

	if (primaryPosition != -1 && pIndex[primaryPosition].offset != -1) {
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
		printf ("Registro não encontrado!\n");
	}
}

/*
	Two functions to print options, currently not in use. 
*/
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

/*
	This function deletes the files that were marked as removed from the database file and all of the indexes.
	It creates a file copy and copies all registers to it but the ones removed.
	By the end the database file is replaced by the file copy, and all indexes are recreated.
*/
int freeSpace (FILE *dataFile, FILE *primaryFile, FILE *winnerFile, FILE *mvpFile,
				primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size) {
	FILE *temporaryFile;
	int i;
	char primaryKey[9];
	char reg_buffer[193];
	if ((temporaryFile = fopen ("temporary.dat", "w+")) != NULL) {
		for (i = 0; i < size; i++) {
			fseek (dataFile, REG_SIZE * i, SEEK_SET);
			fgets (reg_buffer, 193, dataFile);
			sscanf (reg_buffer, "%[^@]", primaryKey);

			if (strstr (primaryKey, "*|") == NULL) {
				fprintf (temporaryFile, "%s", reg_buffer);
			}
		}
	}	
	fclose (dataFile);
	fclose (temporaryFile);
	rename ("temporary.dat", "matches.dat");
	dataFile = fopen ("matches.dat", "r+");


	free(primaryArray);
	free(winnerArray);
	free(mvpArray);

	primaryArray = malloc (sizeof (primaryIndex) * MAX_SIZE);
	winnerArray = malloc (sizeof (winnerIndex) * MAX_SIZE);
	mvpArray = malloc (sizeof (mvpIndex) * MAX_SIZE);

	size = createIndexes (dataFile, primaryFile, primaryArray, winnerFile, winnerArray, mvpFile, mvpArray);
	return size;
}
