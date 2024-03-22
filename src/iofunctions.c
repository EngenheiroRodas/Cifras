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
    int numLines = 0;
    int maxLines = INITIAL_CAPACITY;

    // Allocate memory for the initial capacity
    char **lines = (char **)malloc(maxLines * sizeof(char *)); //sizeof(char *), array de apontadores(strings)
    if (lines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(input_stream);
        exit(EXIT_FAILURE);
    }

    // Read each line of the file
    while (fgets(buffer, sizeof(buffer), input_stream) != NULL) {
        // Check if reallocation is needed
        if (numLines >= maxLines) {
            maxLines *= GROWTH_FACTOR; // Increase capacity using growth factor
            //realloc para tmp, evitando perder dados
            char **tmp = realloc(lines, maxLines * sizeof(char *)); 
            if (tmp == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                // Free previously allocated memory
                for (int i = 0; i < numLines; i++) {
                    free(lines[i]);
                }
                free(lines);
                fclose(input_stream);
                exit(EXIT_FAILURE);
            }
            lines = tmp; //devolver as o novo espaço com mais linhas a lines
        }

        // Allocate memory for the string and copy the content
        lines[numLines] = strdup(buffer);
        if (lines[numLines] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            // Free previously allocated memory
            for (int i = 0; i < numLines; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(input_stream);
            exit(EXIT_FAILURE);
        }
        numLines++;
    }

    *lineCounter = numLines;

    fclose(input_stream);
    return lines;
}

void writeOutput(FILE *output_stream, char *lines[], int lineCounter) {
    for (int i = 0; i < lineCounter; i++) {
        fprintf(output_stream, "%s", lines[i]);
        free(lines[i]);
    }

    for (int i = 0; i < lineCounter; i++) {
            free(lines[i]);
        }
    free(lines);

    return;
}