#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"


	//function used to get command from user during interactive mode returns a char* to the command entered
	char *getInput(){
	//char * to hold command
	char* str;
	//prints out the directory pathway each time it asks for input
	//getting the pathway of the current directory
	char cwd[1024];
        getcwd(cwd, sizeof(cwd));
	//printing the pathway of the current directory/myshell>> as the command line prompt
	printf("\n%s", cwd);
	//using readline to get input from user using the keyboard and saves it in str variable
        str = readline("/myshell>> ");
	//adding command to history so user can  use backspace and back arrow to fix command
        if (strlen(str) != 0) {
                add_history(str);
        }
	//returning the command to main
        return str;
        }
