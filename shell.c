/**
 * @file shell.c
 * @brief A command-line shell program.
 *
 * A command-line shell program that uses system calls to directly invoke
 * operating system services, rather than relying on C library calls.
 *
 * @author blee20\@georgefox.edu
 */

#include "shell.h"

/**
 * @brief Main entry point of the shell.
 *
 * @return The exit status of the shell.
 */
char main() {
    // Good ol' constants
    const int SUCCESS = 0;
    const int CHAR_LIMIT = 255;
    const char* EXIT = "exit\n";

    // 255 characters is the input limit
    char buffer[CHAR_LIMIT];

    while (strncasecmp(buffer, EXIT, strlen(EXIT)) != 0) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // Prompt user for input via print to standard output
        syscall(SYS_write, STDOUT_FILENO, "shell$", 6);

        // Read user input from standard output and store in buffer
        syscall(SYS_read, STDOUT_FILENO, buffer, CHAR_LIMIT);

        // Called in case of sync issues
        syscall(SYS_fsync, STDOUT_FILENO);
    }

    return SUCCESS;
}
