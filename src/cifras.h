#ifndef FUNCTIONSF1_H
#define FUNCTIONSF1_H

extern char cipher_table[];

// devolve o valor de tabela do input
int getIndex(char caracter);
// codifica e imprime o caracter de input
char encode(char input, int offset_value); 
// descodifica e imprime o caracter codificado
char decode(char input, int offset_value);
// converte as letras da password em números da tabela, retornando esse vetor
int *offset_calculator(char *key, int key_size);
// formata, ou não a saída para a cifra
void filter_c(FILE *input_stream, FILE *output_stream, int *offset_values, int key_size, int fflag, int c_method);
// formata, ou não a saída para a decifra
void filter_d(FILE *input_stream, FILE *output_stream, int *offset_values, int key_size, int fflag, int c_method);

//carregar ficehiro para memória, devolve array com texto
char **loadFile(FILE *input_stream, int *lineCounter);
//escreve os conteúdos processados de lines num ficheiro 
void freeLines(char *lines[],int *lineCounter);
//devolve array com valores das estatísticas
double *statCalculator(char *lines[], int *lineCounter, unsigned int *regularchar, unsigned int *weirdChar, unsigned int *temp);
//ataca cesar
int attack2(FILE *output_stream, double *freq);

#endif