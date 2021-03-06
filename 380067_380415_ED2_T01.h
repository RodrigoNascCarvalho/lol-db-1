/*
	Authors:	Rodrigo Nascimento de Carvalho 380067
				Philippe César Ramos 380415
	Description: 	This is a League of Legends Championship matches database, built using files and indexes. 
					This is the library prototype definition used to manipulate the indexes and files for this application.
*/


#ifndef _380067_380415_ED2_T01_   
#define _380067_380415_ED2_T01_

#define REG_SIZE 192
#define MAX_SIZE 1000

typedef struct lolMatch {
	char blueTeam[40];
	char redTeam[40];
	char winnerTeam[40];
	char mvpNickname[40];
	char date[11];
	char matchDuration[6];
	char blueTeamScore[3];
	char redTeamScore[3];
	char primaryKey[9];
} lolMatch;

typedef struct primaryIndex {
	char primaryKey[9];
	int offset;
} primaryIndex;

typedef struct winnerIndex {
	char winner[40];
	char primaryKey[9];
} winnerIndex;

typedef struct mvpIndex {
	char mvpNickname[40];
	char primaryKey[9];
} mvpIndex;

FILE* fileExists (FILE *file, char* name);

FILE* createFile (FILE *file, char* name);

int compareKeys (const void *a, const void *b);

int compareKeyWithMatch (const void *a, const void *b);

int compareKeyWithWinner (const void *a, const void *b);

int compareKeyWithMVP (const void *a, const void *b);

int compareWinnerKeys (const void *a, const void *b);

int compareWinner (const void *a, const void *b);

int compareMVPKeys (const void *a, const void *b);

int compareMVP (const void *a, const void *b);

int compareInt (const void *a, const void*b);

int binarySearch (void* array, void* key, int start, int end, size_t size, int (*compare)(const void*, const void*));

int binarySearchAll (void* array, void* key, int start, int end, int *result, size_t size, int (*compare)(const void*, const void*));

void savePrimaryIndex (FILE *file, primaryIndex *primaryIndex, int size);

void saveWinnerIndex (FILE *file, winnerIndex *winnerIndexArray, int size);

void saveMVPIndex (FILE *file, mvpIndex *mvpIndexArray, int size);

int loadPrimaryIndex (FILE *file, primaryIndex *primaryIndexArray);

void loadWinnerIndex (FILE *file, winnerIndex *winnerIndexArray);

void loadMVPIndex (FILE *file, mvpIndex *mvpIndexArray);

void persistFile (FILE* file, char* name);

void saveIndexFiles (FILE * primaryFile, FILE* winnerFile, FILE* mvpFile, 
						primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size);

void updateIndexFiles (FILE * primaryFile, FILE* winnerFile, FILE* mvpFile, 
						primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size);

int createIndexes (FILE* dataFile, FILE* primaryFile, primaryIndex *primaryIndexArray, 
					FILE* winnerFile, winnerIndex *winnerIndexArray, 
					FILE* mvpFile, mvpIndex *mvpIndexArray);

int checkIndexConsistency (FILE* primaryFile);

void setIndexConsistency (FILE* primaryFile, int isOk);

int loadIndexes (FILE* primaryFile, primaryIndex *primaryIndexArray, 
						FILE* winnerFile, winnerIndex *winnerIndexArray, 
						FILE* mvpFile, mvpIndex *mvpIndexArray);

void createPrimaryKey (lolMatch *element);

void printMatch (lolMatch match);

void scanScore (char *score);

void scanWinnerTeam (lolMatch *element);

void scanBlueTeam (lolMatch *element);

void scanRedTeam (lolMatch *element);

void scanMVP (lolMatch *element);

void scanTeams (lolMatch *element);

void scanDate (lolMatch *element);

void scanMatchDuration (char* element);

void readMatch (lolMatch *element);

int insertMatch (FILE *dataFile, FILE *primaryFile, FILE *winnerFile, FILE *mvpFile,
				primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size, lolMatch element);

int addMatch (FILE *dataFile, FILE *primaryFile, FILE *winnerFile, FILE *mvpFile,
				primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size);

void removeMatch (FILE* dataFile, FILE* primaryFile, primaryIndex *primaryArray, winnerIndex *winnerArray, mvpIndex *mvpArray, int size);

void printSearchMatch (FILE *dataFile, primaryIndex *index, int primaryPosition);

void printMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, int size);

void printMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, int size);

void printMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, int size);

void searchMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, primaryIndex element, int size);

void searchMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, winnerIndex element, int size);

void searchMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, mvpIndex element, int size);

void listMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size);

void searchMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size);

void sortIndexes (primaryIndex *primaryArray, winnerIndex *winnerArray, mvpIndex *mvpArray, int registerCount);

void printOptions(void);

void printListOptions(void);

int freeSpace (FILE *dataFile, FILE *primaryFile, FILE *winnerFile, FILE *mvpFile,
				primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size);

#endif