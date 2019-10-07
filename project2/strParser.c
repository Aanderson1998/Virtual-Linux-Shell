#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"





	struct simple_command strParse(char **args){
	int background=0;
        int input=0;
        int outputA=0;
        int outputO=0;
        int i=0;
        int numOfItems=0;
	int outFileLoc=0;
	int inFileLoc=0;
        char *inFile;
        char *outFile;
	while(args[i]!=NULL){
	if(strcmp(args[i],"<")==0){
        input=1;
	inFileLoc=i+1;
        inFile=args[inFileLoc];
        }
        if(strcmp(args[i],">")==0){
        outputO=1;
	outFileLoc=i+1;
        outFile=args[outFileLoc];
        }
        if(strcmp(args[i],">>")==0){
        outputA=1;
	outFileLoc=i+1;
        outFile=args[outFileLoc];
        }
        if(strcmp(args[i],"&")==0){
        background=1;
        }
        numOfItems++;
        i++;
        }
        simple_command com={args[0],input,outputO,outputA,background,numOfItems,inFile,outFile,args};
	return com;
	}



