#include <stdlib.h>

int main(int argc, char *argv[argc+1]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        return EXIT_FAILURE;
    }

    FILE *fp;
    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            return EXIT_FAILURE;
        }
        encode_print_RLE(fp);
        fclose(fp);
    }
    return EXIT_SUCCESS;
}
