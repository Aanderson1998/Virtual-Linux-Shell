# Virtual Linux Shell

myshell is an sh-compatible command language interpreter that executes commands read from the standard input or from a file. myshell also incorporates useful features from the Bash shell.

myshell is an interactive loop: it repeatedly takes a line of input (the command entered), parses the input, executes the command specified on that line of input, and waits for the command to finish. This is repeated until the the shell exits. The shell can be invoked with either no arguments or a single argument; anything else is an error. 

Interactive mode: When the shell is invoked with no arguments it is in interactive mode. In this mode the shell prints the command line prompt and allows the user to type commands. The user can then press return and the command will run, and then print the command line prompt again meaning it is ready to execute another commands. The user can repeatedly do this until they enter quit, meaning to exit the shell.
 
Batch mode: When the shell is invoked with one argument it is in batch mode. The one argument is the batch file which contains a list of arguments. In this mode instead of the shell reading input directly from the screen, the shell reads input from the batch file and executes the commands within the file line by line. In this mode no prompt is printed to the screen, instead the shell will begin running the commands from the file directly and exit when it reaches the end of the file.
 
Built in Commands: This basic shell contains eight built in commands. This means these commands are done straight from the shell meaning no new processes need to be created to run these commands. These commands are:clr, pause, cd, dir, enviro, echo, help,and quit/

External Commands: 
All other commands, known as external commands, are available through the Unix Bourne Again Shell. In this scenario the line of input is interpreted as program invocation, which is done by the shell forking and execing the programs as its own child processes. 

I/O Redirection:
The shell supports input and output redirection on either or both the stdin (keyboard) or stdout(screen) for all external commands. That is, the command line programname arg1 arg2 < inputfile > outputfile will execute the program programname with arguments arg1 and arg2, the stdinFILE stream replaced by inputfile and the stdoutFILEstream replaced by outputfile. If an input redirection symbol ("<") is given with a file name following it, it will get its arguments to run with the command from the file instead of from the keyboard. If an output redirection symbol is given with a file name following it, it will execute the command and print the contents to the file given instead of to the screen. If you use the ">" redirection symbol it means to overwrite the file. So if the file doesn't exist it will create a new file, if the file does exists it will delete its contents before printing. If you use the ">>" redirection symbol it means to append the file. So if the file doesn't exist it will create a new file, if the file does it exist it will just add to the files content.
 
Background Execution:
The shell also supports background execution of programs for all external commands. The ampersand symbol (&) is the symbol for background execution. This symbol should be placed at the end of the command line to signal background execution. This symbol indicates that the shell should return to the command line prompt immediately after launching the program instead of waiting for the program to be done like it normally does. This allows the program to run in the background while you continue typing in other commands to be executed. 

Piping:
The shell also supports the piping of two external commands together. Piping is essentially an extension of redirection so that commands can be strung together. With piping the shell sends the output of one command to another command for further processing. Using the pipe symbol ("|") between two commands allows the stdout of the first command to be connected to the stdin of the second command. The direct connection between these commands allows them to operate simultaneously and permits data to be transferred between them continuously rather than having to pass it through temporary text files or the display screen.






