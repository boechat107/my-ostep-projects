#include "my_getline.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

void enter_interactive_mode() {
    char *cmd_input = NULL;
    ssize_t input_size = 0;
    while (true) {
        printf("wish> ");
        input_size = my_getline(&cmd_input, stdin);
        // -1 means an error or EOF. We stop the shell in these situations.
        if (input_size == -1)
            return;
        // TODO: parse the shell command exit with "strsep".
    }
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
