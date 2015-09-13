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

int compareWinnerKeys (const void *a, const void *b);

int compareWinner (const void *a, const void *b);

int compareMVPKeys (const void *a, const void *b);

int compareMVP (const void *a, const void *b);

void savePrimaryIndex (FILE *file, primaryIndex *primaryIndex, int size);

void saveWinnerIndex (FILE *file, winnerIndex *winnerIndexArray, int size);

void saveMVPIndex (FILE *file, mvpIndex *mvpIndexArray, int size);

int createIndexes (FILE* dataFile, FILE* primaryFile, primaryIndex *primaryIndexArray, 
					FILE* winnerFile, winnerIndex *winnerIndexArray, 
					FILE* mvpFile, mvpIndex *mvpIndexArray);

int checkIndexConsistency (FILE* primaryFile);

void setIndexConsistency (FILE* primaryFile, int isOk);

int loadIndexes (FILE* primaryFile, primaryIndex *primaryIndexArray, 
						FILE* winnerFile, winnerIndex *winnerIndexArray, 
						FILE* mvpFile, mvpIndex *mvpIndexArray);

int binarySearch (void* array, void* key, int start, int end, size_t size, int (*compare)(const void*, const void*));

lolMatch searchMatch (FILE* dataFile, char* query, int searchOption);

void removeMatch (FILE* dataFile, char* primaryKey);

void printSearchMatch (FILE *dataFile, primaryIndex *index, int primaryPosition);

void printMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, int size);

void printMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, int size);

void printMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, int size);

void searchMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex *index, primaryIndex element, int size);

void searchMatchesOrderByWinner (FILE* dataFile, winnerIndex *index, primaryIndex *primaryArray, winnerIndex element, int size);

void searchMatchesOrderByMVP (FILE* dataFile, mvpIndex *index, primaryIndex *primaryArray, mvpIndex element, int size);

void listMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size);

void searchMatches (FILE* dataFile, primaryIndex *pIndex, winnerIndex *wIndex, mvpIndex *mIndex, int size);

void freeSpace (FILE** dataFile, primaryIndex **primaryIndex, winnerIndex **winnerIndex, mvpIndex **mvpIndex);

void sortIndexes (primaryIndex *primaryArray, winnerIndex *winnerArray, mvpIndex *mvpArray, int registerCount);

void saveIndexFiles (FILE * primaryFile, FILE* winnerFile, FILE* mvpFile, 
						primaryIndex* primaryArray, winnerIndex* winnerArray, mvpIndex* mvpArray, int size);

void printOptions(void);

void printListOptions(void);

#endif