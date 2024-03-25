#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "cifras.h"

#define TABLE_SIZE 67

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

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
 
int main(int argc, char *argv[])
{
    int opt, cflag, fflag, iflag, oflag, eflag, aflag; //flags que vão dizer se os comandos foram utilizados
    int c_method = 0, a_method = 0; //método de cifra e ataque
    char key[] = "Programacao2024"; // Password default
    char *inputFile = NULL, *outputFile = NULL; //nomes de files de input/output
    fflag = 0; // inicialização
    cflag = 1;
    iflag = 0;
    oflag = 0;
    eflag = 0;
    aflag = 0;

    while ((opt = getopt(argc, argv, "fc:d:s:i:o:ea:")) != -1) { // lê as opções fornecidas através da linha de comando
    
        switch (opt) {
            case 'f':
                fflag = 1;
                break;
            case 'c':
                cflag = 1; //cifrar = 1, logo cifrar
                c_method = atoi(optarg);
                break;
            case 'd':
                cflag = 0; //cifrar = 0, logo decifrar
                c_method = atoi(optarg); 
                break;
            case 's':
                strcpy(key, optarg);  //guarda a passe num array chamado key
                break;
            case 'i':
                inputFile = strdup(optarg); //aloca memória para guardar o nome do input
                if (inputFile == NULL) {
                    fprintf(stderr, "Memory allocation failed.\n");
                    exit(EXIT_FAILURE);
                }
                iflag = 1;
                break;
            case 'o':
                outputFile = strdup(optarg); //aloca memória para guardar o nome do output
                if (outputFile == NULL) {
                    fprintf(stderr, "Memory allocation failed.\n");
                    exit(EXIT_FAILURE);
                }
                oflag = 1;
                break;
            case 'e':
                eflag = 1;
                break;
            case 'a':
                aflag = 1;
                a_method = atoi(optarg);
                break;
            default: /* '?' is returned if neither flag is found*/
                fprintf(stderr, "Use -h for help\n");
                exit(EXIT_FAILURE);
        }
    }
    //Error handling
    if (strlen(key) < 4){ //se password for menor que 4 dá erro
        fprintf(stderr, "ERROR: your password must be at least 4 characters long.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < strlen(key); i++) {
        if (getIndex(key[i]) == -1) {
            fprintf(stderr, "ERROR: your password has an unknown character.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (c_method >= 4){ //para a fase 2 o método ou pode ser 1, 2 ou 3, mas não temos o 3 implementado
        fprintf(stderr, "ERROR: ciphering method must be either 1 or 2.\n");
        exit(EXIT_FAILURE);
    }
    //End of error handling
    
    
    
    FILE *input_stream = stdin; //Default to standard output
    FILE *output_stream = stdout; // Default to standard output
       
    if (iflag == 1) {
        input_stream = fopen(inputFile, "r"); //set correct stream
        if (input_stream == NULL) {
            fprintf(stderr, "Error opening input file.\n");
            exit(EXIT_FAILURE);
        }   
    }

    if (oflag == 1) { //set correct stream
        output_stream = fopen(outputFile, "w");
        if (output_stream == NULL) {
            fprintf(stderr, "Error opening output file.\n");
            exit(EXIT_FAILURE);
        }
    }
    char **lines = NULL;

    int lineCounter = 0;
    if (eflag == 1 || aflag == 1) { //carrega para memória, line counter fica modificado
        lines = loadFile(input_stream, &lineCounter);

        if (eflag == 1) {
            unsigned int regularChar = 0, weirdChar = 0;
            double *stats = statCalculator(lines, &lineCounter, &regularChar, &weirdChar);
            if (stats == NULL) {
                fprintf(stderr, "Error: statCalculator failed.\n");
                return EXIT_FAILURE;
            }
            for (int i = 0; i < TABLE_SIZE; i++) {
                fprintf(output_stream,"conta('%c')= %f\n", cipher_table[i], stats[i]);
            }
            fprintf(output_stream, "Total: %u caracteres\n", regularChar);
            fprintf(output_stream, "conta(outros) = %f\n", stats[67]);
            fprintf(output_stream, "Total: %u caracteres\n", (regularChar + weirdChar));

            free(stats);
        }
    }

    //escreve o ficheiro de output e liberta memória, só com -e e -a pq já está incorporado em -c e -d
    if ((eflag == 1) || (aflag == 1)) { 
        freeLines(lines, &lineCounter);
    }

    //cipher decipher block
    int *offset_values = offset_calculator(key); // Array para guardar valores do offset
    if (cflag == 1) {
        filter_c(input_stream, output_stream, key, offset_values, fflag, c_method); //produzem ficheiro de output dentro da função sem alocação de mem dinâmica
    } else {
        filter_d(input_stream, output_stream, key, offset_values, fflag, c_method);
    }
    free(offset_values);

    //closing the files, freeing memory blocks
    if (iflag == 1) {
        fclose(input_stream);
        free (inputFile);
    }
    if (oflag == 1) {
        fclose(output_stream);
        free(outputFile);
    }

    return 0;
}