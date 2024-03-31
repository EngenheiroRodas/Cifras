#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cifras.h"

#define TABLE_SIZE 67
#define NEEDED 1
#define NOT_NEEDED 0

void removeForeignCharacters(char *str) {
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (getIndex(str[i]) != -1) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0'; //termina a linha com '\0'
}

double *statCalculator(FILE *input_stream, unsigned int *regularChar, unsigned int *weirdChar, unsigned int *temp, int chunkSize, int eflag) {
    char **lines = NULL;
    int lineCounter = 0;
    lines = loadFile(input_stream, &lineCounter); //carrega para a memória o ficheiro de input_stream, e diz quantas linhas carregou

    //vetor que terá as estatísticas/frequências dos caracteres do ficheiro
    double *frequency = (double *)malloc((TABLE_SIZE + 1) * sizeof(double)); 
    if (frequency == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for character statistics.\n");
        return NULL;
    }

    if (eflag == 1) { //anda de caracter a caracter, calcula as estatísticas/frequências dos caracteres do ficheiro 
        for (int i = 0; i < lineCounter; i++) {
            int j = 0;
            while (lines[i][j] != '\0') {
                if (getIndex(lines[i][j]) != -1) {
                    
                    temp[getIndex(lines[i][j])]++; // aumenta a qunatidade de um certo caracter
                    (*regularChar)++; //aumenta o número total de caracteres da tabela num ficheiro
                } else {
                    //aumenta o número total de caracteres que não constam da tabela num ficheiro
                    (*weirdChar)++;
                } 
                j++; //passa para a letra seguinte
            }
        }
    } else { //usado para o ataque de vigenere
        int bleedOver = 0; // Variável que fica com o valor restante a saltar na próxima linha
        for (int i = 0; i < lineCounter; i++) {
            removeForeignCharacters(lines[i]); //remove todos os caracteres não pertencentes à tabela de cada linha, 
                                               // evitando o uso de lógica extra para lidar com estes casos
            int length = strlen(lines[i]); //número de caracteres da tabela em cada linha
            int j = 0;
            while(j < length) {
                // Aplica o resto do salto, se existir
                j += bleedOver;
                if (j >= length) {
                    bleedOver = bleedOver - length;
                    break; // Se a variável bleedover exceder o número de letras na linha, modifica-se bleedover e passa-se à próxima linha
                }

                // a cada iteração aumenta-se a quantidade de um certo caracter assim como o número total de caracteres
                temp[getIndex(lines[i][j])]++;
                (*regularChar)++;
            
                int remain = length - (j + 1); //remain corresponde ao número de caracteres até ao fim da linha, a contar de j 
                if (remain < chunkSize) { //se a distância até ao fim da linha for inferior ao salto, 
                                          //teremos de calcular quanto deste salto saltaremos na próxima iteração
                    bleedOver = chunkSize - remain - 1; // Quantidade a saltar no próxima iteração
                    break; // Sai do loop, avança para próxima linha
                } else {
                    bleedOver = 0; //se if não se realizar, não precisaremos de bleedOver 
                }

                j += chunkSize; // Aplica o salto.
            }
        }
    }
    freeLines(lines, &lineCounter); //liberta a memória

    // Cálculo de estatísticas
    for (int i = 0; i < TABLE_SIZE; i++) {
        frequency[i] = (*regularChar > 0) ? (double)temp[i] / *regularChar : 0;
    }
    frequency[TABLE_SIZE] = (*weirdChar > 0) ? (double)*weirdChar / (*regularChar + *weirdChar) : 0;

    return frequency;
}