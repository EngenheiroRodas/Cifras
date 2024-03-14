#ifndef FUNCTIONSF1_H
#define FUNCTIONSF1_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

extern const char cipher_table[];

// devolve o valor de tabela do input
int getIndex(char caracter);
// codifica e imprime o caracter de input
void encode(char input, int offset_value); 
// descodifica e imprime o caracter codificado
void decode(char input, int offset_value);
// converte as letras da password em números da tabela, retornando esse vetor
int *offset_calculator(char *key);
// formata, ou não a saída para a cifra
void format_c(char *key, int *offset_values, int fflag, int method);
// formata, ou não a saída para a decifra
void format_d(char *key, int *offset_values, int fflag, int method);

#endif