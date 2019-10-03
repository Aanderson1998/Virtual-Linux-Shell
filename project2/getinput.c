#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"


char *getInput(){
	char* str;
	char* username = getenv("USER");
	printf("\n@%s", username);
        str = readline(">>> ");
        if (strlen(str) != 0) {
                add_history(str);
        }
        return str;
        }
