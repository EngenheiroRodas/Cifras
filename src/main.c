#include <stdlib.h> // for exit
#include <string.h>
#include <getopt.h>
#include <stdio.h> 

#include "functionsCD.h"
#include "constants.h"

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

extern const char cipher_table[];

int main(int argc, char *argv[])
{
    int fflag, cflag, dflag, sflag;  //flags que vão dizer se os comandos foram utilizados
    int method = 0;
    int opt;
    char key[] = "Programacao2024"; // Password default
    //senha até 1000 caracteres

    fflag = 0; 
    cflag = 0;
    dflag = 0;
    sflag = 0;

    while ((opt = getopt(argc, argv, "fc:d:s:")) != -1) {
        switch (opt) {
            case 'f':
                fflag = 1;
                break;
            case 'c':
                cflag = 1;
                method = atoi(optarg);
                break;
            case 'd':
                dflag = 1;
                method = atoi(optarg); 
                break;
            case 's':
                sflag = 1;
                strcpy(key, optarg);  //guarda a passe num array chamado key
                break;
            default: /* '?' is returned if neither flag is found*/
                fprintf(stderr, "Use -h for help\n");
                exit(EXIT_FAILURE);
        }
    }

    const int key_size = strlen(key); //tamanho da password
    if (key_size < 4){
        fprintf(stderr, "ERROR: your password must be at least 4 characters long.\n");
        exit(EXIT_FAILURE);
    }

    if (method >= 3){
        fprintf(stderr, "ERROR: ciphering method must be either 1 or 2.\n");
        exit(EXIT_FAILURE);
    }

    if(cflag == 1 && dflag == 1) { // não pode decifrar e cifrar ao mesmo tempo
        fprintf(stderr, "ERROR: can't cipher and decipher at the same time, choose only one.\n");
        exit(EXIT_FAILURE);
    }

    int offset_values[key_size]; // Array para guardar valores do offset
    int key_index = 0;
    // Calcula valores de offset na password
    for (int i = 0; key[i] != '\0'; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            if (key[i] == cipher_table[j]) {
                offset_values[key_index++] = j;
                break;
            }
        }   
    }

    int input;
    key_index = 0;
    while ((input = getchar()) && input != EOF) {
        if (cflag == 1) {
            if (method == 1) {
                int offset_value = offset_values[0]; // Valor de offset para caracter.
                cipher(input, offset_value);
            }
            else if (method == 2) {
                int offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                cipher(input, offset_value);
                key_index++;
            }
        } else if (dflag == 1) {
            if (method == 1) {
                int offset_value = offset_values[0]; // Valor de offset para caracter.
                decipher(input, offset_value);
            } else if (method == 2) {
                int offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                decipher(input, offset_value);
                key_index++;
            }
        }
    }
    printf("\n");
    return 0;
}