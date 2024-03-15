#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 101
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

int readLinesFromFile(const char *filename, char *lines[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Cannot open file.\n");
        return -1;
    }

    char buffer[MAX_BUFFER];
    int numLines = 0;
    int maxLines = INITIAL_CAPACITY;

    // Allocate memory for the initial capacity
    lines = (char **)malloc(maxLines * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(file);
        return -1;
    }

    // Read each line of the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Check if reallocation is needed
        if (numLines >= maxLines) {
            maxLines *= GROWTH_FACTOR; // Increase capacity using growth factor
            lines = (char **)realloc(lines, maxLines * sizeof(char *));
            if (lines == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                fclose(file);
                return -1;
            }
        }

        // Allocate memory for the string and copy the content
        lines[numLines] = strdup(buffer);
        if (lines[numLines] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            fclose(file);
            return -1;
        }

        numLines++;
    }

    fclose(file);

    return numLines;
}

int main() {
    char *lines[INITIAL_CAPACITY] = {NULL}; // Array of pointers initialized to NULL
    char *filename = "RunResults.txt";
    int numLines = readLinesFromFile(filename, lines);
    if (numLines == -1) {
        fprintf(stderr, "Failed to read lines from file.\n");
        return EXIT_FAILURE;
    }

    printf("Number of lines read: %d\n", numLines);

    // Process or use the data stored in lines[]

    // Free memory for each string
    for (int i = 0; i < numLines; i++) {
        free(lines[i]);
    }

    return EXIT_SUCCESS;
}