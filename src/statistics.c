#include <stdio.h>
#include <stdlib.h>

#include "cifras.h"

#define TABLE_SIZE 67

double *statCalculator(char *lines[], int *lineCounter, unsigned int *regularchar, unsigned int *weirdChar, unsigned int *temp) 
{
    // Allocate stats with one additional space for the "weird" characters, so a total of 68
    double *frequency = (double*)malloc((TABLE_SIZE + 1) * sizeof(double));
    
    if (frequency == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for character statistics.\n");
        return NULL;
    }

    for (int i = 0; i < *lineCounter; i++) {
        char *individualLine = lines[i];
        unsigned int j = 0;
        while (individualLine[j] != '\0')
        {
            int charIndex = getIndex(individualLine[j]);
            if (charIndex!= -1) {
                // Increment the count for recognized characters
                temp[charIndex]++;
                ++*regularchar;
            } else {
                // Increment the count for unrecognized (weird) characters
                ++*weirdChar;
            } 
            j++;
        }
        //contar com \n
        ++*weirdChar;
    }
    

    // Calculate statistics 
    for (int i = 0; i < 67; i++) {
        if (*regularchar != 0) // Check for division by zero
            frequency[i] = ((double)temp[i] / *regularchar);
        else
            frequency[i] = 0; // Handle division by zero
    }
    if ((*regularchar + *weirdChar) != 0) // Check for division by zero
        frequency[67] = ((double)*weirdChar / (*regularchar + *weirdChar));
    else
        frequency[67] = 0; // Handle division by zero

    return frequency;
}