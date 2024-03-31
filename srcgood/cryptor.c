#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cifras.h"

#define TABLE_SIZE 67

#define CESAR 1
#define VIGENERE 2

#define NEEDED 1
#define NOT_NEEDED 0

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
char encode(char input, int offset_value)
{
    // Se estiver na tabela é codificado 
    int input_num = getIndex(input); 
    if (0 <= input_num && input_num < TABLE_SIZE) {
        return (cipher_table[(input_num + offset_value) % TABLE_SIZE]);
    }
    // Se o input não estiver na tabela, sai sem ser modificado
    else {
    return input; 
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
char decode(char input, int offset_value)
{
    int input_num = getIndex(input);
    // Se estiver na tabela é codificado
    if (0 <= input_num && input_num < TABLE_SIZE) {
        return (cipher_table[(input_num + TABLE_SIZE - offset_value) % TABLE_SIZE]);
    }

    // Se o input não estiver na tabela, sai sem ser modificado
    else {
    return input;
    }
}

/*
 * Function:  calcular offsets 
 * --------------------
 * transforma as letras da senha em valores da tabela:
 *
 * input: senha
 * 
 * Returns: guarda num array offset_values, com o tamanho da chave, os valores da tabela de cada letra da senha, 
 * retornando o apontador para o início desse array
 */
int *offset_calculator(char *key, int key_size)
{
    int *offset_values = malloc(key_size * sizeof(int)); // allocate memory for the offset values
    int key_index = 0; 

    if (offset_values == NULL) {
        printf("Memory allocation failed\n");
        return NULL; // return NULL to indicate failure
    }

    for (int i = 0; key[i] != '\0'; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            if (key[i] == cipher_table[j]) {
                offset_values[key_index++] = j;
                break;
            }
        }   
    }

    return offset_values;
}

void filter_c(FILE *input_stream, FILE *output_stream, int *offset_values, int key_size, int fflag, int c_method) 
{
    int offset_value = offset_values[0]; // Valor de offset para caracter.
    
    int agregado = 0; // agregado de no máximo 6 letras
    int blocos = 0; // conjunto de no máximo 8 blocos
    int key_index = 0;

    char input, output;
    if (fflag == 1) {
        int novaLinha = NEEDED; //para dar \n no final
        while ((fscanf(input_stream, "%c", &input) == 1) && input != EOF) {
            novaLinha = NEEDED;
            if (c_method == VIGENERE) {
                offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
            }
            output = encode(input, offset_value);
            /* getIndex tem de ser diferente de -1(porque este valor indica que input não está na tabela) 
            para o caracter poder sair, já que na formatação elementos não pertencentes da tabela são eliminados*/
            if (getIndex(input) != -1) { 
                key_index++;
                if (agregado < 5) {
                    agregado++;
                    fprintf(output_stream,"%c", output);//printf do elemento codificado
                } else {
                    agregado = 0;
                    fprintf(output_stream,"%c", output);
                    if (blocos < 7) {
                        fprintf(output_stream, "_"); //insere underscore quando agregado = 6
                        blocos++;
                    } else {
                        fprintf(output_stream, "\n"); //insere '\n' quando número de blocos = 8
                        novaLinha = NOT_NEEDED; // se no final do programa a última operação tiver sido printf("\n"), não a volta a repetir
                        blocos = 0;
                    }
                }
            }
        }
        if (novaLinha == NEEDED) { // se no final do programa a última operação não tiver sido printf("\n")
            fprintf(output_stream, "\n");
        }
    } else {
        while ((fscanf(input_stream, "%c", &input) == 1) && input != EOF) {
            if (c_method == CESAR) {
                output = encode(input, offset_value); //printf do elemento codificado
                fprintf(output_stream,"%c", output);
            } else if (c_method == VIGENERE) {
                offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                output = encode(input, offset_value); //printf do elemento codificado
                fprintf(output_stream,"%c", output);
                if (getIndex(input) == -1) {
                    key_index--; //se não pertencer à tabela, o elemento não é codificado, logo a chave não deve avançar
                }
            }
            key_index++; //avança um elemento na chave
        } 
    }
    return;
}

void filter_d(FILE *input_stream, FILE *output_stream, int *offset_values, int key_size, int fflag, int c_method) 
{
    int offset_value = offset_values[0]; // Valor de offset para caracter.
    
    int agregado = 0; // agregado de no máximo 6 letras
    int blocos = 0; // conjunto de no máximo 8 blocos
    int key_index = 0;

    char input, output;
    if (fflag == 1) {
        int novaLinha = NEEDED; //para dar \n no final
        while ((fscanf(input_stream, "%c", &input) == 1) && input != EOF) {
            novaLinha = NEEDED;
            if (c_method == VIGENERE) {
                offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
            }
            output = decode(input, offset_value);
            if (getIndex(input) != -1) {
                key_index++;
                if (agregado < 5) {
                    agregado++; 
                    fprintf(output_stream,"%c", output);//printf do elemento descodificado
                } else {
                    agregado = 0; 
                    fprintf(output_stream,"%c", output);//printf do elemento descodificado
                    if (blocos < 7) {
                        blocos++;
                    } else {
                        fprintf(output_stream, "\n");
                        novaLinha = NOT_NEEDED; // se no final do programa a última operação tiver sido printf("\n"), não a volta a repetir
                        blocos = 0;
                    }
                }
            }
        }
        if (novaLinha == NEEDED) { // se no final do programa a última operação não tiver sido printf("\n")
            fprintf(output_stream, "\n");
        }
    } else {
        while ((fscanf(input_stream, "%c", &input) == 1) && input != EOF) {
            if (c_method == CESAR) {
                output = decode(input, offset_value); //printf do elemento descodificado
                fprintf(output_stream,"%c", output);
            } else if (c_method == VIGENERE) {
                offset_value = offset_values[key_index % key_size]; // Valor de offset para caracteres vai alterando à medida que o ciclo while executa.
                output = decode(input, offset_value); //printf do elemento descodificado
                fprintf(output_stream,"%c", output);
                if (getIndex(input) == -1) {
                    key_index--; //se não pertencer à tabela, o elemento não é descodificado, logo a chave não deve avançar
                }
            }
            key_index++; //avança um elemento na chave
        }
    }
    return;
}