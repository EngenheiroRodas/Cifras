#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cifras.h"

#define TABLE_SIZE 67
#define NEEDED 1
#define NOT_NEEDED 0

double *statCalculator(FILE *input_stream, unsigned int *regularChar, unsigned int *weirdChar, unsigned int *temp, int chunkSize, int eflag) {
    char **lines = NULL;
    int lineCounter = 0;
    lines = loadFile(input_stream, &lineCounter);

    double *frequency = (double *)malloc((TABLE_SIZE + 1) * sizeof(double));
    if (frequency == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for character statistics.\n");
        return NULL;
    }

    if (eflag == 1) { //estatística
        for (int i = 0; i < lineCounter; i++) {
        unsigned int j = 0;
        while (lines[i][j] != '\0')
        {
            if (getIndex(lines[i][j]) != -1) {
                // Increment the count for recognized characters
                temp[getIndex(lines[i][j])]++;
                (*regularChar)++;
            } else {
                // Increment the count for unrecognized (weird) characters
                (*weirdChar)++;
            } 
            j++;
        }
        //contar com \n
        (*weirdChar)++;
        }
    } else { //ataque
        int aux = 0;
        int needTemp = NOT_NEEDED;
        for (int i = 0; i < lineCounter; i++) { // i = 2
            int length = strlen(lines[i]);
            if (needTemp == NOT_NEEDED) {
                for (int j = 0; j < length; j += chunkSize) {
                    // Process each character within the chunk, ensuring not to exceed string bounds
                    if (getIndex(lines[i][j]) != -1) {
                        temp[getIndex(lines[i][j])]++;
                        (*regularChar)++;
                        if ((length - 2 - j) < chunkSize) {
                            aux = chunkSize - (length - 1 - j);
                            needTemp = NEEDED;   
                        }
                    } else {
                        j++;
                    }
                }
            } else if (needTemp == NEEDED) {
                for (int j = aux; j < length; j += chunkSize) {
                    // Process each character within the chunk, ensuring not to exceed string bounds
                    if (getIndex(lines[i][j]) != -1) {
                        temp[getIndex(lines[i][j])]++;
                        (*regularChar)++;
                        if ((length - 2 - j) < chunkSize) {
                            aux = chunkSize - (length - 1 - j);
                            needTemp = NEEDED;   
                        }
                    } else {
                        j++;
                    }
                }
            }        
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