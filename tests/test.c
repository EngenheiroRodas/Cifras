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

const double probabilities [TABLE_SIZE] = 
{ 0.000160079, 0.000554985, 0.000381091, 0.000306662, 0.000263793, 0.000235223, //0-5
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
  0.001124969 }; //66

int getIndex(char caracter);
char **loadFile(FILE *input_stream, int *lineCounter);
//escreve os conteúdos processados de lines num ficheiro 
void freeLines(char *lines[],int *lineCounter);
//devolve array com valores das estatísticas
double *statCalculator(FILE *input_stream, unsigned int *regularchar, unsigned int *weirdChar, unsigned int *temp, int chunkSize);
void viginereAttack(FILE *input_stream, FILE *output_stream, int nnumber);

int getIndex(char input) {
	int i;
	for(i= 0; i<TABLE_SIZE; i++) {
		if (input == cipher_table[i]) {
			return i; 
		}
	}
	return -1;
}


double *statCalculator(FILE *input_stream, unsigned int *regularchar, unsigned int *weirdChar, unsigned int *temp, int chunkSize) 
{
    //aloca memória, calcula estatísticas
    char **lines = NULL;
    int lineCounter = 0;
    lines = loadFile(input_stream, &lineCounter);

    // Allocate stats with one additional space for the "weird" characters, so a total of 68
    double *frequency = (double*)malloc((TABLE_SIZE + 1) * sizeof(double));
    
    if (frequency == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for character statistics.\n");
        return NULL;
    }

    for (int i = 0; i < lineCounter; i++) {
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
            j += chunkSize;
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
    
    //liberta memória do ficheiro lido, estatísticas calculadas já não é preciso
    freeLines(lines, &lineCounter);

    return frequency;
}

//diz-se qual é a stream de entrada e carrega para a memória dinamicamente para array de strings "lines"
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

        buffer[strcspn(buffer, "\n")] = 0; // Encontra \n e remove-o
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
    unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0};
    if (nnumber <= DEFAULT_SIZE) {
        for (int chunkSize = 1; chunkSize < DEFAULT_SIZE; chunkSize++){ //para chunksize = 2
            //aloca memória
            double **errorArray = (double **) malloc(chunkSize * sizeof(double *));
            for (int i = 0; i < chunkSize; i++) {
                errorArray[i] = (double *) malloc((TABLE_SIZE + 1) * sizeof(double));
            }
            int *min_offset = (int *) calloc(chunkSize, sizeof(int));
            double *min_error = (double *) malloc(chunkSize * sizeof(double)); //aloca 1, 2, 3, 4, 5...
            for (int i = 0; i < chunkSize; i++) {
                min_error[i] = ABSURDLY_LARGE_ERROR;
            }

            for (int i = 1; i <= chunkSize; i++) {
                //cria array de estatísticas
                double *freq = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize); 

                //calcula erro quadrático
                for (int offset = 0; offset < TABLE_SIZE; offset++) {
                    for (int j = 0; j < TABLE_SIZE; j++) {
                        errorArray[(i - 1)][offset] +=((pow((freq[j] - probabilities[(j + offset) % 67]), 2)) / probabilities[(j + offset) % 67]);
                    }
                }
                //fazer ciclo chunksize para error
                for (int j = 1; j < TABLE_SIZE; j++) {
                    if (errorArray[(i - 1)][j] < min_error[(i - 1)]) {
                        min_error[(i - 1)] = errorArray[(i - 1)][j];
                        min_offset[(i - 1)] = j;
                    }
                }
                free(freq);
            }

            fprintf(output_stream, "tamanho chave %d:\"%c\" erro %f\n", 
                                chunkSize, cipher_table[(TABLE_SIZE - (min_offset[chunkSize - 1])) % TABLE_SIZE], min_error[chunkSize - 1]);
            // Memory clean-up
            for (int i = 0; i < DEFAULT_SIZE; i++) {
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
    FILE *input_stream = stdin; //Default to standard output
    FILE *output_stream = stdout; // Default to standard output

    int nnumber = 0;

    input_stream = fopen("1paragraph-vigenere.txt", "r"); //set correct stream
    if (input_stream == NULL) {
                    fprintf(stderr, "ERROR: failure opening input file.\n");
                    exit(EXIT_FAILURE);
                }
    output_stream = fopen("1paragraph-cesar.txt", "w");
    if (output_stream == NULL) {
                    fprintf(stderr, "ERROR: failure opening output file.\n");
                    exit(EXIT_FAILURE);
                }

    viginereAttack(input_stream, output_stream, nnumber);

    return 0;
}