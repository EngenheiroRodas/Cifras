#include "functionsCD.h"
#include "constants.h"
#include <stdio.h>


extern const char cipher_table[];

void cipher(int input, int offset_value)
{   
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (input == (int) cipher_table[i]) {
            int encrypted_index = (i + offset_value) % TABLE_SIZE;
            printf("%c", cipher_table[encrypted_index]);
            return;
        }
    }
    // Se não estiver na tabela sai sem ser cifrado
    printf("%c", input); 
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
}