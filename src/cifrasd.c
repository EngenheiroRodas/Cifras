#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CESAR 1
#define VIGENER 2

#define TRUE 1
#define FALSE 0

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

const char TABLE[67] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
    'U', 'V', 'W', 'X', 'Y', 'Z', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
    'u', 'v', 'w', 'x', 'y', 'z', 
    ' ', '.', ',', ';', '-' 
};

char *senha;

void help(void) {
    printf("-d: a operação a realizar deve ser decifrar, com o método nn \n");
    printf("-c: a operação a realizar deve ser cifrar, com o método nn \n");
    printf("-s: senha a usar para cifrar/decifrar \n");
    printf("-h: mostra a ajuda para o utilizador  \n");
    printf("-f: filtra o ficheiro de entrada e formata o ficheiro de saída \n");
}

int getNumberOfChar(char caracter) {
	int i;
	for(i= 0; i<67; i++) {
		if (caracter == TABLE[i]) {
			return i;
		}
	}
	return -1;
}

char encode(char caracter, int index) {
	int n = getNumberOfChar(caracter);
	if (0 <= n && n < 67) {
		return TABLE[(n+getNumberOfChar(senha[index%strlen(senha)]))%67];
	} else {
		return caracter;
	}
}

char decode(char caracter, int index) {
	int n = getNumberOfChar(caracter);
	if (0 <= n && n < 67) {
		return TABLE[(n+67-getNumberOfChar(senha[index%strlen(senha)]))%67];
	} else {
		return caracter;
	}
}

void cifra(int tipo, int filtrar) {
	char entrada;
	int contador = 0;

	if (filtrar) {
		int linha_atual = 0;
		int palavra_atual = 0;
		int novaLinha = TRUE;
		while ((scanf("%c", &entrada)==1 ) && entrada != EOF) {
			char caracter ;
			novaLinha = TRUE;
			if (tipo == CESAR) {
				caracter = encode(entrada, 0);
			} else {
				caracter = encode(entrada, contador);
			}
			if (getNumberOfChar(caracter) != -1) {
				contador++;
				if (palavra_atual < 5) {
					palavra_atual++;
					printf("%c", caracter);
				} else {
					palavra_atual = 0;
					printf("%c", caracter);
					if (linha_atual < 7) {
						printf("_");
						linha_atual++;
					} else {
						printf("\n");
						novaLinha = FALSE;
						linha_atual = 0;
					}
				}
			}
		}
		if (novaLinha == TRUE) {
			printf("\n");
		}
	} else {
		while ((scanf("%c", &entrada)==1 ) && entrada != EOF) {
			if (tipo == CESAR) {
				printf("%c", encode(entrada, 0));
			} else if (tipo == VIGENER) {
				char caracter = encode(entrada, contador);
				printf("%c", caracter);
				if (getNumberOfChar(caracter) == -1) {
					contador--;
				}
			}
			contador++;
		}
	}
}

void decifrar(int tipo, int filtrar) {
	char entrada;
	int contador = 0;

	if (filtrar) {
		int linha_atual = 0;
		int palavra_atual = 0;
		int novaLinha = TRUE;
		while ((scanf("%c", &entrada)==1 ) && entrada != EOF) {
			char caracter ;
			novaLinha = TRUE;
			if (tipo == CESAR) {
				caracter = decode(entrada, 0);
			} else {
				caracter = decode(entrada, contador);
			}
			if (getNumberOfChar(caracter) != -1) {
				contador++;
				if (palavra_atual < 5) {
					palavra_atual++;
					printf("%c", caracter);
				} else {
					palavra_atual = 0;
					printf("%c", caracter);
					if (linha_atual < 7) {
						linha_atual++;
					} else {
						printf("\n");
						novaLinha = FALSE;
						linha_atual = 0;
					}
				}
			}
		}
		if (novaLinha == TRUE) {
			printf("\n");
		}
	} else {
		while ((scanf("%c", &entrada)==1 ) && entrada != EOF) {
			if (tipo == CESAR) {
				printf("%c", decode(entrada, 0));
			} else if (tipo == VIGENER) {
				char caracter = decode(entrada, contador);
				printf("%c", caracter);
				if (getNumberOfChar(caracter) == -1) {
					contador--;
				}
			}
			contador++;
		}
	}
}

int main(int argc, char *argv[]) {
    int opt, cifrar, cesar_ou_veg, filtrar;

    char senha_default[] = "Programacao2024";
    senha = senha_default;
	filtrar = FALSE;

    while ((opt = getopt(argc, argv, "d:c:s:hf")) != -1) {
        switch (opt) {
            case 's':
                senha=optarg;
                break;
            case 'c':
				sscanf(optarg, "%d", &cesar_ou_veg);
				cifrar = TRUE;
                break;
            case 'd':
                sscanf(optarg, "%d", &cesar_ou_veg);
				cifrar = FALSE;
                break;
            case 'f':
                filtrar = TRUE;
                break;
            case 'h':
                help();
                return 0;
        }
    }

	if (cifrar == TRUE) {
		cifra(cesar_ou_veg, filtrar);
	} else if (cifrar == FALSE) {
		decifrar(cesar_ou_veg, filtrar);
	}
	
	return 1;
}
