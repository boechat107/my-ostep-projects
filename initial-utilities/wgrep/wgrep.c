#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void match_print(char *word, FILE *stream) {
    size_t _line_len = 0;
    char *line = NULL;
    ssize_t nread;
    // An indexer of "word" that indicates which is the next character to
    // match.
    ssize_t match_idx = 0;
    ssize_t word_len = strlen(word);
    // TODO: implement my own "getline" and use c99 again.
    while((nread = getline(&line, &_line_len, stream)) != -1) {
        // We don't need to bother if the line length is smaller than the word
        // we are looking for.
        if (nread < word_len)
            continue;
        for (ssize_t i = 0; i < nread; i++) {
            if (line[i] == word[match_idx]) {
                match_idx++;
            } else {
                match_idx = 0;
            }
            if (match_idx == word_len) {
                printf("%s", line);
                break;
            }
        }
    }
    free(line);
}

int main(int argc, char *argv[argc+1]) {
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return EXIT_FAILURE;
    }
    // The word we are looking for.
    char *word = argv[1];
    // If there is no input file, we should read from STDIN.
    if (argc == 2) {
        match_print(word, stdin);
    } else {
        FILE *fp;
        for (int i = 2; i < argc; i++) {
            fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("wgrep: cannot open file\n");
                return EXIT_FAILURE;
            }
            match_print(word, fp);
            fclose(fp);
        }
    }
    return EXIT_SUCCESS;
}
