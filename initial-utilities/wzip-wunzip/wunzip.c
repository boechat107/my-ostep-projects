#include <stdio.h>
#include <stdlib.h>

void decode_print_RLE(FILE *stream) {

}

int main(int argc, char *argv[argc+1]) {
    if (argc < 2) {
        printf("unwzip: file1 [file2 ...]\n");
        return EXIT_FAILURE;
    }

    FILE *fp;
    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("unwzip: cannot open file\n");
            return EXIT_FAILURE;
        }
        decode_print_RLE(fp);
        fclose(fp);
    }
    return EXIT_SUCCESS;
}
