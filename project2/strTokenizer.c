#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 1000


	char **str_tok(char *input){
 	int tokSize = SIZE;
  	int position = 0;
  	//allocate space in memory for tokenized string, array of separate strings in long string
  	char **tokens = (char**)malloc(tokSize * sizeof(char*));
  	char *token;
  	if (!tokens) {
    		printf("allocation error");
    		exit(0);
  	}
  	token = strtok(input, " ");
  	while (token != NULL) {
    		tokens[position] = token;
    		position++;
    		token = strtok(NULL, " ");
  	}
  	tokens[position] = NULL;
  	return tokens;
	}


	int main(void){
	char *str="Hi how are you doing today\n";
	char** strsplit=str_token(str);
	int position=0;
	while (strsplit[position]!=NULL){
		printf("%s\n",strsplit[position]);
		position++;
		}
	}
