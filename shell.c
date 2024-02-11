/**
 * @file shell.c
 * @brief A command-line shell program
 *
 * A command-line shell program that uses system calls to directly invoke
 * operating system services, rather than relying on C library calls
 *
 * @author Brody Lee (blee20@georgefox.edu)
 */


#include "shell.h"
#include "shellcmd.h"


#define CLEAR 0
#define EQUAL 0
#define SUCCESS 0
#define CHAR_LIMIT 255
#define EXIT "exit\n"


/**
 * @brief Main entry point of the shell.
 *
 * @return The exit status of the shell.
 */
int main() {
    // 255 characters is the input limit
    char buffer[CHAR_LIMIT];
    char cwd[CHAR_LIMIT];
    int prev_exit_code = -1;

    while (strcmp(buffer, EXIT) != EQUAL) {
        // Clear the buffer
        memset(buffer, CLEAR, sizeof(buffer));

        // If error occurs and null is returned, print last error and exit
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }

        // Display current working directory
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, " $ ", strlen(" $ "));

        // Get input from user and store in buffer
        read(STDIN_FILENO, buffer, CHAR_LIMIT);

        // Thought about switch statement, but what if we have built-in's sharing the same letter?
        if (strncmp(buffer, "echo", strlen("echo")) == EQUAL) {
            // Echo command call
            prev_exit_code = shell_cmd_echo(prev_exit_code,buffer);
        } else if (strncmp(buffer, "cd", strlen("cd")) == EQUAL) {
            // Change directory command call
            prev_exit_code = shell_cmd_chdir(buffer);
        } else if (strncmp(buffer, "pwd", strlen("pwd")) == EQUAL) {
            // Print working directory command call
            prev_exit_code = shell_cmd_pwd(cwd);
        } else if (strcmp(buffer, EXIT) == EQUAL) {
            // Exit process
            exit(SUCCESS);
        } else {
            // Fork/execute non-implemented commands
            shell_cmd_exec(0, buffer);
        }

        // Sync standard input to eliminate any weird discrepancies
        fsync(STDOUT_FILENO);
    }
}
