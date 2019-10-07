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




	int main(int argc, char **argv){
  	char *input;
  	char **args;
  	int again;
	struct simple_command com;
	printf("\033[H\033[J");
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
                if (com.word == NULL) {
                again= 1;
                }
                for (int i = 0; i < numInternalCommands; i++) {
                if (strcmp(com.word, intCommands[i]) == 0) {
                again= internalCommands(com);
                }
                }
                again=externalCommands(com);
                }
                free(input);
                free(args);
        } while (again);
        }



