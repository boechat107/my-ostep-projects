#include <stdio.h>
#include <stdlib.h>

// Increase this number
#define BUFFER_SIZE 10
#define RLE_CNT_NBYTES 4

void to_bytes(unsigned char bytes[RLE_CNT_NBYTES], unsigned long n) {
    for (size_t i = 0; i < RLE_CNT_NBYTES; i++) {
        bytes[RLE_CNT_NBYTES - (i + 1)] = (n >> (8 * i)) & 255;
    }
}


void encode_print_RLE(FILE *stream) {
    char buffer[BUFFER_SIZE] = {0};
    unsigned char rle_cnt[RLE_CNT_NBYTES + 1] = {0};
    char *fgets_ret = buffer;
    unsigned long char_cnt = 0;
    char last_char = 0;
    while((fgets_ret = fgets(buffer, BUFFER_SIZE, stream))) {
        for (size_t i = 0; i < BUFFER_SIZE; i++) {
            if (buffer[i] == last_char) {
                char_cnt++;
            } else if (buffer[i] == '\0') {
                break;
            } else {
                // We only need to check this because of the first character
                // (worse for big files).
                // An alternative would be initializing by reading a single
                // character first (worse for small files).
                if (char_cnt > 0) {
                    to_bytes(rle_cnt, char_cnt);
                    rle_cnt[RLE_CNT_NBYTES] = last_char;
                    fwrite(rle_cnt, 1, 5, stdout);
                }
                last_char = buffer[i];
                char_cnt = 1;
            }
            if (buffer[i] == '\n')
                break;
        }
    }
}


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
