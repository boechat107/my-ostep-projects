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

int main(int argc, char *argv[argc + 1]) {
    if (is_batch_mode(argc)) {
        printf("batch mode");
    } else if (is_interactive_mode(argc)) {
        printf("interactive mode");
    } else {
        print_error();
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
