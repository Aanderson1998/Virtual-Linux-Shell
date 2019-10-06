output:
	gcc getinput.c strTok.c strParser.c internalCommands.c externalCommands.c findPipe.c pipeExecution.c main.c -lreadline -o myshell
getinput.o: getinput.c shell.h
	gcc -c getinput.c
strTok.o: strTok.c shell.h
	gcc -c strTok.c
strParser.o: strParser.c shell.h
	gcc -c strParser.c
internalCommands.o: internalCommands.c shell.h
	gcc -c internalCommands.c
externalCommands.o: externalCommands.c shell.h
	gcc -c externalCommands.c
findPipe.o: findPipe.c shell.h
	gcc -c findPipe.c
pipeExecution.o: pipeExecution.c shell.h
	gcc -c pipeExecution.
main.o: main.c shell.h
	gcc -c main.c  
clean:
	rm *.o myshell
