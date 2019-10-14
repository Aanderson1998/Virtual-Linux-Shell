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


	//printDir function. Used in cd function to print the current directory
  	void printDir(){
	//uses getcwd function to get the current directory
  	char cwd[1024];
  	getcwd(cwd, sizeof(cwd));
	//prints out current directory
  	printf("\nDirectory: %s", cwd);
  	}





	//function for pause command. Calls getchar() which pauses stops programming until enter is pressed. Returns 1 when end of function is reached
	int pause(){
        getchar();
        return 1;
        }





	//function for clr command
	int clr(struct simple_command com){
	//checks size of command input. If it is greater than one or it has any arguments it is not a valid command. Prints error message and returns
	if(com.size>1){
		printf("too many arguments for command clr\n");
		return 0;
	//else clears screen by printing string
	}else{
        	printf("\033[H\033[J");
	}
	//returns 1 when end of function is reached
        return 1;
        }





	//function for quit command
  	int quit(struct simple_command com){
	//checks size of command input. If it is greater than one or it has any arguments it is not a valid command. Prints error message and returns
	if(com.size>1){
		printf("too many arguments for command quit");
		return 0;
	//else prints goodbye message
	}else{
		printf("\nThank you for using this shell. Good bye\n");
	}
	//exits from program
	exit(0);
	}





	//function for cd command
	int cd(struct simple_command com){
	//command can only take at most one argument. So if command size is over 2 there are too many arguments. Prints error message and returns
	if(com.size>2){
		printf("too many arguments for cd command\n");
		return 0;
	}
	//if there is no argument, just the cd command it just prints the current directory
	if (com.arguments[1] == NULL) {
		//calls printDir() function which prints current working directory
    		printDir();
	//else if there is an argument calls chdir() function to change to that directory.
	//If it is less than 1 that means there was an error when changing directory. Prints error message and returns
    	}else if(chdir(com.arguments[1])<0){
		printf("unable to open directory\n");
		return 0;
	}else{
	//else it changes the directory and calls printDir() function to print the directory contents
		chdir(com.arguments[1]);
		printDir();
    	}
	//returns one when end of function is reached
    	return 1;
  	}





	//function for help command
	int help(struct simple_command com){
	//pointer to file for if there is output redirection
	FILE *fp;
	//if there is no output redirection and the size of the command is 1 that means to just print help message to standard output (screen) and return
	if(com.outputORedirection==0 && com.outputARedirection==0 && com.size==1){
		puts("\n***WELCOME TO MY SHELL HELP***"
                "\nCommands supported by this shell:"
                "\n>quit"
                "\n>cd"
                "\n>help"
                "\n>pause"
                "\n>dir"
                "\n>echo"
                "\n>environ"
                "\n>clr"
                "\n>all other general commands available through the UNIX shell"
                "\n>this shell also performs pipe handling, background execution, redirection, and batch execution"
                "\n>Use the man command for information on other commands"
                "\n>Have fun using this shell!");
		return 1;
	}
	//if there is one argument (command size is 2) or the size is 3 but there is no output redirection then the arguments are invalid
	//prints error message and returns
        if(com.size==2 ||(com.size==3 && com.outputORedirection==0 && com.outputARedirection==0)){
                printf("invalid arguments\n");
                return 0;
        }
	//if the command size is greater than 3 then there are too many arguments for this command. Prints an error message and returns
	if(com.size>3){
                printf("too many arguments for help  command\n");
                return 0;
        }
	//checks for overwriting output redirection. If there is then opens file to write and assigns it to file pointer
	if(com.outputORedirection==1){
                fp = fopen(com.command[com.outFileLoc], "w");
	//if it is not overwriting redirection it is appending redirection. So opens file to append and assigns it to file pointer
        }else{
                fp=fopen(com.command[com.outFileLoc], "a");
        }
	//if file pointer is NULL that means it could not open output file. Prints error message and returns
        if(fp == NULL){
                puts("Couldn't open file");
                return 0;
	//else it prints help message to the file and returns
        }else{
		fputs("\n***WELCOME TO MY SHELL HELP***"
        	"\nCommands supported by this shell:"
        	"\n>quit"
        	"\n>cd"
        	"\n>help"
        	"\n>pause"
        	"\n>dir"
        	"\n>echo"
        	"\n>environ"
        	"\n>clr"
        	"\n>all other general commands available through the UNIX shell"
        	"\n>this shell also performs pipe handling, background execution, redirection, and batch execution"
        	"\n>Use the man command for information on other commands"
        	"\n>Have fun using this shell!",fp);
		fclose(fp);
		return 1;
        }
	//returns 1 if end of function is reached
	return 1;
	}





	//function for dir command
	int dir(struct simple_command com){
	//pointer to file for if there is output redirection
	FILE *fp;
	//getting current directory for if there is no arguments and needs to just print contents of current directory
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	//pointer to directory and directory struct
	DIR *p;
	struct dirent *d;
	//checks size of command. If it equals 3 or it equals 4 but there is no redirection, the arguments are invalid. Prints error message and returns
	if((com.size==3||com.size==4) && com.outputORedirection==0 && com.outputARedirection==0){
                printf("invalid arguments\n");
                return 0;
        }
	//if the size of the command is over four there are too many arguments for this command. Prints error message and returns
        if(com.size>4){
                printf("too many arguments for dir command\n");
                return 0;
        }
	//if the size of the command is only one that means it prints the current directorys content
	if (com.size==1){
		//assigns pointer to current working directory
		p=opendir(cwd);
	//if there is an argument that means there is a specific directory (not cwd) to display the contents of
	//assigns pointer to that directory
	} else if (com.size==2){
		p=opendir(com.arguments[1]);
	}
	//if p is NULL that means the directory could not be opened or found. Prints error message and returns
	if(p==NULL){
  		perror("Cannot find directory");
  		return 0;
  	}
	//checks for output redirection. If there is none, iterates through files in directory and prints each to the standard output and returns
	if(com.outputORedirection==0 && com.outputORedirection==0){
		while(d=readdir(p)){
  		puts(d->d_name);
	}
	return 1;
	}
	//checks for overwriting output redirection. If there is then sets file pointer to file and opens file to write
	if(com.outputORedirection==1){
        	fp = fopen(com.command[com.outFileLoc], "w");
        }else{
		//if there is not overwriting redirection then it is appending. Sets file pointer to file and opens file to append
		fp=fopen(com.command[com.outFileLoc], "a");
        }
	//if fp is NULL that means file could not be opened. Prints error message and returns
        if(fp == NULL){
        	puts("Couldn't open file");
		return 0;
        }else{
		//else if file opens, iterates through files in directory and prints them to the output file
		while(d=readdir(p)){
        	fputs(d->d_name,fp);
		return 1;
        }
	//closes output file and returns
	fclose(fp);
	return 1;
	}
	//when end of function is reached
	return 1;
	}





	//function for echo command
	int echo(struct simple_command com){
	//pointer to file for if there is output redirection
	FILE *fp;
	//integer variable starting at one to hold value in command arguments array. (0 holds echo so we start at 1 to print out every string after echo)
	int i=1;
	//checks to see if there is no output redirection. If there isn't, normally prints the string arguments to the standard output and returns
	if(com.outputORedirection==0 && com.outputARedirection==0){
		while(com.arguments[i]!= NULL){
  		printf("%s ", com.arguments[i]);
		i++;
	}
  	return 1;
	}
	//if there is overwriting output redirection, opens the file to write
	if(com.outputORedirection==1){
        	fp = fopen(com.command[com.outFileLoc], "w");
        }else{
	//else if it is not overwriting, it is appending output redirection so it opens the file to append
        	fp=fopen(com.command[com.outFileLoc], "a");
        }
	//if it is NULL the file could not be opened. Prints error message and returns
        if(fp == NULL){
        	puts("Couldn't open file");
        	return 0;
        }else{
	//else it iterates through argument array and prints out all the arguments to the file
		i=1;
		while(com.arguments[i]!=NULL){
		fprintf(fp,"%s ",com.arguments[i]);
		i++;
	}
	//closes the file
	fclose(fp);
	}
	//returns 1 if the end of function is reached
	return 1;
	}





	//function for environ command
	int environ(struct simple_command com){
	//pointer to file for if there is output redirection
	FILE *fp;
	//char* for the environment variables. Uses the getenv function to get the values of the environment variables and assigns them to proper char*
	char* username=getenv("USER");
	char* homeDir=getenv("HOME");
	char* editor=getenv("EDITOR");
	char* userID=getenv("UID");
	char* terminalEmulator=getenv("TERM");
	char* shell=getenv("SHELL");
	char* path=getenv("PATH");
	char* logName=getenv("LOGNAME");
	char* lang=getenv("LANG");
	//checks for output redirection. If there is none and the size of the command is one, meaning there are no arguments, then print the variables to the screen and return
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
	//if there is 1 or 2 arguments and there is no output redirection then the arguments are invalid. Prints an error message and returns
	if(com.size==2 ||(com.size==3 && com.outputORedirection==0 && com.outputARedirection==0)){
        	printf("invalid arguments\n");
        	return 0;
        }
	//if the command size is greater than 3 that means there are too many arguments for this command. Prints an error message and returns
        if(com.size>3){
        	printf("too many arguments for environ  command\n");
        	return 0;
        }
	//next checks for overwriting output redirection, if there is, it opens the file to write
	if(com.outputORedirection==1){
        	fp = fopen(com.command[com.outFileLoc], "w");
        }else {
	//if there isn't overwriting output redirection then it is appending redirection so it opens the file to append
        	fp=fopen(com.command[com.outFileLoc], "a");
        }
	//if fp is NULL the file couldn't be open. Prints an error message and returns
	if(fp == NULL){
        	puts("Couldn't open file");
        	return 0;
        }else{
	//if it opens it writes all environment variables to the file and returns
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
	//returns 1 if end of function is reached
	return 1;
	}





	//function used to iterate through internal commands and send command to its proper function
  	int internalCommands(struct simple_command com){
	//variable to hold the command number which is used in the switch case
    	int commandNum=0;
	//for loop that iterates through array of internal commands
  	for (int i = 0; i < numInternalCommands; i++) {
		//uses strcmp to compare command inputted to commands in array. When it equals 0 or they are the same value stops iterating
        	if (strcmp(com.word, intCommands[i]) == 0) {
		//the command number equals the index value of the command plus one (since array values start at 0 and can't have 0 for switch case)
            	commandNum = i+1;
            	break;
        	}
    	}
	//switch case to send command to proper function. They are switched by their commandNum or value in the array(plus one)
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
		environ(com);
		return 1;
    	case 8:
	      	clr(com);
		return 1;
    	default:
        	break;
    	}

    	return 1;
	}




