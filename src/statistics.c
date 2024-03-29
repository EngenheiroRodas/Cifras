#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cifras.h"

#define TABLE_SIZE 67

double *statCalculator(FILE *input_stream, unsigned int *regularChar, unsigned int *weirdChar, unsigned int *temp, int chunkSize) {
    char **lines = NULL;
    int lineCounter = 0;
    lines = loadFile(input_stream, &lineCounter);

    double *frequency = (double *)malloc((TABLE_SIZE + 1) * sizeof(double));
    if (frequency == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for character statistics.\n");
        return NULL;
    }
    
    for (int i = 0; i < lineCounter; i++) {
        int length = strlen(lines[i]);
        for (int j = 0; j < length; j += chunkSize) {
            // Process each character within the chunk, ensuring not to exceed string bounds
            if (getIndex(lines[i][j]) != -1) {
                temp[getIndex(lines[i][j])]++;
                (*regularChar)++;
            } else {
                (*weirdChar)++;
            }
        }
        // Account for the newline character at the end of each line
        (*weirdChar)++;
        if () {
            
        }
    }
    freeLines(lines, &lineCounter);

    // Calculate statistics 
    for (int i = 0; i < TABLE_SIZE; i++) {
        frequency[i] = *regularChar ? (double)temp[i] / *regularChar : 0;
    }
    frequency[TABLE_SIZE] = (*regularChar + *weirdChar) ? (double)*weirdChar / (*regularChar + *weirdChar) : 0;
    
    return frequency;
}