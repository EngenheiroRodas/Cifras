#include <stdio.h>

#include "cifras.h"

double *statCalculator(char *lines[], int *lineCounter) {
    // Variable Declarations
    unsigned int regularCharCount = 0, weirdCharCount = 0;
    int temp[68] = {0}; // Initialize array to zero
    double stats[68]; // Static array to retain values after the function returns

    // Calculate statistics
    for (int i = 0; i < *lineCounter; i++) {
        char *individualLine = lines[i];
        int j = 0;
        while (individualLine[j] != '\0') {
            int charIndex = getIndex(individualLine[j]); // Assuming getIndex is defined elsewhere
            if (charIndex != -1) {
                regularCharCount++;
            } else {
                weirdCharCount++;
            }
            temp[charIndex]++;
            j++;
        }
    }

    // Calculate statistics as percentages
    for (int i = 0; i < 67; i++) {
        if (regularCharCount != 0) // Check for division by zero
            stats[i] = (double)temp[i] / regularCharCount;
        else
            stats[i] = 0; // Handle division by zero
    }
    if ((regularCharCount + weirdCharCount) != 0) // Check for division by zero
        stats[67] = (double)temp[67] / (regularCharCount + weirdCharCount);
    else
        stats[67] = 0; // Handle division by zero

    return stats; // Return pointer to the array of statistics
}