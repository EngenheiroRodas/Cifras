#ifndef FUNCTIONSF1_H
#define FUNCTIONSF1_H
#define TABLE_SIZE 67

#include <stdbool.h>

extern char cipher_table[];

typedef struct Trienode
{
    struct Trienode *filho[TABLE_SIZE];
    bool fimpalavra;
} Trienode;

/// @brief Devolve o valor de tabela do input
/// @param input 
/// @return O valor da tabela do caracter de input.
///         Se o valor não estiver na tabela devolve -1
int getIndex(char caracter);



/// @brief Codifica e imprime o caracter de input
/// @param input 
/// @param offset_value 
/// @return Imprime o input codificado, usando encoded = (input + offset) % 67, ou input sem estar modificado, 
///         caso este não pertença à tabela
char encode(char input, int offset_value); 



/// @brief Descodifica e imprime o caracter de input
/// @param input 
/// @param offset_value 
/// @return Imprime o input descodificado, usando decoded = (encoded_char - offset + 67) % 67, ou input sem estar modificado, 
///         caso este não pertença à tabela
char decode(char input, int offset_value);



/// @brief calcular offsets
/// @param key 
/// @param key_size 
/// @return Guarda num vetor offset_values, com o tamanho da chave, os valores da tabela de cada letra da senha, 
///         retornando o apontador para o início desse vetor
int *offset_calculator(char *key, int key_size);



/// @brief A função filter_c escreve diretamente em output_stream o resultado da cifragem, indo buscar os valores de offset a 
///        offset_values e fflag para saber se filtra ou não, e c_method, o método de cifragem
/// @param input_stream 
/// @param output_stream 
/// @param offset_values 
/// @param key_size 
/// @param fflag 
/// @param c_method 
void filter_c(FILE *input_stream, FILE *output_stream, int *offset_values, int key_size, int fflag, int c_method);



/// @brief A função filter_d escreve diretamente em output_stream o resultado da decifragem, indo buscar os valores de offset a 
///        offset_values e fflag para saber se filtra ou não, e c_method, o método de decifragem
/// @param input_stream 
/// @param output_stream 
/// @param offset_values 
/// @param key_size 
/// @param fflag 
/// @param c_method 
void filter_d(FILE *input_stream, FILE *output_stream, int *offset_values, int key_size, int fflag, int c_method);



/// @brief Carrega para a memória a informação dada a partir de input_stream
/// @param input_stream 
/// @param lineCounter 
/// @return char **lines, o qual tem lá alocado dinamicamente a informação de input_stream
char **loadFile(FILE *input_stream, int *lineCounter);



/// @brief Auxilia loadFile, libertando a memória que este aloca
/// @param lines 
/// @param lineCounter 
void freeLines(char *lines[],int *lineCounter);



/// @brief Auxilia statCalculator, removendo todos os caracteres não pertencentes à tabela de cada linha, 
///        evitando o uso de lógica extra para lidar com estes casos
/// @param str 
void removeForeignCharacters(char *str);



/// @brief Calcula estatísticas, saltando de chunkSize em Chunksize.
///        Para calcular estatísticas ou para atacar césar este valor será 1, para atacar viginére este valor vai variar
/// @param input_stream 
/// @param regularChar 
/// @param weirdChar 
/// @param temp 
/// @param chunkSize 
/// @param eflag 
/// @return Dá um vetor com a razão entre o número de vezes do caracter em questão encontrado e o número total de caracteres no ficheiro
double *statCalculator(FILE *input_stream, unsigned int *regularChar, unsigned int *weirdChar, unsigned int *temp, int chunkSize, int eflag);



/// @brief ataca césar, modificando por referência o valor do min_offset e o valor do min_error
/// @param input_stream 
/// @param min_offset 
/// @param min_error 
/// @param eflag 
void cesarAttack(FILE *input_stream, int *min_offset, double *min_error);
//ataca viginere
void vigenereAttack(FILE *input_stream, FILE *output_stream, int nnumber);

void ataquedicionario(FILE *output_stream, Trienode *dicionario, char **input, int lineCounter);

#endif