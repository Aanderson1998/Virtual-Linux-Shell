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
	int inFileLoc;
	int outFileLoc;
	int backgroundLoc;
	int i=0;
	int j=0;
	char **array=NULL;
	while(args[i]!=NULL){
	if(strcmp(args[i],"<")==0){
	input=1;
	inFileLoc=i+1;
	}
	if(strcmp(args[i],">")==0) {
	outputO=1;
	outFileLoc=i+1;
	}
	if(strcmp(args[i],">>")==0){
        outputA=1;
	outFileLoc=i+1;
	}
	if(strcmp(args[i],"&")==0){
        background=1;
	backgroundLoc=i;
	}
	i++;
	j++;
	}
	int count=0;
	i=0;
	if(input==0 && outputO==0 && outputA==0 && background==0){
	while(args[i]!=NULL){
	count++;
        array = (char**)realloc(array, (count+1)*sizeof(*array));
        array[count-1] = (char*)malloc(sizeof(args[i]));
        strcpy(array[count-1], args[i]);
        i++;
        }
        array[count]=NULL;
	}else{
	int min;
	if(input==1){
	min=inFileLoc-1;
	}else if(input==0 &&(outputO==1 || outputA==1)){
	min=outFileLoc-1;
	}else{
	min=backgroundLoc;
	}
	count=0;
	i=0;
	while(i<min){
	count++;
    	array = (char**)realloc(array, (count+1)*sizeof(*array));
    	array[count-1] = (char*)malloc(sizeof(args[i]));
    	strcpy(array[count-1], args[i]);
	i++;
	}
	array[count]=NULL;
	}
	simple_command com={args[0],input,outputO,outputA,background,j,inFileLoc,outFileLoc,args,array};
	return com;
	}
