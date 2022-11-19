#ifndef MY_GETLINE
#define MY_GETLINE

#include <stdio.h>

// Arbitrary number.
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
int my_getline(char *[], FILE *);

#endif
