#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<dirent.h>
#include "shell.h"





  	int externalCommands(struct simple_command com){
  	if(com.word==NULL){
    		printf("No command entered");
    		return 1;
  		}
  	pid_t pid = fork();
  	if (pid < 0) {
   		printf("\nFailed forking child..");
    		return 0;
  	} else if (pid == 0) {
      		if (execvp(com.word, com.arguments) < 0) {
        	printf("\nCould not execute command..");
        	}
    		exit(0);
    	} else{
    		wait(NULL);
    	}
	return 1;
	}



	int execute(struct simple_command com){
  	int i;
  	if (com.word == NULL) {
    		return 1;
  	}
  	for (i = 0; i < numInternalCommands; i++) {
    		if (strcmp(com.word, intCommands[i]) == 0) {
      		return internalCommands(com);
    		}
  	}
  	return externalCommands(com);
	}




	int main(int argc, char **argv){
  	char *input;
  	char **args;
  	int again;
	struct simple_command com;
  	clr();
  	printf("\n\n\n\t*******************welcome to my shell**************************");
  	char* username = getenv("USER");
  	printf("\n\n\nUSER is: @%s\n", username);
  	printf("enter in commands and begin using the shelll\n");
  	sleep(2);
  	do {

		input = getInput();
        	args = str_tok(input);
        	com=strParse(args);
    		again = execute(com);
		free(input);
    		free(args);
  	} while (again);
	}



