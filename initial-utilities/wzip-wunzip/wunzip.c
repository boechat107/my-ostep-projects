#include <stdio.h>
#include <stdlib.h>

#define RLE_CNT_NBYTES 4
#define RLE_NBYTES RLE_CNT_NBYTES + 1


unsigned long to_long(unsigned char *bytes, size_t size) {
    unsigned long retval = 0;
    for (size_t i = 0; i < size; i++) {
        retval <<= 8;
        retval += bytes[i];
    }
    return retval;
}


void decode_print_RLE(FILE *stream) {
    unsigned char rle_entry[RLE_NBYTES] = {0};
    size_t fread_ret = RLE_NBYTES;
    unsigned long char_cnt = 0;

    while((fread_ret = fread(rle_entry, 1, RLE_NBYTES, stream)) == RLE_NBYTES) {
        char_cnt = to_long(rle_entry, RLE_CNT_NBYTES);
        for (size_t i = 0; i < char_cnt; i++) {
            printf("%c", rle_entry[RLE_CNT_NBYTES]);
        }
    }
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
