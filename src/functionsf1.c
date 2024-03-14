#include "functionsf1.h"
#include "constants.h"
#include <stdio.h>

extern const char cipher_table[];

int getIndex(char input) {
	int i;
	for(i= 0; i<TABLE_SIZE; i++) {
		if (input == cipher_table[i]) {
			return i; 
		}
	}
	return -1;
}

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

int offset_calculator(char *key)
{
    int key_size = strlen(key); //tamanho da password
    int offset_values[key_size]; // Array para guardar valores do offset
    int key_index = 0; 

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

void format_c(char *key, int *offset_values, int fflag, int method) 
{
    int key_size = strlen(key); //tamanho da password
    int offset_value = offset_values[0]; // Valor de offset para caracter.
    
    int agregado = 0; // agregado de no máximo 6 letras
    int blocos = 0; // conjunto de no máximo 8 blocos
    int key_index = 0;

    char input;
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
    }
    return;
}

void format_d(char *key, int *offset_values, int fflag, int method) 
{
    int key_size = strlen(key); //tamanho da password
    int offset_value = offset_values[0]; // Valor de offset para caracter.
    
    int agregado = 0; // agregado de no máximo 6 letras
    int blocos = 0; // conjunto de no máximo 8 blocos
    int key_index = 0;

    char input;
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
    return;
}