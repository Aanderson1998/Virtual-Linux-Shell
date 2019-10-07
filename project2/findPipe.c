#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"


	//function to find pipe in command
	int findPipe(char *input){
	//intializes pipe to 0. meaning it is not there
	int pipe=0;
	//iterates through input
	while(*input!='\0'){
	//if pipe is found changes value of pipe to 1 meaning it is present in the string
        if(*input=='|'){
        pipe=1;
	}
	input++;
        }
	//returns integer
	return pipe;
	}
