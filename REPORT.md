# Ecs150project1

**With this project we followed the phase 0 to phase 8 in order. And in this report we will 
explain how we achieved each phases.**

## 5.1 Phase 0: preliminary work
The sshell.c is a simple file using the build in system() function that print That file prints the 
standard UTC time from 'bin/date -u'.

*-Wall* and *-Werror* enables all the warning and error flag, while the *-g sshell.c -o sshell* 
create a executable file named sshell from sshell.c.

## 5.2 Phase 1: running the commands in the hard way
As mentioned in the lecture, we used the fork() function to create a child process as shown in
lectures. And as the instruction mentioned, we hard coded the 'printing UTC time function', 
and we set the destination of fprintf function to 'stderr' instead of 'stdout'.

## 5.3 Phase 2: read commands from the input
Since we can assume that the user can only enter the name of a program without any
arguments, we simply use strcpy function to get all the information from the stdin, and
abandon anything following the first white space. We used the execvp function to search 
programs in the in the $PATH
> it searches the directories listed in the PATH enviroment variables to find the full file name of a file from *filename* if *filename* does not contain a slash.

### 5.3.1 Error management
For this part, we did it a bit later because we are not sure how to handle every error properly. 
To handle this properly, we created a new file inputErrorHandler.c with two useful functions. 
This is how we did it after finishing phase 7:

#### Error: too many process arguments
We created a function named argCounter in func.c to help us to count how many arguments in
a given input string. If the return value of argCount is greater than 16, we fprintf error info to 
stderr and continue to next cycle of the while loop.
#### Error: no such directory
Chdir return a value indicating if a directory is valid or not when executing cd command. We
took advantage of this and push error info to stderr if directiory is not valid. 
#### Error: command not found
Similar to using chdir to handle previous error: for this error, we take advantage of the return 
value of execvp when execute a given cmd and push error info to stderr if cmd is not valid. 
#### Error: missing command
Similar to "Error: too many process arguments", we created a function named isMissingCMD 
that reads input string and output where some commands are missing or not. 
For background sign '&' if there is no corresponding argument detected, we also push this 
error message to stderr.
#### Error: mislocated input/output redirection
Since only the first command in pipe is allowed to redirect input and only the last command is 
allowed to redirect output, we simple check if all commands are following this rule or not in 
parsesErrorHandler function of inputErrorHandler.c.
#### Error: no input/output file
If there is a '<' or '>' sign with no following filename, we push the error to stderr.
#### Error: cannot open input/output file
For input: we use open function to check if the given file director is valid or not;
For output: since we need to create a new txt file in some instances, we only make it not valid
to access and system file. 
#### Error: mislocated background file
If there is a '&' sign detected, we will take the previous argument as the related argument. 
However, if there is the argument is occupied by other signs (such as '<'), we push this error to
stderr.
#### Error: active jobs still running
If there is a backgroud process that is still running while another input string attempt is made, 
we push this error to stderr

## 5.4 Phase 3: arguments
We used to use strtok() function to break the arguments down so we can let the program deal 
with them one by one, however, the drawback of the strtok() function is that it is not a good 
option dealing with special symbols. In addition it modifies the original input. So we decide to 
change the input argument to a char* temp. Every time the program reads a command, we scan
the entire command till we find a whitespace, and we push it to char* temp (because we know 
that arguments are separated by whitespaces). Then we read the char* temp to acquire the 
valid argument.

## 5.5 Phase 4: builtin commands
We first extract the arguments using the method introduced in Phase 3, then we compare the 
input command with our flags to figure out which function should be executed. For example, 
when we read a 'exit' in our char* temp(modified input that helps us to find the valid input), we
print the text 'Bye...' to the stderr followed exiting with exit code 0.

### 5.5.1 exit
We simply compared the input arguments with 'exit'. Once we get a true, we print "Bye..." to
stderr followed by exit with code '0'. 

### 5.5.2 cd and pwd
The cd command is executed when it is detected using methods introduced in phase 2. 
It should be executed in the main process otherwise the directory will not be changed. We use 
chidir function to implement the change directiory and check if the destination directory is valid 
or not.
The pwd command is executed when it is detected using the methods introduced in phase 2.
We use the getcwd function to get the current directory, stores it in a buffer, and later on simply
print the buffer.

## 5.6 Phase 5: input redirection
We set a boolean variable to detect if there is input redirection. After we scanned through the 
input arguments using the method introduced by phase 2, we look for a '<' symbol as the 
indicator for input redirection, and the argument right next to the '<' symbol will be the file name
where input comes from. The boolean will be false as default and true if '<' found. And if the
boolean is true while the next argument is empty or doesn't exist, an error message is popped.
When the child process executed, we write the file name into *STDIN_FILENO*, and the execvp
function will automatically read from it and execute.

## 5.7 Phase 6: output redirection
The implementation of this phase is similar to phase 5. We set a boolean variable to detect if
there is output redirection. After we scanned through the input arguments using the method
introduced by phase 2, we look for a '>' symbol as the indicator for output redirection, and the 
argument right next to the '>' symbol will be the file name where output goes. The boolean will 
be set to false as default and true if '>' is detected. And if the boolean is true while next 
argument is empty or is a system file, an error message is popped. When there is no error and 
the child process executed, we write the file name into *STDOUT_FILENO*, and the execvp
function will automatically read the output file name and execute.

## 5.8 Phase 7: Pipeline commands
When we get a '|' symbol, we create a for loop using the fork() function to create a bunch of
child process to execute the input command. After that we will have several child process to 
execute the command. Each children process will be under a struct called Parse, and they will 
be sotred in a struct called PipedParse. We use pipe() and dup2() function to redirect the 
output of the previous process to the next process as input. 

## 5.9 Phase 8:
We have the general idea about how its done, but sadly we do not have time to implement it.
