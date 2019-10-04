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
	char **beforePipe;
	char **afterPipe;
	beforePipe=str_tok(args[0]," ");
	afterPipe=str_tok(args[1]," ");
	int pipefd[2];
    	pid_t p1, p2;
	if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return 1;
    	}
    	p1 = fork();
    	if (p1 < 0) {
        printf("\nCould not fork");
        return 1;
    	}
	if (p1 == 0) {

        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
	if (execvp(beforePipe[0], beforePipe) < 0) {
            printf("\nCould not execute command 1..");
            return 1;
        }
    	} else {
        p2 = fork();
        if (p2 < 0) {
            printf("\nCould not fork");
            return 1;
        }
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(afterPipe[0], afterPipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            wait(NULL);
            wait(NULL);
        }
    }
	return 1;
}
