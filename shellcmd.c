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

    // Find the substring "echo"
    char* echo_ptr = strstr(command, "echo");

    if (echo_ptr == NULL) {
        return_val = EXIT_FAILURE;
    } else {
        // Move the pointer after "echo"
        echo_ptr += strlen("echo ");

        // Remove possible new-line
        echo_ptr[strcspn(echo_ptr, NEW_LINE)] = NULL_CHAR;

        // Check for the "-n" option
        if (strncmp(echo_ptr, "-n", strlen("-n")) == 0) {
            echo_ptr += strlen("-n");
        }

        // Check if the command contains "$?"
        char* exit_code_ptr = strstr(echo_ptr, "$?");
        if (exit_code_ptr != NULL) {
            char* msg = "Previous exit code: ";

            // Replace "$?" with the previous exit code
            sprintf(exit_code_ptr, "%d", prev_exit_code);

            write(STDOUT_FILENO, msg, strlen(msg));
            write(STDOUT_FILENO, exit_code_ptr, strlen(exit_code_ptr));
        } else {
            // "$?" not found, write the command to STDOUT
            write(STDOUT_FILENO, echo_ptr, strlen(echo_ptr));
        }

        // Add a newline character if "-n" option is not present
        if (strncmp(command, "echo -n", strlen("echo -n")) != 0) {
            write(STDOUT_FILENO, "\n", strlen("\n"));
        }
    }

    return return_val;
}



int shell_cmd_chdir(char* path) {

    int return_val = EXIT_SUCCESS;
    char* tokens[CHAR_LIMIT] = {NULL};
    char* token = strtok(path, " \t");

    // Tokenizing, argument counting, and new-line removal all in one
    int i = 0;
    while (token != NULL && i < CHAR_LIMIT - 1) {
        token[strcspn(token, NEW_LINE)] = NULL_CHAR;
        tokens[i++] = token;
        token = strtok(NULL, " \t");
    }

    // Assert that argument count is no more than two
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
    // Initiate fork
    int pid = fork();
    char* tokens[CHAR_LIMIT] = {NULL};
    char* token = strtok(command, " \t");

    // Tokenize and remove trailing new-line for easier comparision
    int i = 0;
    while (token != NULL && i < CHAR_LIMIT - 1) {
        token[strcspn(token, NEW_LINE)] = NULL_CHAR;
        tokens[i++] = token;
        token = strtok(NULL, " \t");
    }

    // Null-termination
    if (strcmp(tokens[i - 1], "&") == 0) {
        // Run in background and replace ampersand with \0
        tokens[i - 1] = NULL;
        background = 1;
    } else {
        // Wait to finish running and add \0
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