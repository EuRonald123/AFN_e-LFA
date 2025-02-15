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
    int numSimbolos;
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
                        fechoEpsilon(af->transicoes[k].para, fecho, af);
                        for (int l = 0; l < af->numEstados; l++) {
                            if (fecho[l]) {
                                proximosEstados[l] = 1;
                            }
                        }
                    }
                }
            }
        }
        memcpy(estadosAtuais, proximosEstados, sizeof(estadosAtuais));
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
    char buffer[100];

    printf("Digite o numero de estados: ");
    scanf("%d", &af.numEstados);

    printf("Digite o numero de transições: ");
    scanf("%d", &af.numTransicoes);

    printf("Digite o numero de simbolos: ");
    scanf("%d", &af.numSimbolos);

    printf("Digite o estado inicial: ");
    scanf("%d", &af.estadoInicial);

    printf("Digite os estados finais (termine com -1): ");
    for (int i = 0; i < MAX_ESTADOS; i++) {
        scanf("%d", &af.estadosFinais[i]);
        if (af.estadosFinais[i] == -1) {
            break;
        }
    }

    printf("Digite as transicoes (formato: de simbolo para, termine com -1):\n");
    for (int i = 0; i < af.numTransicoes; i++) {
        int de, para;
        char simbolo;
        scanf("%d %c %d", &de, &simbolo, &para);
        af.transicoes[i] = (Transicao){de, simbolo, para};
    }

    char palavra[100];
    printf("Digite uma palavra: ");
    fgets(palavra, sizeof(palavra), stdin);
    palavra[strcspn(palavra, "\n")] = 0;  // Remove o '\n' do final
    getchar();


    if (ehAceito(&af, palavra)) {
        printf("A palavra eh aceita.\n");
    } else {
        printf("A palavra nao eh aceita.\n");
    }

    getchar();
    getchar();
    return 0;
}