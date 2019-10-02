#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 1000



    char **str_tok(char *str){
    char input[SIZE];
    int i=0;
    while(*str!='\0'){
	input[i]=*str;
        i++;
	str++;
    }
	printf("The input is: ");
	printf("%s\n",input);
	int containsInput=0;
	int containsAppend =0;
	int containsOverwrite=0;
	int containsPipe=0;
	int containsBackgroundEx=0;
	char *append=">>";
	char *overwrite =">";
	char *inputFile="<";
	char *backgroundEx="&";
	char *pipes="|";
 	int tokSize = SIZE;
  	int position = 0;
  	//allocate space in memory for tokenized string, array of separate strings in long string
  	char *tokens = (char*)malloc(tokSize * sizeof(char*));
  	if (!tokens) {
    	printf("allocation error");
  	}

	printf("\nThe tokenized input is:\n");
  	char *token = strtok(input, " ");
  	while (token != NULL) {
  	        printf("%s\n", token);
		tokens[position] = token;
    		position++;
    		if(strcmp(token,append)==0){
			containsAppend=1;
		}
		if(strcmp(token,overwrite)==0){
			containsOverwrite=1;
		}
		if(strcmp(token,inputFile)==0){
			containsInput=1;
		}
		if(strcmp(token,backgroundEx)==0){
			containsBackgroundEx=1;
		}
		if(strcmp(token,pipes)==0){
			containsPipe=1;
		}
    		position++;
    		token = strtok(NULL, " ");
  	}
	tokens[position]=NULL;
	printf("contains append output redirection: %d\n",containsAppend);
	printf("contains overwrite output redirection: %d\n",containsOverwrite);
	printf("contains input redirection: %d\n",containsInput);
	printf("contains background execution: %d\n",containsBackgroundEx);
	printf("contains piping: %d\n",containsPipe);
	return tokens;
	}



	int main(void){
        char *str="ls –la >> output.tx";
        char **strSplit=str_tok(str);
	}
