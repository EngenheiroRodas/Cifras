#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cifras.h"

#define TABLE_SIZE 67
#define DEFAULT_SIZE 20 
#define ABSURDLY_LARGE_ERROR 100

const double probabilities [TABLE_SIZE] = 
{ 0.000160079, 0.000554985, 0.000381091, 0.000306662, 0.000263793, 0.000235223, //0-5
  0.000214441, 0.00019842, 0.000185551, 0.000174894, 0.002049944, 0.00037499, //6-11
  0.000699981, 0.00107497, 0.003174913, 0.000549985, 0.000499986, 0.001524958, //12-17
  0.001749952, 3.7499E-05, 0.000192495, 0.000999973, 0.000599984, 0.001674954, //18-23
  0.001874949, 0.000474987, 2.37493E-05, 0.001499959, 0.001574957, 0.002274938, //24-29
  0.000699981, 0.000244993, 0.000599984, 3.7499E-05, 0.000499986, 1.84995E-05, //30-35
  0.061498311, 0.011249691, 0.020999423, 0.032249114, 0.095247384, 0.016499547, //36-41
  0.014999588, 0.045748744, 0.052498558, 0.001124969, 0.005774841, 0.029999176, //42-47
  0.017999506, 0.05024862, 0.056248455, 0.014249609, 0.00071248, 0.044998764, //48-53
  0.047248702, 0.068248126, 0.020999423, 0.007349798, 0.017999506, 0.001124969, //54-59
  0.014999588, 0.000554985, 0.190494768, 0.014249609, 0.014999588, 0.000554985, //60-65
  0.001124969 }; //66


void cesarAttack(FILE *input_stream, int *min_offset, double *min_error)
{
    int chunkSize = 1;
    unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0}; 
    //vetor que vai guardar as estatísticas do ficheiro de input
    double *freq = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize, 1);

    double error[TABLE_SIZE + 1] = {ABSURDLY_LARGE_ERROR};

    for (int j = 0; j < TABLE_SIZE; j++) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            //usam-se as estatísticas para calcular o erro quadrático médio
            error[j] += ((pow((freq[i] - probabilities[(i + j) % TABLE_SIZE]), 2)) / probabilities[(i + j) % TABLE_SIZE]);
        }
    }

    //já não vamos precisar das estísticas
    free(freq);
    
    //cáclulo do erro mínimo e do offset mínimo
    *min_error = error[0];
    for (int i = 1; i < TABLE_SIZE; i++) {
        if (error[i] < *min_error) {
            *min_error = error[i];
            *min_offset = i;
        }
    }
    return;
}

void vigenereAttack(FILE *input_stream, FILE *output_stream, int maxKeySize){
    if (maxKeySize <= DEFAULT_SIZE) {
        maxKeySize = DEFAULT_SIZE; // ver qual será o tamanho máximo da chave
    }

    // Aloca memória para o número de chaves e de erros com diferents chaves que vão existir
    char **key = (char **)malloc(maxKeySize * sizeof(char *)); 
    double **errorArray = (double **) malloc(maxKeySize * sizeof(double *));

    //existirão maxKeysize erros e offsets diferentes, 1 para cada chave
    int *min_offset = (int *) calloc(maxKeySize, sizeof(int));
    double *min_error = (double *) malloc(maxKeySize * sizeof(double));

    //variáveis que ajudarão na determinaçã do erro mínimo e letras da chave correspondente
    int tempMinErrorIndex = 0; //guarda o número de letras que a chave com erro mínimo tem
    double tempMinError = ABSURDLY_LARGE_ERROR; //guarda o erro mínimo encontrado até ao momento
    
    

    //começamos em chunksize 1, correspondendo ao tamanho de chave = 1 e avançamos, até maxKeySize
    for (int chunkSize = 1; chunkSize <= maxKeySize; chunkSize++) {
        for (int i = 0; i < chunkSize; i++){
            errorArray[i] = (double *) calloc(TABLE_SIZE, sizeof(double)); // Aloca 67 espaços de erro para 67 diferentes offsets 
        } 
        
        //Alocação de memória antes de entrar no loop
        key[chunkSize - 1] = malloc((chunkSize + 1) * sizeof(char));  // key terá chunksize + 1, para o '\0'
        min_error[chunkSize - 1] = ABSURDLY_LARGE_ERROR; //inicializa o erro a um número gigante


        /*repete até chunksize, ou seja calcula estatísticas começando no primeiro elemento (i = 0) e saltando de chunksize em chunksize,
        depois começa no segundo elemento (i = 1) saltando de chunksize em chunksize e assim sucessivamente até i < chunkSize*/
        for (int i = 0; i < chunkSize; i++) {
            unsigned int regularChar = 0, weirdChar = 0, temp[TABLE_SIZE] = {0};
            fseek(input_stream, i, SEEK_SET); 
            // Começa do iésimo caracter e calcula estatísticas de chunksize em chunksize
            double *freq = statCalculator(input_stream, &regularChar, &weirdChar, temp, chunkSize, 0);


            for (int offset = 0; offset < TABLE_SIZE; offset++) {
                for (int j = 0; j < TABLE_SIZE; j++) { 
                    //usam-se as estatísticas para calcular o erro quadrático médio
                    errorArray[i][offset] += (pow(freq[j] - probabilities[(j + offset) % TABLE_SIZE], 2)) / probabilities[(j + offset) % TABLE_SIZE];
                }
            }

            //cáclulo do erro mínimo e do offset mínimo
            for (int j = 0; j < TABLE_SIZE; j++) {
                if (errorArray[i][j] < min_error[i]) {
                    min_error[i] = errorArray[i][j];
                    min_offset[i] = j;
                }
            }
            free(freq); // Já não precisamos das frequências
            key[chunkSize - 1][i] = cipher_table[((TABLE_SIZE - min_offset[i]) % TABLE_SIZE)]; // Escrever na chave de tamanho chunksize a(s) letra(s) encontradas com o menor erro
        }
        key[chunkSize - 1][chunkSize] = '\0'; // Terminar a string

        for (int i = 0; i < chunkSize; i++){
            free(errorArray[i]); //liberta porque vai ser alocado outra vez
        } 
            
        
        
        // abre file para mandar o output decifrado com a chave na posição chunksize (começa em 1 e vai avançando) 
        FILE *att_out = fopen("attdraft.txt", "w");
        rewind(input_stream); //rascunho criado para o ataque que é eliminado depois

        // decifra em attdraft.txt
        int attKeySize = strlen(key[chunkSize - 1]);
        int *attOffset = offset_calculator(key[chunkSize - 1], attKeySize);
        filter_d(input_stream, att_out, attOffset, attKeySize, 0, 2);

        fclose(att_out);


        // abre o ficheiro em modo leitura e calcula as suas estatísticas 
        att_out = fopen("attdraft.txt", "r");
        unsigned int regularChar = 0, weirdChar = 0, temp[67] = {0};
        double *attFreq = statCalculator(att_out, &regularChar, &weirdChar, temp, chunkSize, 1);

        double error = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            error += ((pow((attFreq[i] - probabilities[i]), 2)) / probabilities[i]);
        }
        // fim de cálculo das estatísticas

        //acha o menor erro e o tamanho da chave correspondente
        if (error < tempMinError) {
            tempMinError = error; //imprime min se achar min e apenas tab se não achar
            fprintf(output_stream, "min \t");
            tempMinErrorIndex = chunkSize;
        } else {
            fprintf(output_stream, "\t");
        } // imprime o resto necessário
        fprintf(output_stream, "tamanho chave %d: \"%s\" erro %f\n", chunkSize, key[chunkSize - 1], error);
        
        //liberta o que já não é preciso e fecha o ficheiro
        free(attFreq);
        free(attOffset);
        fclose(att_out);
        
    }
    // Por fim deciframos com a passe correspondente ao menor erro quadrático médio entre o ficheiro decifrado 
    //com as passes de diferentes tamanhos e as probabilidades dos caracteres da língua inglesa
    rewind(input_stream);

    int attKeySize = strlen(key[tempMinErrorIndex - 1]);
    int *attOffset = offset_calculator(key[tempMinErrorIndex - 1], attKeySize);
    filter_d(input_stream, output_stream, attOffset, attKeySize, 0, 2); 


    ////////////////////////Limpeza final///////////////////////////
    // remover ficheiro temporário criado e libertar vetor de offset
    remove("attdraft.txt");
    free(attOffset);

    //libertar chave usada até então
    for (int i = 0; i < maxKeySize; i++) {
        free(key[i]);
    }
    free(key);

    //vetores alocados incialmente
    free(errorArray);
    free(min_offset);
    free(min_error);

    return;
}