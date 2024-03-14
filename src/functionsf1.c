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