#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cifras.h"

#define TABLE_SIZE 67
#define DEFAULT_SIZE 20 
#define ABSURDLY_LARGE_ERROR 1000

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

void cesarAttack(FILE *input_stream, int *min_offset, double *min_error, int eflag)
{
    int chunkSize = 1;
    unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0};
    double *freq = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize, eflag);

    double error[TABLE_SIZE + 1] = {ABSURDLY_LARGE_ERROR};

    for (int j = 0; j < TABLE_SIZE; j++) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            error[j] += ((pow((freq[i] - probabilities[(i + j) % 67]), 2)) / probabilities[(i + j) % 67]);
        }
    }

    free(freq);
    *min_error = error[0];
    for (int i = 1; i < TABLE_SIZE; i++) {
        if (error[i] < *min_error) {
            *min_error = error[i];
            *min_offset = i;
        }
    }
    return;
}

void vigenereAttack(FILE *input_stream, FILE *output_stream, int maxKeySize, int eflag){
    if (maxKeySize <= DEFAULT_SIZE) {
        maxKeySize = DEFAULT_SIZE; // Restrict to the default max size
    }
    // Dynamically allocate memory for the key array with initial size + 1 for null terminator
    char **key = (char **)malloc(maxKeySize * sizeof(char *)); // Initial minimal allocation
    
    // Other necessary allocations
    double **errorArray = (double **) malloc(maxKeySize * sizeof(double *));
    int *min_offset = (int *) calloc(maxKeySize, sizeof(int));
    double *min_error = (double *) malloc(maxKeySize * sizeof(double));

    double tempMinError = ABSURDLY_LARGE_ERROR;
    int tempMinErrorIndex = 0;
    for (int chunkSize = 1; chunkSize <= maxKeySize; chunkSize++) {
        // Memory management and initial assignment within the loop
        key[chunkSize - 1] = malloc((chunkSize + 1) * sizeof(char));  // Adjust `key` size for current chunkSize
        for (int i = 0; i < chunkSize; i++) {          
            errorArray[i] = (double *) calloc(TABLE_SIZE, sizeof(double)); // Allocate or reallocate memory
            min_error[i] = ABSURDLY_LARGE_ERROR;
        }

        for (int i = 0; i < chunkSize; i++) { // For each segment of the key
            unsigned int regularChar = 0, weirdChar = 0, temp[TABLE_SIZE] = {0};
            fseek(input_stream, i, SEEK_SET);  // Start from the ith character
            double *freq = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize, eflag);

            // Quadratic error calculation for each possible character in the key
            for (int offset = 0; offset < TABLE_SIZE; offset++) {
                for (int j = 0; j < TABLE_SIZE; j++) { // For each character in array
                    int index = (j + offset) % TABLE_SIZE;
                    errorArray[i][offset] += (pow(freq[j] - probabilities[index], 2)) / probabilities[index];
                }
            }

            // Identify minimum error and corresponding character for this segment
            for (int j = 0; j < TABLE_SIZE; j++) {
                if (errorArray[i][j] < min_error[i]) {
                    min_error[i] = errorArray[i][j];
                    min_offset[i] = j;
                }
            }
            free(freq); // Free the frequency array now that we're done with it
            key[chunkSize - 1][i] = cipher_table[((TABLE_SIZE - min_offset[i]) % TABLE_SIZE)]; // Adjust getCipherChar function accordingly
        }
        key[chunkSize - 1][chunkSize] = '\0'; // Null-terminate after each assignment
        
        // Free allocated errorArray memory for this chunkSize iteration
        // precisa de espaço
        


        FILE *att_out = fopen("attdraft.txt", "w");
        rewind(input_stream);
        
        int attKeySize = strlen(key[chunkSize - 1]);
        int *attOffset = offset_calculator(key[chunkSize - 1], attKeySize);
        filter_d(input_stream, att_out, attOffset, attKeySize, 0, 2);
        fclose(att_out);

        att_out = fopen("attdraft.txt", "r");
        unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0};
        double *attFreq = statCalculator(att_out, &regularChar, &weirdChar, temp, chunkSize, 1);


        double error = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            error += ((pow((attFreq[i] - probabilities[i]), 2)) / probabilities[i]);
        }
        if (error < tempMinError) {
            tempMinError = error;
            fprintf(output_stream, "min \t");
            tempMinErrorIndex = chunkSize;
        } else {
            fprintf(output_stream, "\t");
        }

        fprintf(output_stream, "tamanho chave %d: \"%s\" erro %f\n", chunkSize, key[chunkSize - 1], error);
        
        for (int i = 0; i < chunkSize; i++) {
            free(errorArray[i]);
        }
        free(attFreq);
        free(attOffset);
        fclose(att_out);
    }

    rewind(input_stream);

    int attKeySize = strlen(key[tempMinErrorIndex - 1]);
    int *attOffset = offset_calculator(key[tempMinErrorIndex - 1], attKeySize);
    filter_d(input_stream, output_stream, attOffset, attKeySize, 0, 2); 
    // Final clean-up
    free(attOffset);
    free(errorArray);
    free(min_offset);
    free(min_error);
    for (int i = 0; i < maxKeySize; i++) {
        free(key[i]);
    }
    free(key);
    remove("attdraft.txt");

    return;
}