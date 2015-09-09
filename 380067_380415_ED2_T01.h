#ifndef _380067_380415_ED2_T01_   
#define _380067_380415_ED2_T01_

typedef struct lolMatch {
	char* blueTeam;
	char* redTeam;
	char* winnerTeam;
	char* mvpNickname;
	char date[10];
	char matchDuratio[5];
	char blueTeamScore[2];
	char redTeamScore[2];
	char primaryKey[8];
} lolMatch;

typedef struct primaryIndex {
	char primaryKey[8];
	int offset;
	int flag;
} primaryIndex;

typedef struct winnerIndex {
	char* winner;
	char primaryKey[8];
} winnerIndex;

typedef struct mvpIndex {
	char* mvpNickname;
	char primaryKey[8];
} mvpIndex;

int checkDataFile (void);

int checkPrimaryIndex (void);

int checkWinnerIndex (void);

int checkMVPIndex (void);

void createPrimaryIndex (FILE* dataFile);

void createSecondaryIndexes (FILE* dataFile);

void loadIndexes (primaryIndex **primaryIndex, winnerIndex **winnerIndex, mvpIndex **mvpIndex);

void insertMatch (FILE* dataFile, lolMatch match);

lolMatch searchMatch (FILE* dataFile, char* query, int searchOption);

void removeMatch (FILE* dataFile, char* primaryKey);

void printMatchesOrderByPrimaryKey (FILE* dataFile, primaryIndex **index, int searchOption);

void printMatchesOrderByWinner (FILE* dataFile, winnerIndex **index, int searchOption);

void printMatchesOrderByMVP (FILE* dataFile, mvpIndex **index, int searchOption);

void freeSpace (FILE** dataFile, primaryIndex **primaryIndex, winnerIndex **winnerIndex, mvpIndex **mvpIndex);

void updateIndexes (primaryIndex *primaryIndex, winnerIndex *winnerIndex, mvpIndex *mvpIndex);

#endif