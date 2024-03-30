#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#define MAX_BUFFER 100
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

#define TABLE_SIZE 67
#define ABSURDLY_LARGE_ERROR 1000

#define NEEDED 1
#define NOT_NEEDED 0
#define DEFAULT_SIZE 20 

char cipher_table[TABLE_SIZE] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0-9
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', // 10-19
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', // 20-29
    'U', 'V', 'W', 'X', 'Y', 'Z', // 30-35
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 36-45
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 46-55
    'u', 'v', 'w', 'x', 'y', 'z', // 56-61
    ' ', '.', ',', ';', '-', //62-66
};

const double probabilities[TABLE_SIZE] = { 
    0.000160079, 0.000554985, 0.000381091, 0.000306662, 0.000263793, 0.000235223, //0-5
    0.000214441, 0.00019842, 0.000185551, 0.000174894, 0.002049944, 0.00037499, //6-11
    0.000699981, 0.00107497, 0.003174913, 0.000549985, 0.000499986, 0.001524958, //12-17
    0.001749952, 3.7499E-05, 0.000192495, 0.000999973, 0.000599984, 0.001674954, //18-23
    0.001874949, 0.000474987, 2.37493E-05, 0.001499959, 0.001574957, 0.002274938, //24-29
    0.000699981, 0.000244993, 0.000599984, 3.7499E-05, 0.000499986, 1.84995E-05, //30-35
    0.061498311, 0.011249691, 0.020999423, 0.032249114, 0.095247384, 0.016499547, //36-41
    0.014999588, 0.045748744, 0.052498558, 0.001124969, 0.005774841, 0.029999176, //42-47
    0.017999506, 0.05024862, 0.056248455, 0.014249609, 0.00071248, 0.044998764, //48-53
    0.047248702, 0.068248126, 0.020999423, 0.007349798, 0.017999506, 0.001124969, //54-59
    0.014999588, 0.000554985, 0.190494768, 0.014249609, 0.014999588, 0.000554985, //60-65
    0.001124969 //66
};

int getIndex(char input);
char **loadFile(FILE *input_stream, int *lineCounter);
void freeLines(char *lines[], int *lineCounter);
double *statCalculator(FILE *input_stream, unsigned int *regularchar, unsigned int *weirdChar, unsigned int *temp, int chunkSize);
void viginereAttack(FILE *input_stream, FILE *output_stream, int nnumber);

int getIndex(char input) {
    int i;
    for(i = 0; i < TABLE_SIZE; i++) {
        if (input == cipher_table[i]) {
            return i; 
        }
    }
    return -1;
}

double *statCalculator(FILE *input_stream, unsigned int *regularChar, unsigned int *weirdChar, unsigned int *temp, int chunkSize) {
    char **lines = NULL;
    int lineCounter = 0;
    lines = loadFile(input_stream, &lineCounter);

    double *frequency = (double *)malloc((TABLE_SIZE + 1) * sizeof(double));
    if (frequency == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for character statistics.\n");
        return NULL;
    }

    int aux = 0;
    int needTemp = NOT_NEEDED;
    for (int i = 0; i < lineCounter; i++) {
        int length = strlen(lines[i]);
        if (needTemp == NOT_NEEDED) {
            for (int j = 0; j < length; j += chunkSize) {
                // Process each character within the chunk, ensuring not to exceed string bounds
                if (getIndex(lines[i][j]) != -1) {
                    temp[getIndex(lines[i][j])]++;
                    (*regularChar)++;
                } else {
                    (*weirdChar)++;
                }
                if (j + chunkSize >= length - 1) {
                    aux = length - 1 - j;
                    needTemp = NEEDED;
                }
            }
            // Account for the newline character at the end of each line
            (*weirdChar)++;
        } else {
            for (int j = 0; j < length; j += chunkSize) {
                // Process each character within the chunk, ensuring not to exceed string bounds
                j = aux;
                if (getIndex(lines[i][j]) != -1) {
                    temp[getIndex(lines[i][j])]++;
                    (*regularChar)++;
                } else {
                    (*weirdChar)++;
                }
                if (j + chunkSize >= length - 1) {
                    aux = length - 1 - j;
                    needTemp = NEEDED; 
                }
            }
            // Account for the newline character at the end of each line
            (*weirdChar)++;
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

char **loadFile(FILE *input_stream, int *lineCounter) {
    char buffer[MAX_BUFFER];
    *lineCounter = 0;
    int capacity = INITIAL_CAPACITY;

    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for lines.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, MAX_BUFFER, input_stream) != NULL) {
        if (*lineCounter == capacity) {
            capacity *= GROWTH_FACTOR;
            char **tmp = realloc(lines, capacity * sizeof(char *));
            if (tmp == NULL) {
                fprintf(stderr, "ERROR: Failed to reallocate memory.\n");
                for (int i = 0; i < *lineCounter; i++) {
                    free(lines[i]); // Free individual lines
                }
                free(lines); // Free the lines array
                exit(EXIT_FAILURE);
            }
            lines = tmp;
        }

        // Remove trailing newline character
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        }

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

void viginereAttack(FILE *input_stream, FILE *output_stream, int nnumber)
{
    if (nnumber <= DEFAULT_SIZE) {
        char key[20];
        for (int chunkSize = 1; chunkSize <= nnumber; chunkSize++){ //para chunksize = 2
            //aloca memória
            double **errorArray = (double **) malloc(chunkSize * sizeof(double *)); //aloca 2 letras
            for (int i = 0; i < chunkSize; i++) {
                errorArray[i] = (double *) malloc(TABLE_SIZE * sizeof(double)); //aloca 67 offsets por letra de senha (2*67)
            }
            int *min_offset = (int *) calloc(chunkSize, sizeof(int)); //inicia min_offset com 2 espaços
            double *min_error = (double *) malloc(chunkSize * sizeof(double)); //aloca 2 espaços
            for (int i = 0; i < chunkSize; i++) { //atribui à letra 1 e 2 erro de 1000
                min_error[i] = ABSURDLY_LARGE_ERROR;
            }


            for (int i = 0; i < chunkSize; i++) { //vamos em i = 1!!!(chunksize = 2)
                unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0};
                //cria array de estatísticas
                fseek(input_stream, i, SEEK_SET); // set na letra, se i = 1, segnda
                double *freq = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize);

                //calcula erro quadrático
                for (int offset = 0; offset < TABLE_SIZE; offset++) {
                    for (int j = 0; j < TABLE_SIZE; j++) { //guarda em erroArray segnda letra
                        errorArray[i][offset] +=((pow((freq[j] - probabilities[(j + offset) % 67]), 2)) / probabilities[(j + offset) % 67]);
                    }
                }
                //fazer ciclo chunksize para error
                for (int j = 1; j < TABLE_SIZE; j++) {
                    if (errorArray[i][j] < min_error[i]) {
                        min_error[i] = errorArray[i][j];
                        min_offset[i] = j;
                    }
                }
                free(freq);
                key[i] = cipher_table[(TABLE_SIZE - (min_offset[i])) % TABLE_SIZE];
            }
            
            fprintf(output_stream, "tamanho chave %d:", chunkSize);
            fprintf(output_stream, "\"%s\"\n", key);

            
            
            
            
            // Memory clean-up
            for (int i = 0; i < chunkSize; i++) {
                free(errorArray[i]);
            }
            free(errorArray);
            free(min_offset);
            free(min_error);
        }
        
    } else {

        return;
    }
}

int main () {
    FILE *input_stream = fopen("4lines-vigenere.txt", "r");
    if (input_stream == NULL) {
        fprintf(stderr, "ERROR: failure opening input file.\n");
        exit(EXIT_FAILURE);
    }

    FILE *output_stream = fopen("4lines-attacked3.txt", "w");
    if (output_stream == NULL) {
        fprintf(stderr, "ERROR: failure opening output file.\n");
        fclose(input_stream);  // Ensuring input_stream is closed before exit
        exit(EXIT_FAILURE);
    }
    
    // int nnumber = 2;  // Adjust this value to change the maximum chunk size
    int chunkSize = 2;
            unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0};
            double *frequency = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize);

            for (int i = 0; i < TABLE_SIZE; i++) {
                fprintf(output_stream,"conta('%c')=%u\t%f%%\n", cipher_table[i], temp [i], (frequency[i] * 100));
            }

            fprintf(output_stream, "Total: %u caracteres\n", regularChar);
            fprintf(output_stream, "conta(outros)=%u\t%f%%\n", weirdChar, (frequency[67] * 100));
            fprintf(output_stream, "Total do ficheiro: %u caracteres\n", (regularChar + weirdChar));
            free(frequency);

    fclose(input_stream);
    fclose(output_stream);
    return 0;
}