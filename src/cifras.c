#include "cifras.h"

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
 
int main(int argc, char *argv[])
{
    int cflag, fflag; //flags que vão dizer se os comandos foram utilizados
    int method = 0;
    char key[] = "Programacao2024"; // Password default
    int opt;
    fflag = 0;
    cflag = 1;

    while ((opt = getopt(argc, argv, "fc:d:s:")) != -1) { // lê as opções fornecidas através da linha de comando
    
        switch (opt) {
            case 'f':
                fflag = 1;
                break;
            case 'c':
                cflag = 1; //cifrar = 1, logo cifrar
                method = atoi(optarg);
                break;
            case 'd':
                cflag = 0; //cifrar = 0, logo decifrar
                method = atoi(optarg); 
                break;
            case 's':
                strcpy(key, optarg);  //guarda a passe num array chamado key
                break;
            default: /* '?' is returned if neither flag is found*/
                fprintf(stderr, "Use -h for help\n");
                exit(EXIT_FAILURE);
        }
    }

    int key_size = strlen(key); //tamanho da password
    if (key_size < 4){ //se password for menor que 4 dá erro
        fprintf(stderr, "ERROR: your password must be at least 4 characters long.\n");
        exit(EXIT_FAILURE);
    }

    if (method >= 3){ //para a fase 1 o método ou pode ser 1 ou 2
        fprintf(stderr, "ERROR: ciphering method must be either 1 or 2.\n");
        exit(EXIT_FAILURE);
    }

    int *offset_values = offset_calculator(key); // Array para guardar valores do offset

    if (cflag == 1) {
        format_c(key, offset_values, fflag, method);
    } else {
        format_d(key, offset_values, fflag, method);
    }
    
    free(offset_values);
    return 0;
}