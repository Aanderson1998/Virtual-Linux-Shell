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


	int pipeExecution(char **args){
	//declaring char** to store the values of the parsed input before the pipe and parsed input after the pipe
	char **beforePipe;
	char **afterPipe;
	//tokening input before pipe and after pipe by spaces. (because before pipe and after pipe are both their own simple commands)
	beforePipe=str_tok(args[0]," ");
	afterPipe=str_tok(args[1]," ");
	//creating to command structs. One for command before pipe and one for command after pipe
	//sending before pipe and after pipe char**'s to strparser function to create command structs and setting them equal to com1 and com2
	simple_command com1=strParse(beforePipe);
	simple_command com2=strParse(afterPipe);
	//integer array to hold value of file descriptors for command. pipefd[0]=input pipefd[1]=output
	int pipefd[2];
	//declaring to pid integer variables
    	pid_t p1, p2;
	//seeing if pipe could be initialized. If return value is less than 0 there was an error. Prints error message and returns
	if (pipe(pipefd) < 0) {
        	printf("\nPipe could not be initialized");
        	return 1;
    	}
	//else there was no error so forks a new child process for the first command
    	p1 = fork();
	//if it returns a value less than 0 there was an error forking. Prints error message and returns
    	if (p1 < 0) {
        	printf("\nCould not fork");
        	return 1;
    	}
	//else if the value equals 0 it is the child process
	if (p1 == 0) {
		//closes the normal input
        	close(pipefd[0]);
		//calls dup2 function to duplicate the file descriptor for the output file into the stdout file descriptor place
        	dup2(pipefd[1], STDOUT_FILENO);
		//closes the output
        	close(pipefd[1]);
		///uses the execvp function to replace the current process with the new command input
                //if the value is less than 0 that means the command could not be executed, prints an error message and returns
		if (execvp(com1.word, com1.arguments) < 0) {
            		printf("\nCould not execute command 1..");
            		return 1;
        	}
    	} else {
		//else it is successful and forks a new child to do the second command (after pipe)
        	p2 = fork();
		//if it returns a value less than 0 there was an error forking. Prints error message and returns
        	if (p2 < 0) {
            		printf("\nCould not fork");
            		return 1;
        	}
		//else if the value equals 0 it is the child process
        	if (p2 == 0) {
			//closes normal output
            		close(pipefd[1]);
			//calls dup2 function to duplicate the file descriptor for the input file into the stdin file descriptor place
            		dup2(pipefd[0], STDIN_FILENO);
			//closes the input
            		close(pipefd[0]);
			//uses the execvp function to replace the current process with the new command input
			//if the value is less than 0 that means the command could not be executed, prints an error message and returns
            		if (execvp(com2.word, com2.arguments) < 0) {
                		printf("\nCould not execute command 2..");
                		exit(0);
            		}
		//else it was successful. Checks for background execution (would only be in command after pipe because it occurs at the end of the input)
        	} else if(com2.backgroundEx==0){
			//if it is 0 calls wait
            		wait(NULL);
            		wait(NULL);
        	}else{
			//else it returns
			return 1;
		}
	}
	//returns when end of function is reached
	return 1;
	}
