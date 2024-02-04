/**
 * @file shell.h
 * @brief Function prototypes for the shell.
 *
 * @author Brody Lee (blee20\@georgefox.edu)
 */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>


#ifndef SHELL_H
#define SHELL_H

// TODO add function prototypes

#endif /* SHELL_H */

/**
 * Fork and execute a command not built into shell
 *
 * @param command Command "string" to fork and execute
 */
void executeCommand(char* command);
