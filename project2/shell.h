//Size constant that is used to allocate space for char* in program
#define SIZE 1000

//command struct
typedef struct simple_command{
	char *word; //char pointer to hold main command
	int inputRedirection; //integer variable to hold input redirection. 1 if there is 0 if there is not
	int outputORedirection;  //integer variable to hold overwriting output redirection. 1 if there is 0 if there is not
	int outputARedirection; //integer variable to hold appending output redirection. 1 if there is 0 if there is not
	int backgroundEx; //integer variable to hold background execution. 1 if there is 0 if there is not
	int size; //integer variable to hold size or amount of elements in total command string
	int inFileLoc; //integer variable to hold index of input file in char** command
	int outFileLoc; //integer variable to hold index of output file in char** command
	char **command; //char ** that holds the total command string
	char **arguments; //char** that only holds the main commands and the arguments to the main command
}simple_command;


//function prototype for pipeExecution function
int pipeExecution(char **args);

//function prototype for findPipe function
int findPipe(char *input);

//function prototype for strParse function
struct simple_command strParse(char **args);

//function prototype for str_tok function
char **str_tok(char *input, char *tok);

//function prototype for getinput function
char *getinput(void);

//function prototype for executing external commands function
int externalCommands(struct simple_command com);

//function prototype for execute function. (found in main.c)
int execute(struct simple_command com);

//function prototype for internalCommands function
int internalCommands(struct simple_command com);

//function prototype for quit function
int quit(struct simple_command com);

//function prototype for cd function
int cd(struct simple_command com);

//function prototype for help function
int help(struct simple_command com);

//function prototype for pause function
int pause();

//function prototype for dir function
int dir(struct simple_command com);

//function prototype for echo function
int echo(struct simple_command com);

//function prototype for environ function
int environ(struct simple_command com);

//function prototype for clr function
int clr(struct simple_command com);

//function prototype for printDir function
void printDir();

//array of char* that hold internal commands
static char *intCommands[] = {"quit", "cd", "help", "pause","dir", "echo", "environ", "clr"};

//variable for number of internal commands
static int numInternalCommands=8;





