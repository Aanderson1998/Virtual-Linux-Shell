#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#define SIZE 1000



char *takeInput(){
    //pointer to string to be entered by user
    char* buf;
    //readline similar to fgets() gets users input from keyboard and finishes when enter key is hit
    buf = readline("\n>>> ");
    //if something was entered as in the string doesnt equal 0
    if (strlen(buf) != 0) {
        //add it to history which allows the user to use the keyboard and backspace to fix the command
        add_history(buf);
    }
    return buf;
}



int main(void){
char string[SIZE]=takeInput();
printf("%s\n",string);
}
