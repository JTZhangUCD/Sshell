all : sshell inputErrorHandler.o inputHandler.o func.o parse.o

sshell : sshell.c inputErrorHandler.o inputHandler.o func.o parse.o
	gcc -Wall -Werror -o sshell -g sshell.c inputErrorHandler.o inputHandler.o func.o parse.o

inputErrorHandler.o : inputErrorHandler.c inputErrorHandler.h
	gcc -Wall -Werror -c inputErrorHandler.c

inputHandler.o : inputHandler.c inputHandler.h
	gcc -Wall -Werror -c inputHandler.c

func.o : func.c func.h
	gcc -Wall -Werror -c func.c

parse.o : parse.c parse.h
	gcc -Wall -Werror -c parse.c
	
clean:
	rm -f *.o sshell
