#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cifras.h"

#define MAX_BUFFER 101
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

//diz-se qual é a stream de entrada e carrega para a memória dinamicamente para array de strings "lines"
char **loadFile(FILE *input_stream, int *lineCounter) {
    char buffer[MAX_BUFFER];
    *lineCounter = 0;
    int capacity = INITIAL_CAPACITY;

    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, MAX_BUFFER, input_stream) != NULL) {
        if (*lineCounter == capacity) {
            capacity *= GROWTH_FACTOR;
            char **tmp = realloc(lines, capacity * sizeof(char *));
            if (!tmp) {
                free(lines); // Simplified memory cleanup
                fprintf(stderr, "Failed to reallocate memory.\n");
                exit(EXIT_FAILURE);
            }
            lines = tmp;
        }

        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character if it exists
        lines[*lineCounter] = strdup(buffer);
        if (!lines[*lineCounter]) {
            // Simplified error handling
            fprintf(stderr, "Memory allocation for lines failed.\n");
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