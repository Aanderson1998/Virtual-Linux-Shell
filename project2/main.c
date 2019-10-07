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




	int execute(simple_command com){
	int i;
  	if (com.word == NULL) {
	printf("no command entered");
    		return 1;
  	}
  	//iterates through number of builtin commands, compares them with command inputted if one of them goes to that function
  	for (i = 0; i < numInternalCommands; i++) {
    		if (strcmp(com.word, intCommands[i]) == 0) {
      		return internalCommands(com);
    		}
  	}
  	//else it is not one of the built in commands is an external command
  	return externalCommands(com);
	}


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
                again=execute(com);
		}
                free(input);
                free(args);
        } while (again);
        }



