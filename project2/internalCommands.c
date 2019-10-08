#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/types.h>
#include "shell.h"



  	void printDir(){
  	char cwd[1024];
  	getcwd(cwd, sizeof(cwd));
  	printf("\nDirectory: %s", cwd);
  	}



	int pause(){
        getchar();
        return 1;
        }



	int clr(struct simple_command com){
	if(com.size>1){
		printf("too many arguments for command clr\n");
		return 0;
	}else{
        	printf("\033[H\033[J");
	}
        return 1;
        }



  	int quit(struct simple_command com){
	if(com.size>1){
		printf("too many arguments for command quit");
		return 0;
	}else{
		printf("\nThank you for using this shell. Good bye\n");
	}
	exit(0);
	}



	int cd(struct simple_command com){
	if(com.size>2){
		printf("too many arguments for cd command\n");
		return 0;
	}
	if (com.arguments[1] == NULL) {
    		printDir();
    	}else if(chdir(com.arguments[1])<0){
		printf("unable to open directory\n");
		return 0;
	}else{
      		chdir(com.arguments[1]);
		printDir();
    	}
    	return 1;
  	}



	int help(struct simple_command com){
	FILE *fp;
	if(com.outputORedirection==0 && com.outputARedirection==0 && com.size==1){
	puts("\n***WELCOME TO MY SHELL HELP***"
                "\nCommands supported by this shell:"
                "\n>quit"
                "\n>cd"
                "\n>help"
                "\n>pause"
                "\n>dir"
                "\n>echo"
                "\n>enviro"
                "\n>clr"
                "\n>all other general commands available through the UNIX shell"
                "\n>this shell also performs pipe handling, background execution, redirection, and batch execution"
                "\n>Use the man command for information on other commands"
                "\n>Have fun using this shell!");
		return 1;
	}
        if(com.size==2 ||(com.size==3 && com.outputORedirection==0 && com.outputARedirection==0)){
                printf("invalid arguments\n");
                return 0;
        }
                if(com.size>3){
                printf("too many arguments for help  command\n");
                return 0;
        }
                if(com.outputORedirection==1){
                fp = fopen(com.command[com.outFileLoc], "w");
        }else{
                fp=fopen(com.command[com.outFileLoc], "a");
        }
        if(fp == NULL){
                puts("Couldn't open file");
                return 0;
        }else{
		fputs("\n***WELCOME TO MY SHELL HELP***"
        	"\nCommands supported by this shell:"
        	"\n>quit"
        	"\n>cd"
        	"\n>help"
        	"\n>pause"
        	"\n>dir"
        	"\n>echo"
        	"\n>enviro"
        	"\n>clr"
        	"\n>all other general commands available through the UNIX shell"
        	"\n>this shell also performs pipe handling, background execution, redirection, and batch execution"
        	"\n>Use the man command for information on other commands"
        	"\n>Have fun using this shell!",fp);
		fclose(fp);
		return 1;
        }
	return 1;
	}



	int dir(struct simple_command com){
	FILE *fp;
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	DIR *p;
	struct dirent *d;
	if((com.size==3||com.size==4) && com.outputORedirection==0 && com.outputARedirection==0){
                printf("invalid arguments\n");
                return 0;
        }
        if(com.size>4){
                printf("too many arguments for dir command\n");
                return 0;
        }
	if (com.size==1){
	p=opendir(cwd);
	} else if (com.size==2){
	p=opendir(com.arguments[1]);
	}
	if(p==NULL){
  		perror("Cannot find directory");
  		return 0;
  	}
	if(com.outputORedirection==0 && com.outputORedirection==0){
		while(d=readdir(p)){
  		puts(d->d_name);
	}
		return 1;
	}
	if(com.outputORedirection==1){
        	fp = fopen(com.command[com.outFileLoc], "w");
        }else{
		fp=fopen(com.command[com.outFileLoc], "a");
        }
        if(fp == NULL){
        	puts("Couldn't open file");
		return 0;
        }else{
		while(d=readdir(p)){
        	fputs(d->d_name,fp);
        }
	fclose(fp);
	return 1;
	}
	return 1;
	}



	int echo(struct simple_command com){
	FILE *fp;
	int i=1;
	if(com.outputORedirection==0 && com.outputARedirection==0){
		while(com.arguments[i]!= NULL){
  		printf("%s ", com.arguments[i]);
		i++;
	}
  		return 1;
	}
	if(com.outputORedirection==1){
        	fp = fopen(com.command[com.outFileLoc], "w");
        }else{
        	fp=fopen(com.command[com.outFileLoc], "a");
        }
        if(fp == NULL){
        	puts("Couldn't open file");
        	return 0;
        }else{
		i=1;
		while(com.arguments[i]!=NULL){
		fprintf(fp,"%s ",com.arguments[i]);
		i++;
	}
		fclose(fp);
	}
	return 1;
	}



	int enviro(struct simple_command com){
	FILE *fp;
	char* username=getenv("USER");
	char* homeDir=getenv("HOME");
	char* editor=getenv("EDITOR");
	char* userID=getenv("UID");
	char* terminalEmulator=getenv("TERM");
	char* shell=getenv("SHELL");
	char* path=getenv("PATH");
	char* logName=getenv("LOGNAME");
	char* lang=getenv("LANG");
	if(com.outputORedirection==0 && com.outputARedirection==0 && com.size==1){
  		printf("\n\nUser is: @%s\n", username);
		printf("Home Directory is: @%s\n", homeDir);
		printf("Editor is: @%s\n", editor);
		printf("Your unique user ID is: @%s\n", userID);
		printf("The terminal emulator is: @%s\n", terminalEmulator);
		printf("The shell you are using is: @%s\n", shell);
		printf("The full path is: %s\n", path);
		printf("The login name is: %s\n", logName);
		printf("The language is: %s\n", lang);
		return 1;
	}
	if(com.size==2 ||(com.size==3 && com.outputORedirection==0 && com.outputARedirection==0)){
        	printf("invalid arguments\n");
        	return 0;
        }
        	if(com.size>3){
        	printf("too many arguments for enviro  command\n");
        	return 0;
        }
		if(com.outputORedirection==1){
        	fp = fopen(com.command[com.outFileLoc], "w");
        }else {
        	fp=fopen(com.command[com.outFileLoc], "a");
        }
	if(fp == NULL){
        	puts("Couldn't open file");
        	return 0;
        }else{
		fprintf(fp,"\n\nUser is: @%s\n", username);
        	fprintf(fp,"Home Directory is: @%s\n", homeDir);
        	fprintf(fp,"Editor is: @%s\n", editor);
        	fprintf(fp,"Your unique user ID is: @%s\n", userID);
        	fprintf(fp,"The terminal emulator is: @%s\n", terminalEmulator);
        	fprintf(fp,"The shell you are using is: @%s\n", shell);
		fprintf(fp,"The full path is: %s\n", path);
                fprintf(fp,"The login name is: %s\n", logName);
                fprintf(fp,"The language is: %s\n", lang);
  		fclose(fp);
		return 1;
	}
	return 1;
	}


  	int internalCommands(struct simple_command com){
    	int commandNum=0;
  	for (int i = 0; i < numInternalCommands; i++) {
        	if (strcmp(com.word, intCommands[i]) == 0) {
            	commandNum = i+1;
            	break;
        	}
    	}
    	switch (commandNum) {
    	case 1:
	        quit(com);
		return 1;
     	case 2:
        	cd(com);
		return 1;
      	case 3:
        	help(com);
		return 1;
    	case 4:
		pause();
		return 1;
    	case 5:
		dir(com);
		return 1;
    	case 6:
	        echo(com);
		return 1;
    	case 7:
		enviro(com);
		return 1;
    	case 8:
	      	clr(com);
		return 1;
    	default:
        	break;
    	}

    	return 1;
	}




