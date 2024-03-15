#include "cifras.h"

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
 
int main(int argc, char *argv[])
{
    int cflag, fflag, iflag, oflag; //flags que vão dizer se os comandos foram utilizados
    int method = 0;
    char key[] = "Programacao2024"; // Password default
    char *inputFile = NULL, *outputFile = NULL;
    int opt;
    fflag = 0; // incialização
    cflag = 1;
    iflag = 0;
    oflag = 0;

    while ((opt = getopt(argc, argv, "fc:d:s:i:o:")) != -1) { // lê as opções fornecidas através da linha de comando
    
        switch (opt) {
            case 'f':
                fflag = 1;
                break;
            case 'c':
                cflag = 1; //cifrar = 1, logo cifrar
                method = atoi(optarg);
                break;
            case 'd':
                cflag = 0; //cifrar = 0, logo decifrar
                method = atoi(optarg); 
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
            default: /* '?' is returned if neither flag is found*/
                fprintf(stderr, "Use -h for help\n");
                exit(EXIT_FAILURE);
        }
    }

    FILE *input_stream = stdin; //Default to standard output
    FILE *output_stream = stdout; // Default to standard output
    if (iflag == 1) {
        input_stream = fopen(inputFile, "r");
        if (input_stream == NULL) {
            fprintf(stderr, "Error opening input file.\n");
            exit(EXIT_FAILURE);
        }
    }
    if (oflag == 1) {
        output_stream = fopen(outputFile, "w");
        if (output_stream == NULL) {
            fprintf(stderr, "Error opening output file.\n");
            exit(EXIT_FAILURE);
        }
    }

    int key_size = strlen(key); //tamanho da password
    if (key_size < 4){ //se password for menor que 4 dá erro
        fprintf(stderr, "ERROR: your password must be at least 4 characters long.\n");
        exit(EXIT_FAILURE);
    }

    if (method >= 3){ //para a fase 2 o método ou pode ser 1 ou 2, mas não temos o 3 implementado
        fprintf(stderr, "ERROR: ciphering method must be either 1 or 2.\n");
        exit(EXIT_FAILURE);
    }

    int *offset_values = offset_calculator(key); // Array para guardar valores do offset

    if (cflag == 1) {
        filter_c(input_stream, output_stream, key, offset_values, fflag, method);
    } else {
        filter_d(input_stream, output_stream, key, offset_values, fflag, method);
    }

    if (iflag == 1) { //close the files
        fclose(input_stream);
        free (inputFile);
    }
    if (oflag == 1) {
        fclose(output_stream);
        free(outputFile);
    }

    free(offset_values);
    return 0;
}