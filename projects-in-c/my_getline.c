#include "my_getline.h"
#include <stdio.h>
#include <stdlib.h>

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
