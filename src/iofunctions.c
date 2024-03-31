#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cifras.h"

#define MAX_BUFFER 100
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

char **loadFile(FILE *input_stream, int *lineCounter) {
    char buffer[MAX_BUFFER]; //capacidade máxima de cada linha
    *lineCounter = 0;
    int capacity = INITIAL_CAPACITY; //número de linhas inicial

    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for lines.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, MAX_BUFFER, input_stream) != NULL) {
        if (*lineCounter == capacity) { //se lineCounter for igual a capacity, quer dizer que necessitamos de mais linhas
            capacity *= GROWTH_FACTOR; //assim, duplicamos o número de linhas usando realloc
            char **tmp = realloc(lines, capacity * sizeof(char *));
            if (tmp == NULL) {
                fprintf(stderr, "ERROR: Failed to reallocate memory.\n");
                for (int i = 0; i < *lineCounter; i++) {
                    free(lines[i]); // Em caso de erro liberta todas as linhas
                }
                free(lines); // Em caso de erro liberta o vetor
                exit(EXIT_FAILURE);
            }
            lines = tmp;
        }

        // durante o funcionamento normal é alocada a memória necessária a lines[*linecounter] através de strdup 
        //e copiado o texto de buffer para a linha correspondente
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