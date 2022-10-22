#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MALLOC_SIZE 200

/**
   Reads an entire line from "stream".

   This function is a simpler (and more inefficient) implementation of GNU
   "getline". Different than "getline", "my_getline" doesn't reuse previously
   allocated memory. If "lineptr" is not NULL, the pointed memory space is
   freed before new allocation happens.

   "my_getline", similarly to "getline", returns the number of characters
   read. In case of failure or nothing read because of EOF, the function
   returns -1.
*/
int my_getline(char *lineptr[], FILE *stream) {
    // Counts the number of memory allocations. Starts from 1 because we always
    // allocate at least once.
    size_t cnt_alloc = 1;
    // A reading index; keeps track of characters read.
    size_t read_idx = 0;
    // One specific char indexed from "lineptr". The initial value is useless.
    char current = 0;
    // To keep things simple, we don't reuse previously allocated memory for
    // "lineptr".
    if (*lineptr != NULL)
        free(*lineptr);
    *lineptr = malloc(sizeof(char[MALLOC_SIZE]));
    // We keep reading characters from "stream" until EOF or a line break.
    while (fgets((*lineptr) + read_idx, MALLOC_SIZE, stream) != NULL) {
        // "cnt_alloc * MALLOC_SIZE" is the total amount of memory currently
        // allocated.
        for (read_idx = read_idx; read_idx < (cnt_alloc * MALLOC_SIZE);
             read_idx++) {
            current = (*lineptr)[read_idx];
            // If we find a terminating null byte before a line break, we need
            // to allocate more memory to read the entire line.
            if (current == '\0') {
                break;
            } else if (current == '\n') {
                return read_idx + 1;
            }
        }
        // We allocate more memory and increase the counter of allocations.
        cnt_alloc++;
        *lineptr = realloc(*lineptr, sizeof(char[MALLOC_SIZE * cnt_alloc]));
    }
    // If we didn't read anything, we must return -1.
    if (read_idx > 0)
        return read_idx + 1;
    else
        return -1;
}

void match_print(char *word, FILE *stream) {
    char *line = NULL;
    ssize_t nread;
    // An indexer of "word" that indicates which is the next character to
    // match.
    ssize_t match_idx = 0;
    ssize_t word_len = strlen(word);
    while ((nread = my_getline(&line, stream)) != -1) {
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
    // Freeing memory allocated in the last loop iteration.
    free(line);
}

int main(int argc, char *argv[argc + 1]) {
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
