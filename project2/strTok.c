#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"


	//function to tokenize the input by the char *tok given
   	char **str_tok(char *input, char *tok){
	//declarining and intializing integer variables, one told hold tokenSize and one to hold position in tokens array
	int tokSize = SIZE;
        int position = 0;
	//allocating space for tokens
        char **tokens = (char**)malloc(tokSize * sizeof(char*));
	//declaring char* token
        char *token;
	//printing error if allocation does not work and exiting program
        if (!tokens) {
                printf("allocation error");
                exit(0);
        }
	//setting token to strtok function which tokenizes the string inputted by the char token
        token = strtok(input, tok);
	//iterates through string while token isn't NULL puts word from string in tokens array.
        while (token != NULL) {
                tokens[position] = token;
		//increments position
                position++;
		//tokens again
                token = strtok(NULL, tok);
        }
	//sets last place in array to NULL
        tokens[position] = NULL;
	//returning char **tokens
        return tokens;
        }


