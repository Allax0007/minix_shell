// Operating Systems Programming Assignment 1
// B104030020 盧立宸
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 128
#define MAX_ARGS 32

void execute_command(char *command){
    char *args[MAX_ARGS];
    char *args2[MAX_ARGS]; // For pipe
    char *token;
    int i = 0;
    int backg_flag = 0; // Flag to indicate background execution
    int output_flag = 0; // Flag to indicate output redirection
    int input_flag = 0; // Flag to indicate input redirection
    int pipe_flag = 0; // Flag to indicate pipe
    char *output_file = NULL; // File for output redirection
    char *input_file = NULL; // File for input redirection

    token = strtok(command, " \n");
    while(token != NULL && i < MAX_ARGS - 1){
        if(strcmp(token, "&") == 0){
            backg_flag = 1;
            break; // Background execution, no need to parse further
        }else if(strcmp(token, ">") == 0){
            output_flag = 1;
            token = strtok(NULL, " \n");
            output_file = token;
            break; // Output redirection, no need to parse further
        }else if(strcmp(token, "<") == 0){
            input_flag = 1;
            token = strtok(NULL, " \n");
            input_file = token;
            break; // Input redirection, no need to parse further
        }else if(strcmp(token, "|") == 0){
            pipe_flag = 1;
            for(int j = 0; j < i; j++){
                args2[j] = args[j];
                args[j] = NULL;
            }
            args2[i] = NULL;
            i = 0;
            // Continue parsing the second command
        }
        if(strcmp(token, "|"))args[i++] = token;
        token = strtok(NULL, " \n");
    }
    
    args[i] = NULL;

    if(strcmp(args[0], "exit") == 0) exit(0); // Exit the shell
    else if(strcmp(args[0], "cd") == 0){
        if(chdir(args[1]) != 0) perror("chdir"); // Change directory
        return;
    }

    pid_t pid = fork();
    if(pid == 0){
        // Child process
        if(backg_flag){
            // background execution
            int fd = open("/dev/null", O_WRONLY);
            dup2(fd, 0); // Redirect stdin to /dev/null
            dup2(fd, 1); // Redirect stdout to /dev/null
            dup2(fd, 2); // Redirect stderr to /dev/null
            close(fd);
            signal(SIGCHLD, SIG_IGN); // Ignore SIGCHLD signal
        }
        if(output_flag){
            int fd = open(output_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(input_flag){
            int fd = open(input_file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if(pipe_flag){
            int fd[2];
            pipe(fd);
            pid_t pid2 = fork();
            if(pid2 == 0){
                // Child process
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                execvp(args2[0], args2);
                // If execvp returns, there's an error
                perror("execvp");
                exit(EXIT_FAILURE);
            }else if(pid2 < 0){
                // Fork failed
                perror("fork");
            }else{
                // Parent process
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                waitpid(pid2, NULL, 0);
            }
        }
        execvp(args[0], args);
        // If execvp returns, there's an error
        perror("execvp");
        exit(EXIT_FAILURE);
    }else if(pid < 0){
        // Fork failed
        perror("fork");
    }else{
        // Parent process
        if(backg_flag) printf("[PID %d]\n", pid);           
        else waitpid(pid, NULL, 0); 
    }
}

int main(){
    char command[MAX_COMMAND_LENGTH];
    char path[MAX_COMMAND_LENGTH];
    while(1){
        getcwd(path, sizeof(path)-1);
        printf("%s@minix3 %s %% ", getenv("USER"), path);
        fflush(stdout);

        // EOF or error, exit the shell
        if(fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) break;

        execute_command(command);
    }
    return 0;
}