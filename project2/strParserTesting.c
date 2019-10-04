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
	int background=0;
	int input=0;
	int outputA=0;
	int outputO=0;
	int inFileLoc=0;
	int outFileLoc=0;
	int i=0;
	int j=0;
	while(args[i]!=NULL){
	if(strcmp(args[i],"<")==0){
	input=1;
	inFileLoc=i;
	}
	if(strcmp(args[i],">")==0) {
	outputO=0;
	outFileLoc=i+1;
	}
	if(strcmp(args[i],">>")==0){
        outputA=1;
	outFileLoc=i+1;
	}
	if(strcmp(args[i],"&")==0){
        background=1;
	}
	i++;
	j++;
	}
	simple_command com={args[0],args,input,outputO,outputA,background,j,inFileLoc,outFileLoc};
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
        printf("\ninput redirection: %d\n",com.inputRedirection);
        printf("\noutput overwriting redirection: %d\n",com.outputORedirection);
	printf("\noutput appending redirection: %d\n",com.outputARedirection);
        printf("\nBackground execution: %d\n", com.backgroundEx);
	printf("\nNumer of elements in command: %d\n",com.size);
	printf("\nInput file location is at index: %d\n",com.inFileLoc);
	printf("\nOutput file location is at index %d\n",com.outFileLoc);
        }

