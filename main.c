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
	int runningProgram = 1;
	char menuOption;

	while (runningProgram) {
		printf("1. Cadastrar\n");
		printf("2. Alteração\n");
		printf("3. Remoção\n");
		printf("4. Busca\n");
		printf("5. Listagem\n");
		printf("6. Liberar Espaço\n");
		printf("7. Finalizar\n");

		scanf(" %c", &menuOption);

		switch(menuOption) {
			case '1':
				/*TODO Insert*/
				break;
			case '2':
				/*TODO Update*/
				break;
			case '3':
				/*TODO Remove*/
				break;
			case '4':
				/*TODO Search*/
				break;
			case '5':
				/*TODO List*/
				break;
			case '6':
				/*TODO Free Space*/
				break;
			case '7':
				/*TODO Finish*/
				runningProgram = 0;
				break;
			default:
				break;
		}
	}

	return 0;
}