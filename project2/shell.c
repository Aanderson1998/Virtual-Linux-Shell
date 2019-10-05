#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<dirent.h>
#include "shell.h"





  	int externalCommands(struct simple_command com){
  	int fdin;
	int fdout;
	if(com.word==NULL){
    		printf("No command entered");
    		return 1;
  		}
	if(com.inputRedirection==1){
	fdin=open(com.arguments[com.inFileLoc], O_RDONLY);
	dup2(fdin, 0);
	}
	if(com.outputORedirection==1){
	fdout = open(com.arguments[com.outFileLoc], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
	dup2(fdout,1);
	}else if(com.outputARedirection==1){
	fdout=open(com.arguments[com.outFileLoc], O_CREAT|O_WRONLY|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
	dup2(fdout,1);
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
    	}else{
    		wait(NULL);
    	}
	if(com.inputRedirection==1){
	close(fdin);
	}
	if(com.outputORedirection==1 || com.outputARedirection==1){
	close(fdout);
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
  	printf("\n\n\n\t*******************welcome to my shell**************************");
  	char* username = getenv("USER");
  	printf("\n\n\nUSER is: @%s\n", username);
  	printf("enter in commands and begin using the shelll\n");
  	sleep(2);
  	do {

		input = getInput();
		int pipe=findPipe(input);
		if(pipe==1){
		args=str_tok(input, "|");
		again=pipeExecution(args);
		}else{
        	args = str_tok(input, " ");
        	com=strParse(args);
    		again = execute(com);
		}
		free(input);
    		free(args);
  	} while (again);
	}




