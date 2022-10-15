#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000
#define RLE_CNT_NBYTES 4
#define RLE_NBYTES RLE_CNT_NBYTES + 1

/**
   Converts the number "n" into an array of bytes.

   "size" is the length of the given array.
 **/
void to_bytes(unsigned char *bytes, size_t size, unsigned long n) {
    // In each iteration of the loop, we shift 1 byte to the right and we zero
    // all bytes on the left ("255" makes only the least significant byte
    // contain 1s).
    //
    // Example using 3 bytes (A, B and C):
    //
    // A - B - C
    // 0:  0 - 0 - C  (n >> 0) & 255
    // 1:  0 - 0 - B  (n >> 8) & 255
    // 2:  0 - 0 - A  (n >> 16) & 255
    for (size_t i = 0; i < size; i++) {
        bytes[size - (i + 1)] = (n >> (8 * i)) & 255;
    }
}


void encode_print_RLE(FILE *stream) {
    // Reading buffer. We read files in parts.
    char buffer[BUFFER_SIZE] = {0};
    // The first RLE_CNT_NBYTES bytes to count repetitions. Used for printing.
    unsigned char rle_cnt[RLE_NBYTES] = {0};
    // Used only to get the result of "fgets".
    char *fgets_ret = buffer;
    // The last character we counted. It changes when we find a new character.
    char last_char = 0;
    // The counter of each input character. It should represent the character
    // in "last_char".
    unsigned long char_cnt = 0;
    // We read the file in chunks.
    while((fgets_ret = fgets(buffer, BUFFER_SIZE, stream))) {
        // For each chunk, we iterate on each character.
        for (size_t i = 0; i < BUFFER_SIZE; i++) {
            if (buffer[i] == last_char) {
                char_cnt++;
            } else if (buffer[i] == '\0') {
                // We shouldn't count end of string characters.
                break;
            } else {
                // We only need to check this because of the first character
                // (worse for big files).
                // An alternative would be initializing by reading a single
                // character first (worse for small files).
                if (char_cnt > 0) {
                    // We build the 5-byte entries (4 for the counter, 1 for
                    // the character).
                    to_bytes(rle_cnt, RLE_CNT_NBYTES, char_cnt);
                    rle_cnt[RLE_CNT_NBYTES] = last_char;
                    // We write the entry to STDOUT in binary.
                    fwrite(rle_cnt, 1, RLE_NBYTES, stdout);
                }
                last_char = buffer[i];
                char_cnt = 1;
            }
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
