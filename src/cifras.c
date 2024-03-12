#include <stdlib.h> /* for exit */
#include <string.h>
#include <unistd.h>
#include <stdio.h> /* for printf */

#define TABLE_SIZE 67
#define CESAR 1
#define VIGENERE 2

#define NEEDED 1
#define NOT_NEEDED 0 

// devolve o valor de tabela do input
int getIndex(char caracter);
// codifica e imprime o caracter de input
void encode(char input, int offset_value); 
// descodifica e imprime o caracter codificado
void decode(char input, int offset_value);

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
    int cflag, fflag; //flags que vão dizer se os comandos foram utilizados
    int method = 0;
    int opt;
    char key[] = "Programacao2024"; // Password default
 
    fflag = 0;
    cflag = 1;

    while ((opt = getopt(argc, argv, "fc:d:s:")) != -1) { // lê as opções fornecidas através da linha de comando
    
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
            default: /* '?' is returned if neither flag is found*/
                fprintf(stderr, "Use -h for help\n");
                exit(EXIT_FAILURE);
        }
    }

    int key_size = strlen(key); //tamanho da password
    if (key_size < 4){ //se password for menor que 4 dá erro
        fprintf(stderr, "ERROR: your password must be at least 4 characters long.\n");
        exit(EXIT_FAILURE);
    }

    if (method >= 3){ //para a fase 1 o método ou pode ser 1 ou 2
        fprintf(stderr, "ERROR: ciphering method must be either 1 or 2.\n");
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

    char input;
    key_index = 0;
    int offset_value = offset_values[0]; // Valor de offset para caracter.
    int agregado = 0; // agregado de no máximo 6 letras
    int blocos = 0; // conjunto de no máximo 8 blocos
    
    if (cflag == 1) {
        if (fflag == 1) {
            int novaLinha = NEEDED; //para dar \n no final
            while ((scanf("%c", &input) == 1) && input != EOF) {
                novaLinha = NEEDED;
                if (method == VIGENERE) {
                    offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                }
                /* getIndex tem de ser diferente de -1(porque este valor indica que input não está na tabela) 
                para o caracter poder sair, já que na formatação elementos não pertencentes da tabela são eliminados*/
                if (getIndex(input) != -1) { 
                    key_index++;
                    if (agregado < 5) {
                        agregado++;
                        encode(input, offset_value); //printf do elemento codificado
                    } else {
                        agregado = 0;
                        encode(input, offset_value); //printf do elemento codificado
                        if (blocos < 7) {
                            printf("_"); //insere underscore quando agregado = 6
                            blocos++;
                        } else {
                            printf("\n"); //insere '\n' quando número de blocos = 8
                            novaLinha = NOT_NEEDED; // se no final do programa a última operação tiver sido printf("\n"), não a volta a repetir
                            blocos = 0;
                        }
                    }
                }
            }
            if (novaLinha == NEEDED) { // se no final do programa a última operação não tiver sido printf("\n")
                printf("\n");
            }
        } else {
            while ((scanf("%c", &input) == 1) && input != EOF) {
                if (method == CESAR) {
                    encode(input, offset_value); //printf do elemento codificado
                } else if (method == VIGENERE) {
                    offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                    encode(input, offset_value); //printf do elemento codificado
                    if (getIndex(input) == -1) {
                        key_index--; //se não pertencer à tabela, o elemento não é codificado, logo a chave não deve avançar
                    }
                }
                key_index++; //avança um elemento na chave
            } 
        }
    } else {
        if (fflag == 1) {
            int novaLinha = NEEDED; //para dar \n no final
            while ((scanf("%c", &input) == 1) && input != EOF) {
                novaLinha = NEEDED;
                if (method == VIGENERE) {
                    offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                }
                if (getIndex(input) != -1) {
                    key_index++;
                    if (agregado < 5) {
                        agregado++;
                        decode(input, offset_value); //printf do elemento descodificado
                    } else {
                        agregado = 0; 
                        decode(input, offset_value); //printf do elemento descodificado
                        if (blocos < 7) {
                            blocos++;
                        } else {
                            printf("\n");
                            novaLinha = NOT_NEEDED; // se no final do programa a última operação tiver sido printf("\n"), não a volta a repetir
                            blocos = 0;
                        }
                    }
                }
            }
            if (novaLinha == NEEDED) { // se no final do programa a última operação não tiver sido printf("\n")
                printf("\n");
            }
        } else {
            while ((scanf("%c", &input) == 1) && input != EOF) {
                if (method == CESAR) {
                    decode(input, offset_value); //printf do elemento descodificado
                } else if (method == VIGENERE) {
                    offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                    decode(input, offset_value); //printf do elemento descodificado
                    if (getIndex(input) == -1) {
                        key_index--; //se não pertencer à tabela, o elemento não é descodificado, logo a chave não deve avançar
                    }
                }
                key_index++; //avança um elemento na chave
            }
        }
    }
    return 0;
}

/*
 * Function:  getIndex 
 * --------------------
 * devolve o valor de tabela do input:
 *
 * Parâmetros: caracter de input
 *
 * Returns: o valor da tabela do caracter de input
 *          se o valor não estiver na tabela devolve -1
 */
int getIndex(char input) {
	int i;
	for(i= 0; i<TABLE_SIZE; i++) {
		if (input == cipher_table[i]) {
			return i; 
		}
	}
	return -1;
}

/*
 * Function:  encode 
 * --------------------
 * codifica e imprime o caracter de input:
 *
 * input: caracter de input tranformado para o valor de tabela por getIndex(input)
 * offset_value: o valor do offset
 * 
 * Returns: imprime para stdout o input codificado, usando encoded = (input + offset) % 67 
 */
void encode(char input, int offset_value)
{
    // Se estiver na tabela é codificado 
    int input_num = getIndex(input); 
    if (0 <= input_num && input_num < TABLE_SIZE) {
        printf("%c", cipher_table[(input_num + offset_value) % TABLE_SIZE]);
        return;
    }
    // Se o input não estiver na tabela, sai sem ser modificado
    else {
    printf("%c", input); 
    return;
    }
}

/*
 * Function:  decode 
 * --------------------
 * descodifica e imprime o caracter codificado:
 *
 * input: caracter de input codificado, tranformado para o valor de tabela por getIndex(input)
 * offset_value: o valor do offset
 * 
 * Returns: imprime para stdout o input descodificado, usando decoded = (encoded_char - offset + 67) % 67 
 */
void decode(char input, int offset_value)
{
    int input_num = getIndex(input);
    // Se estiver na tabela é codificado
    if (0 <= input_num && input_num < TABLE_SIZE) {
        printf("%c", cipher_table[(input_num + TABLE_SIZE - offset_value) % TABLE_SIZE]);
        return;
    }

    // Se o input não estiver na tabela, sai sem ser modificado
    else {
    printf("%c", input); 
    return;
    }
}