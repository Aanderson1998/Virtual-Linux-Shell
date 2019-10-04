#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"



	int findPipe(char *input){
	int pipe=0;
	while(*input!='\0'){
        if(*input=='|'){
        pipe=1;
	}
	input++;
        }
	return pipe;
	}
