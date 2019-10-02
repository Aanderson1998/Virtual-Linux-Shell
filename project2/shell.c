#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<dirent.h>
#define SIZE 1000

struct dirent *de;


//function declarations for built in commands
int quit();
int cd(char **args);
int help(char **args);
int pause();
int dir(char **args);
int echo(char **args);
int enviro(char **args);
int clr();
void printDir();

//pointer to array containing name of built in commands
int numInternalCommands=8;
char *intCommands[] = {"quit", "cd", "help", "pause","dir", "echo", "enviro", "clr"};


  	void printDir(){
  	char cwd[1024];
  	getcwd(cwd, sizeof(cwd));
  	printf("\nDirectory: %s", cwd);
  	}

  	//internal command functions
  	int quit(){
	printf("\nThank you for using this shell. Good bye\n");
	exit(0);
	}

	int cd(char** args){
	if (args[1] == NULL) {
    	printf("expected argument to \"cd\"\n");
    	return 0;
    	} else {
      	chdir(args[1]);
    	}
    	return 1;
  	}

	int help(char **args){
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

	int pause(){
	getchar();
	return 1;
	}

	int dir(char **args){
	DIR *dr = opendir(".");
  	if (dr == NULL){
    	printf("Could not open current directory" );
    	}
  	while ((de = readdir(dr)) != NULL){
   	printf("%s\n", de->d_name);
    	closedir(dr);
	}
  	return 1;
	}

	int echo(char **args){
	int i=1;
	while(args[i]!= NULL){
  	printf("%s ", args[i]);
	i++;
	}
  	return 1;
	}

	int enviro(char **args){
	 char* username=getenv("USER");
	char* homeDir=getenv("HOME");
	char* editor=getenv("EDITOR");
	char* userID=getenv("UID");
	char* terminalEmulator=getenv("TERM");
	char* shell=getenv("SHELL");
  	printf("\n\nUser is: @%s\n", username);
	printf("Home Directory is: @%s\n", homeDir);
	printf("Editor is: @%s\n", editor);
	printf("Your unique user ID is: @%s\n", userID);
	printf("The terminal emulator is: @%s\n", terminalEmulator);
	printf("The shell you are using is: @%s\n", shell);
  	return 1;
	}

	int clr(){
	printf("\033[H\033[J");
  	return 1;
	}

  	//function to decide which internal command you are going to
  	int internalCommands(char **args){
    	int commandNum=0;
  	for (int i = 0; i < numInternalCommands; i++) {
          //compares the parsed command inputed with comands in list
        	if (strcmp((args[0]), intCommands[i]) == 0) {
            	//if command matches up adds 1 to it so it will match up with value of switch case
            	commandNum = i+1;
            	break;
        	}
    	}
    	//switch case for built in commands
    	switch (commandNum) {
    	case 1:
		//quit
	        quit();
		return 1;
     	case 2:
		//cd
        	cd(args);
		return 1;
      	case 3:
		//help
        	help(args);
		return 1;
    	case 4:
        	//pause
		pause();
		return 1;
    	case 5:
        	//dir
		dir(args);
		return 1;
    	case 6:
        	//echo
	        echo(args);
		return 1;
    	case 7:
        	//enviro
		enviro(args);
		return 1;
    	case 8:
        	//clr
	      	clr();
		return 1;
    	default:
        	break;
    	}

    	return 1;
	}


  	//function for running external commands
  	int externalCommands(char **args){
  	if(args[0]==NULL){
    		printf("No command entered");
    		return 1;
  		}
  	pid_t pid = fork();
  	if (pid < 0) {
   		printf("\nFailed forking child..");
    		return 0;
  	} else if (pid == 0) {
      		if (execvp(args[0], args) < 0) {
        	printf("\nCould not execute command..");
        	}
    		exit(0);
    	} else{
    		wait(NULL);
    	}
	return 1;
	}

	//getting command input
	char *getInput(){
  	char* str;
	char* username = getenv("USER");
	printf("\n@%s", username);
    	str = readline(">>> ");
    	if (strlen(str) != 0) {
        	add_history(str);
    	}
    	return str;
	}

	//tokenizer
	char **str_tok(char *input){
 	int tokSize = SIZE;
  	int position = 0;
  	//allocate space in memory for tokenized string, array of separate strings in long string
  	char **tokens = (char**)malloc(tokSize * sizeof(char*));
  	char *token;
  	if (!tokens) {
    		printf("allocation error");
    		exit(0);
  	}
  	token = strtok(input, " ");
  	while (token != NULL) {
    		tokens[position] = token;
    		position++;
    		token = strtok(NULL, " ");
  	}
  	tokens[position] = NULL;
  	return tokens;
	}

	int execute(char **args){
  	int i;
  	if (args[0] == NULL) {
    		return 1;
  	}
  	//iterates through number of builtin commands, compares them with command inputted if one of them goes to that function
  	for (i = 0; i < numInternalCommands; i++) {
    		if (strcmp(args[0], intCommands[i]) == 0) {
      		return internalCommands(args);
    		}
  	}
  	//else it is not one of the built in commands is an external command
  	return externalCommands(args);
	}


	int main(int argc, char **argv){
  	char *input;
  	char **args;
  	int again;
  	//displaying shell
  	clr();
  	printf("\n\n\n\t*******************welcome to my shell**************************");
  	char* username = getenv("USER");
  	printf("\n\n\nUSER is: @%s\n", username);
  	printf("enter in commands and begin using the shelll\n");
  	sleep(2);
  	do {
    		input = getInput();
    		args = str_tok(input);
    		again = execute(args);
		free(input);
    		free(args);
  	} while (again);
	}



