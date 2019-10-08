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





        int externalCommands(struct simple_command com){
        int fdin;
        int fdout;
        if(com.word==NULL){
                printf("No command entered");
                return 1;
                }
        if(com.inputRedirection==1){
        fdin=open(com.command[com.inFileLoc], O_RDONLY);
        dup2(fdin, 0);
        }
        if(com.outputORedirection==1){
        fdout = open(com.command[com.outFileLoc], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
        dup2(fdout,1);
        }else if(com.outputARedirection==1){
        fdout=open(com.command[com.outFileLoc], O_CREAT|O_WRONLY|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
        dup2(fdout,1);
        }
        pid_t pid = fork();
        if (pid < 0) {
                printf("\nFailed forking child..");
                return 0;
        } else if (pid == 0) {
                if (execvp(com.word, com.arguments) < 0) {
                printf("\nCould not execute command..");
                }
                exit(0);
        }else if(com.backgroundEx==0){
        wait(NULL);
	}else{
        if(com.inputRedirection==1){
        close(fdin);
        }
        if(com.outputORedirection==1 || com.outputARedirection==1){
        close(fdout);
        }
	}
        return 1;
        }

