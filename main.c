/*
	Authors:	Rodrigo Nascimento de Carvalho 380067
				Philippe César Ramos 380415
	Description: 	This is a League of Legends Championship matches database, built using files and indexes. 
					This is the code for the main application using the file and indexes manipulation library.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "380067_380415_ED2_T01.h"

int main (int argc, char *argv[]) {
	int runningProgram = 1, registerCount, matchFileExists = 1;
	char menuOption;
	FILE *matchFile, *primaryIndexFile, *winnerIndexFile, *mvpIndexFile;
	primaryIndex *primaryIndexArray;
	winnerIndex *winnerIndexArray;
	mvpIndex *mvpIndexArray;

	primaryIndexArray = malloc (sizeof (primaryIndex) * MAX_SIZE);
	winnerIndexArray = malloc (sizeof (winnerIndex) * MAX_SIZE);
	mvpIndexArray = malloc (sizeof (mvpIndex) * MAX_SIZE);

	/*
		Check if matches.dat exists and creates it if necessary
	*/
	if ((matchFile = fileExists(matchFile, "matches.dat")) == NULL) {
		matchFile = createFile(matchFile, "matches.dat");
		matchFileExists = 0;
	}

	primaryIndexFile = fileExists(primaryIndexFile, "iprimary.idx");
	winnerIndexFile = fileExists(winnerIndexFile, "iwinner.idx");
	mvpIndexFile = fileExists(mvpIndexFile, "imvp.idx");

	if (primaryIndexFile && winnerIndexFile && mvpIndexFile && matchFileExists ) {
		if (checkIndexConsistency(primaryIndexFile)) {
			registerCount = loadIndexes (primaryIndexFile, primaryIndexArray, 
							winnerIndexFile, winnerIndexArray, mvpIndexFile, mvpIndexArray);
		} else {
			primaryIndexFile = createFile(primaryIndexFile, "iprimary.idx");
			winnerIndexFile = createFile(winnerIndexFile, "iwinner.idx");
			mvpIndexFile = createFile(mvpIndexFile, "imvp.idx");
			registerCount = createIndexes (matchFile, primaryIndexFile, primaryIndexArray, 
							winnerIndexFile, winnerIndexArray, mvpIndexFile, mvpIndexArray);
		}
	} else {
		primaryIndexFile = createFile(primaryIndexFile, "iprimary.idx");
		winnerIndexFile = createFile(winnerIndexFile, "iwinner.idx");
		mvpIndexFile = createFile(mvpIndexFile, "imvp.idx");
		registerCount = createIndexes (matchFile, primaryIndexFile, primaryIndexArray, 
						winnerIndexFile, winnerIndexArray, mvpIndexFile, mvpIndexArray);
	}

	while (runningProgram) {
		/*printOptions();*/
		scanf(" %c", &menuOption);

		switch(menuOption) {
			case '1':
				registerCount = addMatch (matchFile, primaryIndexFile, winnerIndexFile, mvpIndexFile,
							primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);
				break;
			case '2':
				updateMatch (matchFile, primaryIndexFile, primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);
				break;
			case '3':
				removeMatch (matchFile, primaryIndexFile, primaryIndexArray, registerCount);
				break;
			case '4':
				searchMatches (matchFile, primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);
				break;
			case '5':
				listMatches (matchFile, primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);
				break;
			case '6':
				registerCount = freeSpace (matchFile, primaryIndexFile, winnerIndexFile, mvpIndexFile,
							primaryIndexArray, winnerIndexArray, mvpIndexArray, registerCount);
				break;
			case '7':
				runningProgram = 0;

				saveIndexFiles (primaryIndexFile, winnerIndexFile, mvpIndexFile, primaryIndexArray, 
									winnerIndexArray, mvpIndexArray, registerCount);

				free(primaryIndexArray);
				free(winnerIndexArray);
				free(mvpIndexArray);
				fclose(matchFile);
				fclose(primaryIndexFile);
				fclose(mvpIndexFile);
				fclose(winnerIndexFile);
				break;
			default:
				break;
		}
	}

	return 0;
}