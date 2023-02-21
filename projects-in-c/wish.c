#include "my_getline.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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

void print_str_array(size_t len, char *str_array[]) {
    for (size_t i = 0; i < len; i++)
        printf("String %zu: %s\n", i, str_array[i]);
}

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
   Concatenates strings to form a complete file path.

   The function adds a "/" between "s1" and "s2" if necessary.
*/
char *concat_path_prog(char *s1, char *s2) {
    size_t s1_len = strlen(s1);
    size_t s1_req_extra = 0;
    // We check if the base path contains an ending "/". If it doesn't contain,
    // we need to count an extra character that we will add later.
    if (s1[s1_len - 1] != '/')
        s1_req_extra = 1;
    size_t s2_len = strlen(s2);
    size_t new_str_len = s1_len + s1_req_extra + s2_len;
    // +1 to include the terminating null byte "\0".
    char *new_str = malloc(sizeof(char) * (new_str_len + 1));
    new_str[0] = '\0';
    new_str = strcat(new_str, s1);
    // If the last "s1" character is not "/", we need to add this extra
    // character.
    if (s1_req_extra > 0) {
        new_str[s1_len] = '/';
        new_str[s1_len + 1] = '\0';
    }
    new_str = strcat(new_str, s2);
    return new_str;
}

/**
   Parses the command-line string and stores the tokens in "parsed_cmd",
   returning the number of parsed tokens.

   Note that the memory allocation for the parsed tokens happens inside the
   function, and we need to free it later with "free_array_of_string".
*/
size_t parse_cmd(char *cmd_input, char **parsed_cmd[]) {
    char *str_input, *token = NULL;
    size_t i = 0;
    size_t parsed_cmd_size = 0;
    // "strtok" mutates its argument; we use a copy to avoid unexpected
    // side-effects.
    char *cmd_input_copy = my_strdup(cmd_input);
    *parsed_cmd = malloc(sizeof(char *) * EXEC_ARG_STEP_SIZE);
    parsed_cmd_size = EXEC_ARG_STEP_SIZE;
    // I use "strtok" because "strsep" is not available for c99.
    for (i = 0, str_input = cmd_input_copy;; i++, str_input = NULL) {
        token = strtok(str_input, " \n");
        // If "token" is "NULL", we don't have anything else to parse from the
        // command-line string.
        if (token == NULL)
            break;
        // We need to check if we can store the number of tokens we are
        // parsing. We may need to increase the size of the tokens array.
        if (i >= parsed_cmd_size) {
            parsed_cmd_size += EXEC_ARG_STEP_SIZE;
            *parsed_cmd =
                realloc(*parsed_cmd, sizeof(char *) * parsed_cmd_size);
        }
        (*parsed_cmd)[i] = my_strdup(token);
    }
    free(cmd_input_copy);
    return i;
}

/**
   Helper function to free the memory allocated for an array strings.
*/
void free_array_of_string(size_t len, char **str_array[]) {
    // The number of tokens is not the same as the size of the array.
    if (*str_array != NULL) {
        for (size_t i = 0; i < len; i++)
            free((*str_array)[i]);
        free(*str_array);
    }
}

/**
   Returns true if both strings are the same.
*/
_Bool equal_str(const char *s1, const char *s2) { return strcmp(s1, s2) == 0; }

/**
   Sets the user paths the shell uses to execute programs.

   This function is the only place where "_user_paths_" and "_user_paths_len_"
   are mutated.
*/
void set_user_paths(size_t n_paths, char *paths[]) {
    // We need to free existing allocated memory before setting new paths.
    if (_user_paths_len_ > 0) {
        free_array_of_string(_user_paths_len_, &_user_paths_);
        _user_paths_len_ = 0;
    }
    // The user can set an empty path and the shell would execute only built-in
    // commands.
    if (n_paths == 0) {
        _user_paths_ = NULL;
        _user_paths_len_ = 0;
    } else {
        _user_paths_len_ = n_paths;
        // We copy the given paths to a new freshly allocated memory.
        _user_paths_ = malloc(sizeof(char *) * n_paths);
        for (size_t i = 0; i < n_paths; i++) {
            _user_paths_[i] = my_strdup(paths[i]);
        }
    }
}

/**
   Executes built-in commands if one is given. If a built-in command is
   executed, returns true.
*/
_Bool exec_builtin_cmds_or_ignore(size_t cmd_len, char *parsed_cmd[]) {
    // Note:
    // I don't like the need of adding a return statement for each built-in
    // command. I do it here as a first simple approach.
    // I believe that a better way would be to detect the built-in commands in
    // the caller function. We could have an integer argument informing which
    // one of the commands should be executed.
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
            int is_ok = chdir(parsed_cmd[1]);
            if (is_ok != 0)
                print_error();
        }
        return true;
    }
    // ======= path =======
    else if (equal_str(parsed_cmd[0], "path")) {
        set_user_paths(cmd_len - 1, parsed_cmd + 1);
        // print_str_array(_user_paths_len_, _user_paths_);
        return true;
    }
    // If no built-in command is detected, nothing happens.
    return false;
}

/**
   Initialize the user paths with default values ("/bin" only).
*/
void init_user_path() {
    char *default_path[1] = {DEFAULT_USER_PATH};
    set_user_paths(1, default_path);
}

/**
   Returns a copy of "parsed_cmd", but includes a NULL at the end of the array.

   This is the format expected for the execv family.
*/
char **mk_execv_args(size_t cmd_len, char *parsed_cmd[]) {
    char **args = malloc(sizeof(char *) * (cmd_len + 1));
    for (size_t i = 0; i < cmd_len; i++)
        args[i] = parsed_cmd[i];
    args[cmd_len] = NULL;
    return args;
}

/**
   Given a program name, returns its full path if we can find the program file
   in the user paths; otherwise, returns NULL.

   The caller of this function is responsible to free the memory allocated to
   build the full program path.
*/
char *find_path_or_error(char *prog_name) {
    for (size_t i = 0; i < _user_paths_len_; i++) {
        char *full_path = concat_path_prog(_user_paths_[i], prog_name);
        int found = access(full_path, X_OK);
        if (found != 0)
            free(full_path);
        else
            return full_path;
    }
    // If the function didn't return a path in the loop, we can't find the
    // program in the user paths.
    return NULL;
}

/**
   Executes programs in forked processes.
*/
void exec_path_commands(size_t cmd_len, char *parsed_cmd[]) {
    char *path = find_path_or_error(parsed_cmd[0]);
    // If we can't find a path containing the command-line program, it is
    // an error.
    if (path == NULL) {
        print_error();
        return;
    }
    int pid = fork();
    if (pid < 0)
        print_error();
    // If this condition is true, it is the child process running.
    else if (pid == 0) {
        char **exec_args = mk_execv_args(cmd_len, parsed_cmd);
        execvp(exec_args[0], exec_args);
    } else {
        // Parent code running.
        // We wait for the forked process to finish.
        waitpid(pid, NULL, 0);
    }
    // Only the parent process should reach this point. Or we got an error to
    // fork a process, or it succeeded.
    free(path);
}

void loop_for_commands(FILE *stream, _Bool is_batch) {
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
        if (!is_batch)
            printf("wish> ");
        input_size = my_getline(&cmd_input, stream);
        // -1 means an error or EOF. We stop the shell in these situations.
        if (input_size == -1)
            break;
        else if (input_size == 0)
            continue;
        else {
            // "parse_cmd" allocates new memory and uses "parsed_cmd" to point
            // to it.
            free_array_of_string(cmd_len, &parsed_cmd);
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
    free_array_of_string(cmd_len, &parsed_cmd);
}

int main(int argc, char *argv[argc + 1]) {
    // We initialize the user path with default values. This should give the
    // user the hability of calling basic programs like "ls".
    init_user_path();
    if (is_batch_mode(argc)) {
        FILE *fp = fopen(argv[argc - 1], "r");
        if (fp == NULL) {
            print_error();
            exit(EXIT_FAILURE);
        }
        loop_for_commands(fp, true);
        fclose(fp);
    } else if (is_interactive_mode(argc)) {
        loop_for_commands(stdin, false);
    } else {
        print_error();
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
