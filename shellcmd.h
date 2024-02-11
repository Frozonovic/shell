/**
 * @file shell_cmd.h
 * @brief Function prototypes for commands
 *
 * @author Brody Lee (blee20@georgefox.edu)
 */


#ifndef SHELL_CMD_H
#define SHELL_CMD_H


/**
 * Prints all following arguments to standard output
 * @param command
 * @return
 */
int shell_cmd_echo(int prev_exit_code, char* command);


/**
 * Prints the current working directory to standard output
 *
 * @param cwd Current working directory array
 * @return Zero on success, else non-zero on failure
 */
int shell_cmd_pwd(char* cwd);


/**
 * Changes the current working directory
 *
 * @param path Directory to change to
 * @return Zero on success, else non-zero on failure
 */
int shell_cmd_chdir(char* path);


/**
 * Fork and execute a command not built into shell
 *
 * @param command Command "string" to fork and execute
 * @return Zero on success, else non-zero on failure
 */
int shell_cmd_exec(int background, char* command);

#endif /* SHELL_CMD_H */