#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "cifras.h"

#define TABLE_SIZE 67

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

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
 
int main(int argc, char *argv[])
{
    int opt, cflag, fflag, iflag, oflag, eflag, aflag, wflag; //flags que vão dizer se os comandos foram utilizados
    int c_method = 0, a_method = 0; //método de cifra e ataque
    char *key = "Programacao2024"; // Password default
    char *dictionary_name;
    fflag = 0; // inicialização
    cflag = 1;
    iflag = 0;
    oflag = 0;
    eflag = 0;
    aflag = 0;
    wflag = 0;
    int nnumber = 0;

    FILE *input_stream = stdin; //Default to standard output
    FILE *output_stream = stdout; // Default to standard output

    while ((opt = getopt(argc, argv, "fc:d:s:i:o:ea:n:")) != -1) { // lê as opções fornecidas através da linha de comando
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
                key = strdup(optarg);  //guarda a passe num array chamado key
                break;
            case 'i':
                input_stream = fopen(optarg, "r"); //set correct stream
                if (input_stream == NULL) {
                    fprintf(stderr, "ERROR: failure opening input file.\n");
                    exit(EXIT_FAILURE);
                } else
                iflag = 1;
                break;
            case 'o':
                output_stream = fopen(optarg, "w");
                if (output_stream == NULL) {
                    fprintf(stderr, "ERROR: failure opening output file.\n");
                    exit(EXIT_FAILURE);
                } else
                oflag = 1;
                break;
            case 'e':
                eflag = 1;
                break;
            case 'a':
                aflag = 1;
                a_method = atoi(optarg);
                break;
            case 'n':
                nnumber = atoi(optarg);
                break;
            case 'w':
                wflag = 1;
                dictionary_name = strdup(optarg);
                break;
            default: /* '?' is returned if neither flag is found*/
                fprintf(stderr, "Use -h for help\n");
                exit(EXIT_FAILURE);
        }
    }
    //Error handling
    for (size_t i = 0; i < strlen(key); i++) { //password não pode ter carcateres desconhecidos
        if (getIndex(key[i]) == -1) {
            fprintf(stderr, "ERROR: your password has an unknown character.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (c_method >= 4 || a_method >= 4){ //métodos de ataque e cifragem inválidos
        fprintf(stderr, "ERROR: ciphering or attack method invalid.\n");
        exit(EXIT_FAILURE);
    }
    //End of error handling
    
      
    if (eflag == 1) { //calcula estatísticas
        int chunkSize = 2;
        unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0};
        double *frequency = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize, eflag);

        for (int i = 0; i < TABLE_SIZE; i++) {
            fprintf(output_stream,"conta('%c')=%u\t%f%%\n", cipher_table[i], temp [i], (frequency[i] * 100));
        }

        fprintf(output_stream, "Total: %u caracteres\n", regularChar);
        fprintf(output_stream, "conta(outros)=%u\t%f%%\n", weirdChar, (frequency[67] * 100));
        fprintf(output_stream, "Total do ficheiro: %u caracteres\n", (regularChar + weirdChar));
        free(frequency);
    }

    if (aflag == 1) {
        if (a_method == 1) {

        } else if (a_method == 2) { //ataque 2
        int min_offset = 0;
        double min_error = 0;
            cesarAttack(input_stream, &min_offset, &min_error, eflag);

            fprintf(output_stream, "offset com menor erro %d, letra '%c', "
                       "cifrado com letra '%c', "
                       "erro quadrático médio: %f.\n",
                       min_offset, cipher_table[min_offset],
                       cipher_table[(TABLE_SIZE - min_offset) % TABLE_SIZE], min_error);
        
            char key_letter[1] = {cipher_table[(TABLE_SIZE - min_offset) % TABLE_SIZE]};
            int *attack_values = offset_calculator(key_letter, 1);
            rewind(input_stream);

            filter_d(input_stream, output_stream, attack_values, 1, 0, 1);

            free(attack_values);
        } else { //ataque 3
            vigenereAttack(input_stream, output_stream, nnumber, eflag);
        }
    }      
    

    //cipher decipher block
    int key_size = strlen(key);
    int *offset_values = offset_calculator(key, key_size); // Array para guardar valores do offset
    if (cflag == 1) {
        filter_c(input_stream, output_stream, offset_values, key_size, fflag, c_method); //produzem ficheiro de output dentro da função sem alocação de mem dinâmica
    } else {
        filter_d(input_stream, output_stream, offset_values, key_size, fflag, c_method);
    }
    free(offset_values);

    //closing the files, freeing memory blocks
    if (iflag == 1) {
        fclose(input_stream);
    }
    if (oflag == 1) {
        fclose(output_stream);
    }
    if (wflag == 1) {
        free(dictionary_name);
    }
    return 0;
}