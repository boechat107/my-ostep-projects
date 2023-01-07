#include "my_getline.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// TODO: increase this number after testing.
// This constant defines the amount of memory (number of positions in the array
// of strings) we allocate to store the parsed tokens of the command-line.
#define EXEC_ARG_STEP_SIZE 2
// Default user path
#define DEFAULT_USER_PATH "/bin"

// Array of strings to store user defined paths.
// WARNING: they are mutated only by the function "set_user_paths".
char **_user_paths_ = NULL;
size_t _user_paths_len_ = 0;

void print_error() { fprintf(stderr, "An error has occurred\n"); }

_Bool is_batch_mode(int argc) {
    if (argc == 2)
        return true;
    else
        return false;
}

_Bool is_interactive_mode(int argc) {
    if (argc == 1)
        return true;
    else
        return false;
}

/**
   An equivalent implementation of GNU's strdup.
*/
char *my_strdup(char *str) {
    char *output = NULL;
    size_t str_len = strlen(str);
    output = malloc(sizeof(char) * (str_len + 1));
    for (size_t i = 0; i < str_len; i++) {
        output[i] = str[i];
    }
    output[str_len] = '\0';
    return output;
}

/**
   Parses the command-line string and stores the tokens in "parsed_cmd",
   returning the number of parsed tokens.

   Note that the memory allocation for the parsed tokens happens inside the
   function, and we need to free it later with "free_parsed_cmd".
*/
size_t parse_cmd(char *cmd_input, char **parsed_cmd[]) {
    char *str_input, *token = NULL;
    size_t i = 0;
    size_t parsed_cmd_size = 0;
    *parsed_cmd = malloc(sizeof(char*) * EXEC_ARG_STEP_SIZE);
    parsed_cmd_size = EXEC_ARG_STEP_SIZE;
    // I use "strtok" because "strsep" is not available for c99.
    for (i = 0, str_input = cmd_input;; i++, str_input = NULL) {
        token = strtok(str_input, " \n");
        // If "token" is "NULL", we don't have anything else to parse from the
        // command-line string.
        if (token == NULL)
            break;
        // We need to check if we can store the number of tokens we are
        // parsing. We may need to increase the size of the tokens array.
        if (i >= parsed_cmd_size) {
            parsed_cmd_size += EXEC_ARG_STEP_SIZE;
            *parsed_cmd = realloc(*parsed_cmd,
                                 sizeof(char*) * parsed_cmd_size);
        }
        (*parsed_cmd)[i] = my_strdup(token);
    }
    return i;
}

/**
   Helper function to free the array strings used to store the parsed tokens of
   commands.
*/
void free_parsed_cmd(size_t cmd_len, char **parsed_cmd[]){
    // The number of tokens is not the same as the size of the array.
    if (*parsed_cmd != NULL) {
        for (size_t i = 0; i < cmd_len; i++)
            free((*parsed_cmd)[i]);
        free(*parsed_cmd);
    }
}

/**
   Returns true if both strings are the same.
*/
_Bool equal_str(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}

/**
   Executes built-in commands if one is given. If a built-in command is
   executed, returns true.
*/
_Bool exec_builtin_cmds_or_ignore(size_t cmd_len, char *parsed_cmd[]) {
    // ======= exit =======
    if (equal_str(parsed_cmd[0], "exit")) {
        // This command doesn't accept any argument.
        if (cmd_len > 1)
            print_error();
        else
            exit(EXIT_SUCCESS);
        return true;
    }
    // ====== cd ==========
    else if (equal_str(parsed_cmd[0], "cd")) {
        // This command accepts only 1 argument.
        if (!(cmd_len == 2))
            print_error();
        else {
            printf("built-in command cd\n");
        }
        return true;
    }
    // ======= path =======
    else if (equal_str(parsed_cmd[0], "path")) {
        printf("built-in command path\n");
        return true;
    }
    // If no built-in command is detected, nothing happens.
    return false;
}

/**
   Sets the user paths the shell uses to execute programs.

   This function is the only place where "_user_paths_" and "_user_paths_len_"
   are mutated.
*/
void set_user_paths(size_t n_paths, char *paths[]) {
    if (_user_paths_len_ > 0) {
        free_parsed_cmd(_user_paths_len_, &_user_paths_);
        _user_paths_len_ = 0;
    }
    _user_paths_ = malloc(sizeof(char*) * n_paths);
    for (size_t i = 0; i < n_paths; i++) {
        _user_paths_[i] = my_strdup(paths[0]);
    }
    _user_paths_len_ = n_paths;
}

/**
   Initialize the user paths with default values ("/bin" only).
*/
void init_user_path() {
    char *default_path[1] = { DEFAULT_USER_PATH };
    set_user_paths(1, default_path);
}

/**
   Returns a copy of "parsed_cmd", but includes a NULL at the end of the array.

   This is the format expected for the execv family.
*/
char **mk_execv_args(size_t cmd_len, char *parsed_cmd[]) {
    char **args = malloc(sizeof(char*) * (cmd_len + 1));
    for (size_t i = 0; i < cmd_len; i++)
        args[i] = parsed_cmd[i];
    args[cmd_len] = NULL;
    return args;
}

/**
   Executes programs in forked processes.
*/
void exec_path_commands(size_t cmd_len, char *parsed_cmd[]) {
    int pid = fork();
    if (pid < 0)
        print_error();
    // If this condition is true, it is the child process running.
    else if (pid == 0) {
        char **exec_args = mk_execv_args(cmd_len, parsed_cmd);
        execvp(exec_args[0], exec_args);
        exit(EXIT_SUCCESS);
    } else {
        // Parent code running.
        // We wait for the forked process to finish.
        waitpid(pid, NULL, 0);
    }
}

void enter_interactive_mode() {
    // We use it to store the inputted command-line string.
    char *cmd_input = NULL;
    // This pointer will point to an array of strings, which one representing a
    // parsed command-line token.
    char **parsed_cmd = NULL;
    // Stores the size of the raw input string.
    ssize_t input_size = 0;
    // Stores the number of tokens of the parsed input.
    size_t cmd_len = 0;
    // This variable will be true if the input is a built-in command.
    _Bool is_built_cmd = false;
    while (true) {
        printf("wish> ");
        input_size = my_getline(&cmd_input, stdin);
        // -1 means an error or EOF. We stop the shell in these situations.
        if (input_size == -1)
            break;
        else if (input_size == 0)
            continue;
        else {
            // "parse_cmd" allocates new memory and uses "parsed_cmd" to point
            // to it.
            free_parsed_cmd(cmd_len, &parsed_cmd);
            cmd_len = parse_cmd(cmd_input, &parsed_cmd);
            // This usually happens when the input is empty.
            if (cmd_len == 0) {
                continue;
            }
            // We check if the input contains any built-in command and execute
            // it. If none is detected, we try to execute binaries in the path.
            is_built_cmd = exec_builtin_cmds_or_ignore(cmd_len, parsed_cmd);
            if (!is_built_cmd)
                exec_path_commands(cmd_len, parsed_cmd);
        }
    }
    free(cmd_input);
    free_parsed_cmd(cmd_len, &parsed_cmd);
}

int main(int argc, char *argv[argc + 1]) {
    // We initialize the user path with default values.
    init_user_path();
    if (is_batch_mode(argc)) {
        printf("batch mode: %s", argv[1]);
    } else if (is_interactive_mode(argc)) {
        enter_interactive_mode();
    } else {
        print_error();
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
