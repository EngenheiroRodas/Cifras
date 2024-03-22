#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_BUFFER 101
#define MAX_LINES 140

int readInputFile(char *linhas[]);
int compare_categories(const void *a, const void *b);
void writeOutput(char *linhas[]);

int main() {
    char *linhas[MAX_LINES];

    writeOutput(linhas);

    return 0;
}

int readInputFile(char *linhas[]) {
    FILE *file = fopen("RunResults.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Cannot open file. \n");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];
    int numLines = 0;

    // Read and discard the first line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        break; // Read and discard only the first line
    }
    
    // Read each line of the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *escalaoA = strchr(buffer, '\t') + 1; // encontra f ou m 
        if (*escalaoA == 'F')
            continue; // Skip this line
        linhas[numLines] = (char *)malloc(strlen(buffer) + 1);
        if (linhas[numLines] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        strcpy(linhas[numLines], buffer);
        numLines++;
    }

    fclose(file);
    return numLines;
}

int compare_categories(const void *a, const void *b) {
    // Cast from void pointers to char pointers
    const char *lineA = *(const char **)a;
    const char *lineB = *(const char **)b;
    
    // Assuming 'Pos.\tEscalão\t...' format, we find the Escalão start after the first tab
    char *escalaoA = strchr(lineA, '\t') + 1; // Points to start of 'Escalão' in A
    char *escalaoB = strchr(lineB, '\t') + 1; // Points to start of 'Escalão' in B

    // Use strncmp to compare the 'Escalão' part directly without modifying originals
    int result = strncmp(escalaoA, escalaoB, 3);

    return result;
}

void writeOutput(FILE output_stream, char *linhas[],int lineCounter) {
    FILE *outputFile = fopen("sorted_output.txt", "w");
    if (outputFile == NULL) {
        fprintf(stderr, "ERROR: Cannot open output file. \n");
        exit(EXIT_FAILURE);
    }

    int numLines = readInputFile(linhas);

    qsort(linhas, numLines, sizeof(char *), compare_categories);

    for (int i = 0; i < numLines; i++) {
        fprintf(outputFile, "%s", linhas[i]);
        free(linhas[i]);
    }

    fclose(outputFile);

    return;
}