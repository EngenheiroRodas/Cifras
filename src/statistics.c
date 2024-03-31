#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cifras.h"

#define TABLE_SIZE 67
#define NEEDED 1
#define NOT_NEEDED 0

int getIndex(char c);

void removeForeignCharacters(char *str) {
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (getIndex(str[i]) != -1) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0'; // Null-terminate the resulting string
}

double *statCalculator(FILE *input_stream, unsigned int *regularChar, unsigned int *weirdChar, unsigned int *temp, int chunkSize, int eflag) {
    char **lines = NULL;
    int lineCounter = 0;
    lines = loadFile(input_stream, &lineCounter);//

    double *frequency = (double *)malloc((TABLE_SIZE + 1) * sizeof(double));
    if (frequency == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for character statistics.\n");
        return NULL;
    }

    if (eflag == 1) { //estatística
        for (int i = 0; i < lineCounter; i++) {
            int j = 0;
            while (lines[i][j] != '\0') {
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
        }
    } else { //ataque
        int bleedOver = 0; // Adjusted for bleeding out logic.
        for (int i = 0; i < lineCounter; i++) {
            removeForeignCharacters(lines[i]);
            int length = strlen(lines[i]); 
            for (int j = 0; j < length; j++) { //
                // Correctly apply the bleed-over amount from the previous line, if any.
                j += bleedOver;
                if (j >= length) {
                    bleedOver = bleedOver - length;
                    break; // If bleed-over exceeds line length, move to next line.
                }

                temp[getIndex(lines[i][j])]++;
                (*regularChar)++;
            
                int nextJump = chunkSize; //
                if (eflag == 0) { // During attack phase, jump calculations differ.
                    int remain = length - (j + 1); // 
                    if (remain < chunkSize) {
                        bleedOver = chunkSize - remain - 1; // (-1 bcz j index) Correctly calculate bleed-over for the next line.
                        break; // End current line processing, bleed-over to next.
                    } else {
                        bleedOver = 0;
                    }
                } else {
                    bleedOver = 0; // Reset bleedOver in statistics phase as it's not needed.
                }
                j += (nextJump - 1); 
                // Apply the jump, considering loop increment. 
            }
        }
    }
    freeLines(lines, &lineCounter);

    // Calculate statistics.
    for (int i = 0; i < TABLE_SIZE; i++) {
        frequency[i] = (*regularChar > 0) ? (double)temp[i] / *regularChar : 0;
    }
    frequency[TABLE_SIZE] = (*weirdChar > 0) ? (double)*weirdChar / (*regularChar + *weirdChar) : 0;

    return frequency;
}