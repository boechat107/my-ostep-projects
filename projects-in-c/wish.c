#include "my_getline.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: increase this number after testing.
// This constant defines the amount of memory (number of positions in the array
// of strings) we allocate to store the parsed tokens of the command-line.
#define EXEC_ARG_STEP_SIZE 2

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
            // printf("realloc\n");
        }
        (*parsed_cmd)[i] = my_strdup(token);
        // printf("token %zu: %s\n", i, (*parsed_cmd)[i]);
    }
    // for (size_t j = 0; j < i; j++)
    //     printf("%s ", (*parsed_cmd)[j]);
    // printf("\n");
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

void enter_interactive_mode() {
    // We use it to store the inputted command-line string.
    char *cmd_input = NULL;
    // This pointer will point to an array of strings, which one representing a
    // parsed command-line token.
    char **parsed_cmd = NULL;
    ssize_t input_size = 0;
    size_t cmd_len = 0;
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
            // TODO: check if execvp frees this memory.
            cmd_len = parse_cmd(cmd_input, &parsed_cmd);
            // printf("orig: %s\n", cmd_input);
            // for (size_t i = 0; i < cmd_len; i++)
            //     printf("p token %zu: %s\n", i, parsed_cmd[i]);
            if (cmd_len > 0 && strcmp(parsed_cmd[0], "exit") == 0)
                break;
        }
    }
    free(cmd_input);
    free_parsed_cmd(cmd_len, &parsed_cmd);
}

int main(int argc, char *argv[argc + 1]) {
    if (is_batch_mode(argc)) {
        printf("batch mode");
    } else if (is_interactive_mode(argc)) {
        enter_interactive_mode();
    } else {
        print_error();
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
