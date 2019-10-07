#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"


//function used to get command from user during interactive mode
char *getInput(){
	//char * to hold command
	char* str;
	//getting the pathway of the current directory
	char cwd[1024];
        getcwd(cwd, sizeof(cwd));
	//printing the pathway of the current directory/myshell>> as the command line prompt and using readline to get the command entered
	printf("\n%s", cwd);
        str = readline("/myshell>> ");
	//adding command to history so user can  use backspace and back arrow to fix command
        if (strlen(str) != 0) {
                add_history(str);
        }
	//returning the command to main
        return str;
        }
