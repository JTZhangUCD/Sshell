#include "inputHandler.h"
#include "parse.h"
#include "func.h"
#include "inputErrorHandler.h"

void complete(char *input, int *status)
{
    fprintf(stderr, "+ completed '%s':[%d]\n", input, *status);
}

int main(int argc, const char * argv[])
{
    char* input;
    int status;

    while (1) {
        input = (char *)malloc(MAX_INPUT_SIZE * sizeof(char));

        waitpid(-1, NULL, WNOHANG); // kill zombie child
        printf("sshell$ ");

        size_t size = MAX_INPUT_SIZE;
        getline(&input, &size, stdin);

        // input error handler
        if (argCounter(input) > 16) {
            fprintf(stderr, "Error: too many process arguments\n");
            continue;
        } else if (input == NULL) {
            continue;
        } if (isMissingCMD(input)) {
            fprintf(stderr, "Error: missing command\n");
            continue;
        }

        struct PipedParses parses = build_parse_set(input);

        if (parsesErrorHandler(&parses)) {
            continue;
        }

        struct Parse parse = parses.parses[0];
        // special case handler:
        // 1. cd cmd:
        if (strcmp(parse.args[0], "cd") == 0) {
            if (chdir(parse.args[1]) == 0) {
                strtok(input, "\n");
                fprintf(stderr, "+ completed '%s':[%d]\n", input, 0);
            } else {
                fprintf(stderr, "Error: no such directory\n");
            }
            continue;
        }
        // 2. exit cmd:
        waitpid(-1, NULL, WNOHANG);
        if (strcmp(parse.args[0], "exit") == 0) {
            fprintf(stderr, "Bye...\n");
            exit(0);
        }
        // 3. pwd
        if (strcmp(parse.args[0], "pwd") == 0) {
            char pwd_buf[256];
            getcwd(pwd_buf, sizeof(pwd_buf));
            fprintf(stdout, "'%s'\n", pwd_buf);
            continue;
        }
        
        // not pipe
        if (parses.numOfParses == 1) {
            pid_t pid = fork();
            if (pid == 0) {
                if (parse.isInputRedir) {
                    inputRedir(parse.irFilename);
                } if (parse.isOutputRedir) {
                    outputRedir(parse.orFilename);
                } if (execvp(parse.args[0], parse.args) == -1) {
                    fprintf(stderr, "Error: command not found\n");
                    exit(1);
                } else {
                    exit(0);
                }
            } else if (pid > 0) {
                waitpid(pid, &status, 0);
            } else {
                fprintf(stderr, "fork\n");
                exit(1);
            }
            // print result:
            strtok(input, "\n");
            fprintf(stderr, "+ complete '%s' [%d]\n", input, WEXITSTATUS(status));

            // is pipe
        } else {
            // init pipe
            int fd[2];
            int pipeInfoCarrier = 0;
            int pipeStatus;
            int pipeStatuses[parses.numOfParses];

            // use a for loop to go through parses
            for (int p_idx = 0; p_idx < parses.numOfParses; p_idx++) {
                waitpid(-1, NULL, WNOHANG); // kill zombie child
                pipe(fd);
                pid_t pid = fork();
                struct Parse curParse = parses.parses[p_idx];
                if (pid == 0) {
                    if (p_idx > 0) {
                        dup2(pipeInfoCarrier, STDIN_FILENO);
                    }
                    if (p_idx < parses.numOfParses-1) {
                        dup2(fd[1], STDOUT_FILENO);
                    }
                    close(fd[0]);
                    if (curParse.isInputRedir) {
                        inputRedir(curParse.irFilename);
                    }
                    if (curParse.isOutputRedir) {
                        outputRedir(curParse.orFilename);
                    }

                    if (execvp(curParse.args[0], curParse.args) == -1) {
                        fprintf(stderr, "Error: command not found\n");
                        exit(1);
                    } else {
                        exit(0);
                    }
                } else if (pid > 0) {
                    waitpid(pid, &pipeStatus, 0);
                    pipeStatuses[p_idx] = WEXITSTATUS(pipeStatus);
                    close(fd[1]);
                    pipeInfoCarrier = fd[0];
                } else {
                    fprintf(stderr, "fork in pipe\n");
                    exit(1);
                }
            }
            // print result:
            strtok(input, "\n");
            fprintf(stderr, "+ complete '%s'", input);
            for (int i = 0; i < parses.numOfParses; i++) {
                fprintf(stderr, "[%d]", pipeStatuses[i]);
            }
            fprintf(stderr, "\n");
        }
        free(input);
    }
    return EXIT_SUCCESS;
}

