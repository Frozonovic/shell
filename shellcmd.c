/**
 * @file shell_cmd.c
 * @brief Implementation for shell commands
 *
 * A separate file containing the commands that the shell supports
 * to maintain readability and organization
 *
 * @author Brody Lee (blee20@georgefox.edu)
 */


#include "shell.h"


#define NULL_CHAR '\0'
#define NEW_LINE "\n"
#define CHAR_LIMIT 255


int shell_cmd_echo(int prev_exit_code, char* command) {
    int return_val = EXIT_SUCCESS;


    char* echo_ptr = strstr(command, "echo");
    echo_ptr[strlen(echo_ptr) - 1] = NULL_CHAR;

    echo_ptr += 5;

    if (strncmp(echo_ptr, "-n", strlen("-n")) == 0) {
        echo_ptr += 3;
    }

    if (strcmp(echo_ptr, "$?") == 0) {
        char* msg = "Previous exit code: ";

        // Using `printf` here for simplicity's sake
        printf("%s%d", msg, prev_exit_code);
    } else {
        write(STDOUT_FILENO, echo_ptr, strlen(echo_ptr));
    }



    if (strncmp(command, "echo -n", strlen("echo -n")) != 0) {
        write(STDOUT_FILENO, "\n", strlen(NEW_LINE));
    }

    return return_val;
}


int shell_cmd_chdir(char* path) {
    int return_val = EXIT_SUCCESS;
    char* tokens[CHAR_LIMIT] = {NULL};
    char* token = strtok(path, " \t");

    int i = 0;
    while (token != NULL && i < CHAR_LIMIT - 1) {
        token[strcspn(token, NEW_LINE)] = NULL_CHAR;
        tokens[i++] = token;
        token = strtok(NULL, " \t");
    }

    if (i > 2) {
        char* msg = "Error: Too many arguments!\n";

        write(STDERR_FILENO, msg, strlen(msg));
        return_val = EXIT_FAILURE;
    } else {
        if (i == 1) {
            // Directory omitted, change to HOME
            chdir(getenv("HOME"));
        } else {
            // Change to specified directory
            chdir(tokens[1]);
        }
    }

    return return_val;
}


int shell_cmd_pwd(char* cwd) {
    int return_val = EXIT_SUCCESS;

    // Display the current working directory
    write(STDOUT_FILENO, cwd, strlen(cwd));
    write(STDOUT_FILENO, NEW_LINE, strlen(NEW_LINE));

    return return_val;
}


int shell_cmd_exec(int background, char* command) {
    int pid = fork();
    char* tokens[CHAR_LIMIT] = {NULL};
    char* token = strtok(command, " \t");

    int i = 0;
    while (token != NULL && i < CHAR_LIMIT - 1) {
        token[strcspn(token, NEW_LINE)] = NULL_CHAR;
        tokens[i++] = token;
        token = strtok(NULL, " \t");
    }

    // Null-terminate array
    if (strcmp(tokens[i - 1], "&") == 0) {
        tokens[i - 1] = NULL;
        background = 1;
    } else {
        tokens[i] = NULL;
    }


    // Child process
    if (pid == 0) {
        // Execute command
        execvp(*tokens, tokens);

        // If failure occurs, print last error and exit
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        if (!background) {
            waitpid(pid, NULL, 0);
        }
    } else {
        // If error occurs, print the last error that occurred
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}