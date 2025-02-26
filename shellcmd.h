/**
 * @file shell_cmd.h
 * @brief Function prototypes for commands
 *
 * @author Brody Lee (blee20@georgefox.edu)
 */


#ifndef SHELL_CMD_H
#define SHELL_CMD_H


/**
 * Prints user-input to standard output
 *
 * @param prev_exit_code Exit code from previous process
 * @param command Command with args to execute
 * @return Zero on success, else non-zero on failure
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
 * Forks and executes commands not in built-in shell commands
 *
 * @param background Boolean for whether to run in background or not
 * @param command Command with args to execute
 * @return Zero on success, else non-zero on failure
 */
int shell_cmd_exec(int background, char* command);

#endif /* SHELL_CMD_H */