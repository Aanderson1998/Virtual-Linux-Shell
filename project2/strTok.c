#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"


	//function to tokenize the input by the char *tok given
   	char **str_tok(char *input, char *tok){
	int tokSize = SIZE;
        int position = 0;
	//allocating space for tokens
        char **tokens = (char**)malloc(tokSize * sizeof(char*));
        char *token;
	//printing error if allocation does not work and exiting program
        if (!tokens) {
                printf("allocation error");
                exit(0);
        }
	//tokening input by the char tok that was passed through to function
        token = strtok(input, tok);
        while (token != NULL) {
                tokens[position] = token;
                position++;
                token = strtok(NULL, tok);
        }
        tokens[position] = NULL;
	//returning char **tokens
        return tokens;
        }


