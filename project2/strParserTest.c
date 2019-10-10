#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "shell.h"





	//function used to get command from user during interactive mode returns a char* to the command entere
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





	//function to tokenize the input by the char *tok given
        char **str_tok(char *input, char *tok){
        //declarining and intializing integer variables, one told hold tokenSize and one to hold position in tokens array
        int tokSize = SIZE;
        int position = 0;
        //allocating space for tokens
        char **tokens = (char**)malloc(tokSize * sizeof(char*));
        //declaring char* token
        char *token;
        //printing error if allocation does not work and exiting program
        if (!tokens) {
                printf("allocation error");
                exit(0);
        }
        //setting token to strtok function which tokenizes the string inputted by the char token
        token = strtok(input, tok);
        //iterates through string while token isn't NULL puts word from string in tokens array.
        while (token != NULL) {
                tokens[position] = token;
                //increments position
                position++;
                //tokens again
                token = strtok(NULL, tok);
        }
        //sets last place in array to NULL
        tokens[position] = NULL;
        //returning char **tokens
        return tokens;
        }





	//function to parse the tokened input and create a struct of the simple command values
	struct simple_command strParse(char **args){
	//variable to hold if there is background execution. 1 for yes 0 for no
	int background=0;
	//variable to hold if there is input redirection 1 for yes 0 for no
	int input=0;
	//variable to hold if there is overwriting output redirection 1 for yes 0 for no
	int outputA=0;
	//variable to hold if there is appending output redirection 1 for yes 0 for no
	int outputO=0;
	//variable for location in array for the input file
	int inFileLoc;
	//variable for the location in the array for the output file
	int outFileLoc;
	//variable for the location of the background symbol in the array
	int backgroundLoc;
	//variable to increment through args array
	int i=0;
	//integer variable to hold size of the array. Increments by 1 as the while loop goes through to get number of elements in args array
	int j=0;
	//char** variable to hold just the command and the arguments of the command
	char **array=NULL;
	//while loop to iterate through args array to each separate string in the command input
	while(args[i]!=NULL){
		//if the string equals an input redirection symbol sets input to 1 and sets the input file location to i+1
		if(strcmp(args[i],"<")==0){
			input=1;
			inFileLoc=i+1;
		}
		//if the string equals an overwriting output redirection symbol sets outputO to 1 and sets the output file location to i+1
		if(strcmp(args[i],">")==0) {
			outputO=1;
			outFileLoc=i+1;
		}
		//if the string equals an appending output redirection symbol sets outputA to 1 and sets output file location to i+1
		if(strcmp(args[i],">>")==0){
       			outputA=1;
			outFileLoc=i+1;
		}
		//if the string equals a background execution operater sets background to 1 and sets background location to i
		if(strcmp(args[i],"&")==0){
        		background=1;
			backgroundLoc=i;
		}
		//increments the array index by 1 and increments size(j) by 1
		i++;
		j++;
	}
	//integer variables to hold indexes in the arrays
	int count=0;
	i=0;
	//checks if there is any special variables in command(input, outputO, outputA, background). If not puts all the strings in to another array because they are all arguments to the command
	if(input==0 && outputO==0 && outputA==0 && background==0){
		while(args[i]!=NULL){
			count++;
			//reallocates data for the array each time through
        		array = (char**)realloc(array, (count+1)*sizeof(*array));
        		array[count-1] = (char*)malloc(sizeof(args[i]));
        		strcpy(array[count-1], args[i]);
        		i++;
        	}
		//sets last position in array to NULL
        	array[count]=NULL;
	}else{
	//else there is some form of a special symbol in the command input. Need to find location of first special symbol in command because everything before that goes in argument array
	//use min variable to keep location of first special symbol
	int min;
	//checks if there is an input symbol(if so it would be the first special symbol in the command)
	//if there is an input symbol then the index of it would be at the input file location -1
	//sets min to this value
	if(input==1){
		min=inFileLoc-1;
	//if there is no input and there is either type of output execution, the smallest index would be at that locations. Which is output file index -1
	//sets min to this value
	}else if(input==0 &&(outputO==1 || outputA==1)){
		min=outFileLoc-1;
	//else there is no redirection and the special symbol is the background execution symbol
	//sets min to this value
	}else{
		min=backgroundLoc;
	}
	//next creates array to hold just the arguments. So why the index of the args array doesn't equal min, iterates through args and copies value in to array to hold just the arguments
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
	//creates struct for simple command with these values
	simple_command com={args[0],input,outputO,outputA,background,j,inFileLoc,outFileLoc,args,array};
	//returns the struct
	return com;
	}




	int main(void){
	//char* to hold the string for the commanf
        char *input;
        //char** to hold the tokenized input
        char **args;
        //integer variable to keep program running when in interactive mode
        int again;
        //struct variable to hold command struct
        struct simple_command com;
        //size_t variables to hold line size and buf size of command when read from file
        size_t line_buf_size = 0;
	//printing statement to enter command
        printf("enter in command to be parsed\n");
	//gets input from user and stores it in input
        input = getInput();
        //tokens the input by spaces and stores that value in args
	args = str_tok(input, " ");
	//parses the args** using the strParser function and store that in the struct com
	com=strParse(args);
	//integer variable to hold index of argument array when iterating through array
        int k=0;
        printf("\n\n\ncommand structure elements are:\n");
	//prints out the main command which equals com.word
        printf("\nmain command: %s\n",com.word);
	//iterates through argument array and prints out all the arguments
        printf("\nthe arguments are:\n");
        while (com.arguments[k]!=NULL){
        	printf("%s\n",com.arguments[k]);
        	k++;
        }
	//checks if there is input redirection and if it is prints the input file
        if(com.inputRedirection==1){
        	printf("There is input redirection from %s\n",com.command[com.inFileLoc]);
	//else prints there is no input redirection
        }else if(com.inputRedirection==0){
        	printf("there is no input redirection\n");
        }
	//checks for appending output redirection and if there is prints the output file
        if(com.outputARedirection==1){
        	printf("there is appending output redirection to %s\n", com.command[com.outFileLoc]);
	//else prints there is no appending output redirection
        }else if (com.outputARedirection==0){
        	printf("there is no appending output redirection\n");
        }
	//checks if there is overwriting outputredirection and if there is prints the output file
        if(com.outputORedirection==1){
        	printf("there is overwrite output redirection to %s\n", com.command[com.outFileLoc]);
	//else prints there is no overwriting output redirection
        }else if(com.outputORedirection==0){
        	printf("there is no overwriting output redirection\n");
        }
	//checks if there is background execution. If there is says there is else, says there isn't
	if(com.backgroundEx==1){
		printf("There is background execution\n");
	}else{
		printf("There is no background execution\n");
        }
	}

