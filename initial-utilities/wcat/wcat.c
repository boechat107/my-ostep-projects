#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100


/**
   Reads and prints the contents of the file found by the given file path.
 **/
int read_print(char file_path[]) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        return 1;
    } else {
        char buffer[BUFFER_SIZE] = {0};
        char *ret = buffer;
        // We read the file line by line or chunks of the size "BUFFER_SIZE".
        while (ret != NULL) {
            ret = fgets(buffer, BUFFER_SIZE, fp);
            // If we reach the end of the file, we shouldn't print anything.
            if (ret != NULL)
                printf("%s", buffer);
        }
    }
    fclose(fp);
    return 0;
}


int main(int argc, char *argv[argc+1]) {
    if (argc < 2)
        return EXIT_SUCCESS;  // From "stdlib.h".

    int ret = 0;
    for (int i = 1; i < argc; i++) {
        ret = read_print(argv[i]);
        if (ret != 0) {
            printf("wcat: cannot open file\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
