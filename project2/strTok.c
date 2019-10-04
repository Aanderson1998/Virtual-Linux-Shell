#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"

   	char **str_tok(char *input, char *tok){
	int tokSize = SIZE;
        int position = 0;
        char **tokens = (char**)malloc(tokSize * sizeof(char*));
        char *token;
        if (!tokens) {
                printf("allocation error");
                exit(0);
        }
        token = strtok(input, tok);
        while (token != NULL) {
                tokens[position] = token;
                position++;
                token = strtok(NULL, tok);
        }
        tokens[position] = NULL;
        return tokens;
        }


