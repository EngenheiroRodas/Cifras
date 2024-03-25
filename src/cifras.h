#ifndef FUNCTIONSF1_H
#define FUNCTIONSF1_H

extern const char cipher_table[];

// devolve o valor de tabela do input
int getIndex(char caracter);
// codifica e imprime o caracter de input
char encode(char input, int offset_value); 
// descodifica e imprime o caracter codificado
char decode(char input, int offset_value);
// converte as letras da password em números da tabela, retornando esse vetor
int *offset_calculator(char *key);
// formata, ou não a saída para a cifra
void filter_c(FILE *input_stream, FILE *output_stream, char *key, int *offset_values, int fflag, int method);
// formata, ou não a saída para a decifra
void filter_d(FILE *input_stream, FILE *output_stream, char *key, int *offset_values, int fflag, int method);

//carregar ficehiro para memória, devolve array com texto
char **loadFile(FILE *input_stream, int *lineCounter);
//escreve os conteúdos processados de lines num ficheiro 
void freeLines(char *lines[],int *lineCounter);
//devolve array com valores das estatísticas
double *statCalculator(char *lines[], int *lineCounter, unsigned int *regularchar, unsigned int *weirdChar);

#endif