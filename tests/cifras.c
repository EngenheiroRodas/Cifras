#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 67
#define MAX_BUFFER 100
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

const char cipher_table[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0-9
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', // 10-19
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', // 20-29
    'U', 'V', 'W', 'X', 'Y', 'Z', // 30-35
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 36-45
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 46-55
    'u', 'v', 'w', 'x', 'y', 'z', // 56-61
    ' ', '.', ',', ';', '-', //62-66
};
// Function prototype for statCalculator (assuming it's declared in cifras.h)
double *statCalculator(char *lines[], int *lineCounter);
int getIndex(char input);
void freeLines(char *lines[], int lineCounter);
char **loadFile(FILE *input_stream, int *lineCounter);

int main() {
    FILE *input_stream = NULL;
    input_stream = fopen("cesar_1paragraph.txt", "r");
    if (input_stream == NULL) {
        fprintf(stderr, "Error: Unable to open the file.\n");
        exit(EXIT_FAILURE);
    }
    char **lines;

    // Calculate the number of lines (excluding the end marker)
    int lineCounter = 0;
    lines = loadFile(input_stream, &lineCounter);
    
        
    double *stats = statCalculator(lines, &lineCounter);
    if (stats == NULL) {
        fprintf(stderr, "Error: statCalculator failed.\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < 68; i++) {
        fprintf(stdout,"%.2f\n", stats[i]);
    }
    free(stats);


    fclose(input_stream);
    return EXIT_SUCCESS;
}

double *statCalculator(char *lines[], int *lineCounter) {
    // We need 68 positions: 67 for the valid characters and 1 additional for "weird" characters
    unsigned int counts[TABLE_SIZE + 1] = {0}, totalChars = 0;
    // Allocate stats with one additional space for the "weird" characters, so a total of 68
    double *stats = calloc(TABLE_SIZE + 1, sizeof(double));
    
    if (!stats) {
        fprintf(stderr, "Failed to allocate memory for character statistics.\n");
        return NULL;
    }

    for (int i = 0; i < *lineCounter; i++) {
        for (char *p = lines[i]; *p; p++) {
            int index = getIndex(*p);
            if (index != -1) {
                // Increment the count for recognized characters
                counts[index]++;
                totalChars++;
            } else {
                // Increment the count for unrecognized (weird) characters
                counts[TABLE_SIZE]++;
            }
        }
    }

    // Calculate statistics for recognized characters
    for (int i = 0; i < TABLE_SIZE; i++) {
        stats[i] = totalChars ? (double)counts[i] / totalChars : 0;
    }
    // Separately calculate the stats for unrecognized characters
    stats[TABLE_SIZE] = (*lineCounter) ? (double)counts[TABLE_SIZE] / (*lineCounter) : 0;

    return stats;
}


int getIndex(char input) {
	int i;
	for(i= 0; i<TABLE_SIZE; i++) {
		if (input == cipher_table[i]) {
			return i; 
		}
	}
	return -1;
}


//diz-se qual é a stream de entrada e carrega para a memória dinamicamente para array de strings "lines"
char **loadFile(FILE *input_stream, int *lineCounter) {
    char buffer[MAX_BUFFER];
    *lineCounter = 0;
    int capacity = INITIAL_CAPACITY;

    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, MAX_BUFFER, input_stream) != NULL) {
        if (*lineCounter == capacity) {
            capacity *= GROWTH_FACTOR;
            char **tmp = realloc(lines, capacity * sizeof(char *));
            if (!tmp) {
                free(lines); // Simplified memory cleanup
                fprintf(stderr, "Failed to reallocate memory.\n");
                exit(EXIT_FAILURE);
            }
            lines = tmp;
        }

        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character if it exists
        lines[*lineCounter] = strdup(buffer);
        if (!lines[*lineCounter]) {
            // Simplified error handling
            fprintf(stderr, "Memory allocation for lines failed.\n");
            exit(EXIT_FAILURE);
        }
        (*lineCounter)++;
    }
    
    return lines;
}

void freeLines(char *lines[], int lineCounter) {
    for (int i = 0; i < lineCounter; i++) {
        free(lines[i]);
    }
    free(lines);

    return;
}