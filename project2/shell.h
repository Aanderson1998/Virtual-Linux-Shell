
#define SIZE 1000


typedef struct simple_command{
	char *word;
	int inputRedirection;
	int outputORedirection;
	int outputARedirection;
	int backgroundEx;
	int size;
	int inFileLoc;
	int outFileLoc;
	char **command;
	char **arguments;
}simple_command;



int pipeExecution(char **args);
int findPipe(char *input);
struct simple_command strParse(char **args);
char **str_tok(char *input, char *tok);
char *getinput(void);
int externalCommands(struct simple_command com);
int execute(struct simple_command com);
int internalCommands(struct simple_command com);
int quit(struct simple_command com);
int cd(struct simple_command com);
int help(struct simple_command com);
int pause();
int dir(struct simple_command com);
int echo(struct simple_command com);
int enviro(struct simple_command com);
int clr(struct simple_command com);
void printDir();
static char *intCommands[] = {"quit", "cd", "help", "pause","dir", "echo", "enviro", "clr"};
static int numInternalCommands=8;





