#ifndef FUNCTIONSF1_H
#define FUNCTIONSF1_H

// devolve o valor de tabela do input
int getIndex(char caracter);
// codifica e imprime o caracter de input
void encode(char input, int offset_value); 
// descodifica e imprime o caracter codificado
void decode(char input, int offset_value);

int offset_calculator(char *key);

void format_c(char *key, int *offset_values, int fflag, int method);

void format_d(char *key, int *offset_values, int fflag, int method);

#endif