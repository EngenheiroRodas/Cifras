#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cifras.h"

#define MAX_BUFFER 100
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

//diz-se qual é a stream de entrada e carrega para a memória dinamicamente para array de strings "lines"
char **loadFile(FILE *input_stream, int *lineCounter) {
    char buffer[MAX_BUFFER];
    *lineCounter = 0;
    int capacity = INITIAL_CAPACITY;

    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for lines.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, MAX_BUFFER, input_stream) != NULL) {
        if (*lineCounter == capacity) {
            capacity *= GROWTH_FACTOR;
            char **tmp = realloc(lines, capacity * sizeof(char *));
            if (tmp == NULL) {
                fprintf(stderr, "ERROR: Failed to reallocate memory.\n");
                for (int i = 0; i < *lineCounter; i++) {
                    free(lines[i]); // Free individual lines
                }
                free(lines); // Free the lines array
                exit(EXIT_FAILURE);
            }
            lines = tmp;
        }

        lines[*lineCounter] = strdup(buffer);
        if (lines[*lineCounter] == NULL) {
            fprintf(stderr, "ERROR: Memory allocation failed for lines.\n");
            for (int i = 0; i < *lineCounter; i++) {
                free(lines[i]);
            }
            free(lines);
            exit(EXIT_FAILURE);
        }
        (*lineCounter)++;
    }
    return lines;
}

void freeLines(char *lines[], int *lineCounter) {
    for (int i = 0; i < *lineCounter; i++) {
        free(lines[i]);
    }
    free(lines);

    return;
}