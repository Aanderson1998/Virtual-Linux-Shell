
#define SIZE 1000


typedef struct simple_command{
char *word;
char **arguments;
int inputRedirection;
int outputARedirection;
int outputORedirection;
int piping;
int backgroundEx;
}simple_command;



struct simple_command strParse(char **args);
char **str_tok(char *input);
char *getinput(void);
int externalCommands(struct simple_command com);
int execute(struct simple_command com);
int internalCommands(struct simple_command com);
int quit();
int cd(struct simple_command com);
int help(struct simple_command com);
int pause();
int dir(struct simple_command com);
int echo(struct simple_command com);
int enviro(struct simple_command com);
int clr();
void printDir();
static char *intCommands[] = {"quit", "cd", "help", "pause","dir", "echo", "enviro", "clr"};
static int numInternalCommands=8;
