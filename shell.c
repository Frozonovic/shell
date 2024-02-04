/**
 * @file shell.c
 * @brief A command-line shell program.
 *
 * A command-line shell program that uses system calls to directly invoke
 * operating system services, rather than relying on C library calls.
 *
 * @author Brody Lee (blee20\@georgefox.edu)
 */


#include "shell.h"

/**
 * @brief Main entry point of the shell.
 *
 * @return The exit status of the shell.
 */
int main() {
    // Good ol' constants
    const int SUCCESS = 0;
    const int CHAR_LIMIT = 255;
    const char* EXIT = "exit\n";

    // 255 characters is the input limit
    char buffer[CHAR_LIMIT];
    char cwd[CHAR_LIMIT];

    while (strcmp(buffer, EXIT) != 0) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // If error occurs and null is returned, print last error and exit
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }

        // Display current working directory
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, " $ ", 2);

        // Get input from user and store in buffer
        read(STDIN_FILENO, buffer, CHAR_LIMIT);

        // Thought about switch statement, but what if we have built-in's sharing the same letter?
        if (strcmp(buffer, "cd\n") == 0) {
            // Directory omitted, change to HOME
            chdir(getenv("HOME"));
        } else if (strncmp(buffer, "cd ", 3) == 0) {
            // Change to specified directory
            char* directory = buffer + 3;
            chdir(directory);
        } else if (strcmp(buffer, "pwd\n") == 0) {
            // Display the current working directory
            write(STDOUT_FILENO, cwd, strlen(cwd));
        } else if (strcmp(buffer, EXIT) == 0) {
            break;
        } else {
            // Fork and exec command
            executeCommand(buffer);
        }

        // Sync standard input to eliminate any weird discrepancies
        fsync(STDOUT_FILENO);
    }

    return SUCCESS;
}


void executeCommand(char* command) {
    int pid = fork();

    if (pid == 0) {
        // Child process
        char* token = strtok(command, " \t");
        execvp(token, &token);

        // If failure occurs, print last error and exit
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        waitpid(pid, NULL, 0);
    } else {
        // If error occurs, print the last error that occurred
        perror("fork");
        exit(EXIT_FAILURE);
    }
}
