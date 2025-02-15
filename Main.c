#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 5
#define MAX_TRANSICOES 7
#define MAX_SIMBOLOS 3

typedef struct {
    int de;
    char simbolo;
    int para;
} Transicao;

typedef struct {
    int numEstados;
    int numTransicoes;
    int estadoInicial;
    int estadosFinais[MAX_ESTADOS];
    Transicao transicoes[MAX_TRANSICOES];
} AF_e;

void fechoEpsilon(int estado, int fecho[], AF_e *af) {
    fecho[estado] = 1;
    for (int i = 0; i < af->numTransicoes; i++) {
        if (af->transicoes[i].de == estado && af->transicoes[i].simbolo == 'e' && !fecho[af->transicoes[i].para]) {
            fechoEpsilon(af->transicoes[i].para, fecho, af);
        }
    }
}

void imprimirFechoEpsilon(AF_e *af) {
    for (int estado = 0; estado < af->numEstados; estado++) {
        int fecho[MAX_ESTADOS] = {0};
        fechoEpsilon(estado, fecho, af);
        printf("Fecho epsilon do estado %d: ", estado);
        for (int i = 0; i < af->numEstados; i++) {
            if (fecho[i]) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }
}

int ehAceito(AF_e *af, char *palavra) {
    int estadosAtuais[MAX_ESTADOS] = {0};
    int proximosEstados[MAX_ESTADOS] = {0};
    int fecho[MAX_ESTADOS] = {0};

    fechoEpsilon(af->estadoInicial, fecho, af);
    for (int i = 0; i < af->numEstados; i++) {
        if (fecho[i]) {
            estadosAtuais[i] = 1;
        }
    }

    for (int i = 0; palavra[i] != '\0'; i++) {
        char simbolo = palavra[i];
        memset(proximosEstados, 0, sizeof(proximosEstados));
        for (int j = 0; j < af->numEstados; j++) {
            if (estadosAtuais[j]) {
                for (int k = 0; k < af->numTransicoes; k++) {
                    if (af->transicoes[k].de == j && af->transicoes[k].simbolo == simbolo) {
                        int fechoTemp[MAX_ESTADOS] = {0};
                        fechoEpsilon(af->transicoes[k].para, fechoTemp, af);
                        for (int l = 0; l < af->numEstados; l++) {
                            if (fechoTemp[l]) {
                                proximosEstados[l] = 1;
                            }
                        }
                    }
                }
            }
        }
        memcpy(estadosAtuais, proximosEstados, sizeof(estadosAtuais));
        // Recalcular o fecho epsilon para os estados atuais
        memset(fecho, 0, sizeof(fecho));
        for (int j = 0; j < af->numEstados; j++) {
            if (estadosAtuais[j]) {
                fechoEpsilon(j, fecho, af);
            }
        }
        memcpy(estadosAtuais, fecho, sizeof(estadosAtuais));
    }

    for (int i = 0; i < af->numEstados; i++) {
        if (estadosAtuais[i]) {
            for (int j = 0; j < MAX_ESTADOS; j++) {
                if (af->estadosFinais[j] == i) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int main(void) {
    AF_e af;
    FILE *file = fopen("automato.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(file, "%d", &af.numEstados);
    fscanf(file, "%d", &af.estadoInicial);
    fscanf(file, "%d", &af.numTransicoes);

    for (int i = 0; i < MAX_ESTADOS; i++) {
        fscanf(file, "%d", &af.estadosFinais[i]);
        if (af.estadosFinais[i] == -1) {
            break;
        }
    }

    for (int i = 0; i < af.numTransicoes; i++) {
        int de, para;
        char simbolo;
        fscanf(file, "%d %c %d", &de, &simbolo, &para);
        af.transicoes[i] = (Transicao){de, simbolo, para};
    }

    fclose(file);

    imprimirFechoEpsilon(&af);

    char palavra[100];
    printf("Digite uma palavra: ");
    scanf("%s", palavra);

    if (ehAceito(&af, palavra)) {
        printf("A palavra eh aceita.\n");
    } else {
        printf("A palavra nao eh aceita.\n");
    }

    printf("Pressione qualquer tecla para fechar o programa...\n");
    getchar(); // Consumir o '\n' deixado pelo scanf
    getchar(); // Esperar por uma tecla

    return 0;
}