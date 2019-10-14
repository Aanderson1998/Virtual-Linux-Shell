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



	//executing function. (For simple commands so not commands involving pipes)
	int execute(simple_command com){
	//checks if first word in command is NULL. If it is prints error message and returns
  	if (com.word == NULL) {
		printf("no command entered");
    		return 1;
  	}
	//iterates through internal command array and compares element in array with command word entered
  	for (int i = 0; i < numInternalCommands; i++) {
		//if it equals 0 meaning they are the same word, sends command struct to internalCommands function to be executed
    		if (strcmp(com.word, intCommands[i]) == 0) {
      			return internalCommands(com);
  		}
	}
	//else it is not in internal commands array meaning it must be an external command. Sends to externalCommand function to be executed
  	return externalCommands(com);
	}





	int main(int argc, char **argv){
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
        ssize_t line_size;
	//if argcis equal to 1 that means there was no argument to myshell program call which means it should run in interactive mode
	if(argc==1){
		//do while loop to continue executing function while again==1. Which means until user enters quit
		do{
			//get input from user and store it in input*
                	input = getInput();
			//search for pipe in input using find pipe function
			int pipe=findPipe(input);
			//if there is a pipe. Tokenize input by the pipe and send that char** to pipe execution function
                	if(pipe==1){
                		args=str_tok(input, "|");
                		again=pipeExecution(args);
			//else means there is no pipe so is a simple command
                	}else{
				//tokens the input by spaces and stores that value in args
                		args = str_tok(input, " ");
				//parses the args** using the strParser function and store that in the struct com
                		com=strParse(args);
				//sends the command struct to the execute function
                		again=execute(com);
                	}
		//continue doing this while again==1 which means until quit function is called
        	} while (again);
	//else means there was a batch file to be read from
	}else{
	//creates file pointer to batch file and opens batch file to read
        FILE *fp = fopen(argv[1], "r");
	//if the pointer is null there was an error opening file. Prints error message and exits program
        if (!fp){
        	fprintf(stderr, "Error opening file '%s'\n", argv[1]);
        	exit(0);
        }else{
		//uses getline function to read a line from file and puts it in memory space of input*
		line_size = getline(&input, &line_buf_size, fp);
		//while the size of the line is greater that or equal to 0
		while (line_size >= 0){
			//print input to show file is being read correctly
			printf("%s\n",input);


//program should execute commands from batch file but there are errors when trying to do it. Here is code

/*			//it sends input to findpipe function to search for pipe. Returns 1 if pipe was found or 0 if not
                	int pipe=findPipe(input);
			//if pipe is found, tokens input by the pipe and send that to pipeExecution function
                	if(pipe==1){
                		args=str_tok(input, "|");
                		pipeExecution(args);
                	}else{
				//else it is a simple command so tokens it by spaces and stores that value in args
                		args=str_tok(input, " ");
				//parses the args** using the strParser function and stores that in the struct com
                		com=strParse(args);
				//sends the command struct to the execute function
                		execute(com);
			}
*/			//gets next line in file


			 line_size = getline(&input, &line_buf_size, fp);
		}
	}
	//closes file
	fclose(fp);
	return 1;
	}
	}
