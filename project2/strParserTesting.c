#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"





	char *getInput(){
        char* str;
        str = readline(">>> ");
        if (strlen(str) != 0) {
                add_history(str);
        }
        return str;
        }




    	char **str_tok(char *input){
        int tokSize = SIZE;
        int position = 0;
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





	struct simple_command strParse(char **args){
	int pipe=0;
	int background=0;
	int input=0;
	int output=0;
	int i=0;
	while(args[i]!=NULL){
	if(strcmp(args[i],"|")==0){
	pipe=1;
	}
	if(strcmp(args[i],"<")==0){
	input=1;
	}
	if(strcmp(args[i],">")==0 || strcmp(args[i],">>")==0){
        output=1;
	}
	if(strcmp(args[i],"&")==0){
        background=1;
	}
	i++;
	}
	simple_command com={args[0],args,input,output,pipe,background};
	char **comArgs=com.arguments;
	int k=0;
	return com;
	}



	int main(int argc, char **argv){
        char *input;
        char **args;
    	simple_command com;
        printf("enter in command to be parsed\n");
        sleep(2);
        input = getInput();
       	args = str_tok(input);
       	com=strParse(args);
        int k=0;
        printf("\n\n\ncommand structure elements are:\n");
        printf("\nmain command: %s\n",com.word);
        printf("\nparsed string is:\n");
        while (com.arguments[k]!=NULL){
        printf("%s\n",com.arguments[k]);
        k++;
        }
        printf("\npiping: %d\n",com.piping);
        printf("\ninput redirection: %d\n",com.inputRedirection);
        printf("\noutput redirection: %d\n",com.outputRedirection);
        printf("\nBackground execution: %d\n", com.backgroundEx);
        }

