#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 101

int readInputFile(char ***linhas_ptr) {
    FILE *file = fopen("RunResults.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Cannot open file.\n");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];
    int numLines = 0;

    // Dynamically allocate memory for the array of pointers
    char **linhas = NULL;

    // Read and discard the first line
    fgets(buffer, sizeof(buffer), file);

    // Read each line of the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Dynamically allocate memory for a new line pointer
        char **temp = (char **)realloc(linhas, (numLines + 1) * sizeof(char *));
        if (temp == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            fclose(file);
            for (int i = 0; i < numLines; i++) {
                free(linhas[i]);
            }
            free(linhas);
            exit(EXIT_FAILURE);
        }
        linhas = temp;

        // Dynamically allocate memory for the string and copy the content
        linhas[numLines] = strdup(buffer);
        if (linhas[numLines] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            fclose(file);
            for (int i = 0; i < numLines; i++) {
                free(linhas[i]);
            }
            free(linhas);
            exit(EXIT_FAILURE);
        }

        numLines++;
    }

    fclose(file);

    // Update the pointer to the array of pointers
    *linhas_ptr = linhas;

    return numLines;
}

int main() {
    char **linhas = NULL;
    int numLines = readInputFile(&linhas);
    printf("Number of lines read: %d\n", numLines);

    // Print or process each line as needed
    for (int i = 0; i < numLines; i++) {
        printf("Line %d: %s", i + 1, linhas[i]);
    }

    // Free memory for each string
    for (int i = 0; i < numLines; i++) {
        free(linhas[i]);
    }

    // Free memory for the array of pointers
    free(linhas);

    return 0;
}
