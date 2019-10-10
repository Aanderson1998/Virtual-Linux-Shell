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




	//function for executing external commands
        int externalCommands(struct simple_command com){
	//integer variables to hold values of stdin and stdout file descriptors
        int fdin;
        int fdout;
	//checks to see if command was entered. If not prints error message and returns
        if(com.word==NULL){
                printf("No command entered");
                return 1;
                }
	//forks new child process
	pid_t pid = fork();
	//if the value is less than 0 that means the fork did not work. Prints error message and returns
        if (pid < 0) {
                printf("\nFailed forking child..");
                return 0;
	//if the pid value equals 0 that means it is the child process
        } else if (pid == 0) {
		//checks the command structure to see if there is input redirection.
		//If there is it opens the file to read and use dup2 to duplicate the file descriptor for the input file into the stdin file descriptor place
        	if(com.inputRedirection==1){
        		fdin=open(com.command[com.inFileLoc], O_RDONLY);
        		dup2(fdin, 0);
			close(fdin);
        	}
		//checks the command structure to see if there is overwriting output redirection.
		//If there is it opens the file to truncate and write and use dup2 to duplicate the file descriptor for the output file into the stdout file descriptor place
        	if(com.outputORedirection==1){
        		fdout = open(com.command[com.outFileLoc], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
        		dup2(fdout,1);
			close(fdout);
		//checks the command structure to see if there is appending output redirection.
		//If there is it opens the file to append and write and use dup2 to duplicate the file descriptor for the output file into the stdout file descriptor place
        	}else if(com.outputARedirection==1){
        		fdout=open(com.command[com.outFileLoc], O_CREAT|O_WRONLY|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
        		dup2(fdout,1);
			close(fdout);
        		}
		//uses the execvp function to replace the current process with the new command input
		//if the value is less than 0 that means the command could not be executed, prints an error message and returns
		if (execvp(com.word, com.arguments) < 0) {
                	printf("\nCould not execute command..");
			return 0;
                }else{
			//if running the command is successful it checks if there is no background execution
			// if background execution is 0 (false) it calls wait function.
			if(com.backgroundEx==0){
        			wait(NULL);
			}
		}
		//returns 1 when reaching end of function
		return 1;
		}
	}
