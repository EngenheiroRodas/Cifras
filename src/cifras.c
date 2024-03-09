#include <stdlib.h> /* for exit */
#include <string.h>
#include <getopt.h>
#include <stdio.h> /* for printf */

#define TABLE_SIZE 67
void cipher(int input, int offset_value);
void decipher(int input, int offset_value);

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

const char cipher_table[] = { 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', // 0-9
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', // 10-19
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', // 20-29
    'U', 'V', 'W', 'X', 'Y', 'Z', // 30-35
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', // 36-45
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', // 46-55
    'u', 'v', 'w', 'x', 'y', 'z', // 56-61
    ' ', '.', ',', ';', '-' //62-66
};

int main(int argc, char *argv[])
{
    int cflag, dflag, fflag; //flags que vão dizer se os comandos foram utilizados
    int method = 0;
    int opt;
    char key[] = "Programacao2024"; // Password default
    //senha até 1000 caracteres
 
    fflag = 0;
    cflag = 0;
    dflag = 0;

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

    int input, offset_value;
    key_index = 0;
    while ((input = getchar()) && input != EOF) { //cipher(input, offset_value);
        if (method == 1) {
            offset_value = offset_values[0]; // Valor de offset para caracter.
        }
        else if (method == 2) {
            offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
            key_index++;
        }

        if (cflag == 1) {
            if (fflag) {
                int agregado = 0;
                int blocos = 0;
                int novaLinha = 1;
                if (agregado < 5) {
                    agregado++;
                    cipher(input, offset_value); //printf
                } else {
                    agregado = 0;
                    cipher(input, offset_value);
                    if (blocos < 7) {
                        printf("_");
                        blocos++;
                    } else {
                        printf("\n");
                        novaLinha = 0;
                        blocos = 0;
                    }
                }
                if (novaLinha == 1) {
                printf("\n");
                }
            }
            else {
                cipher(input, offset_value);
            }
        }
        else if (dflag == 1){
            if (fflag) {
                int agregado = 0;
                int blocos = 0;
                int novaLinha = 1;
                if (agregado < 5) {
                    agregado++;
                    decipher(input, offset_value); //printf
                } else {
                    agregado = 0;
                    decipher(input, offset_value);
                    if (blocos < 7) {
                        blocos++;
                    } else {
                        printf("\n");
                        novaLinha = 0;
                        blocos = 0;
                    }
                }
                if (novaLinha == 1) {
			    printf("\n");
		        }
            }
            else {
                decipher(input, offset_value);
            }
        }
    }
    printf("\n");
    return 0;
}

void cipher(int input, int offset_value)
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        if (input == (int) cipher_table[i]) {
            int encrypted_index = (i + offset_value) % TABLE_SIZE;
            printf("%c", cipher_table[encrypted_index]);
            return;
        }
    }
    // Se não estiver na tabela sai sem ser cifrado
    printf("%c", input); 
    return;

}

void decipher(int input, int offset_value)
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (input == (int) cipher_table[i]) {
            int deciphered_index = (i + TABLE_SIZE - offset_value) % TABLE_SIZE;
            printf("%c", cipher_table[deciphered_index]);
            return;
        }
    }
    // Se não estiver na tabela sai sem ser cifrado
    printf("%c", input); 
    return;
}
