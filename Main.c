#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 500
#define MAX_TRANSICOES 700

typedef struct {
    int de;
    char simbolo;
    int para;
} Transicao;

typedef struct {
    int numEstados;
    int numTransicoes;
    int estadoInicial;
    int numEstadosFinais;
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
        printf("Fecho epsilon do estado %d: {", estado);
        int primeiro = 1;
        for (int i = 0; i < af->numEstados; i++) {
            if (fecho[i]) {
                if (!primeiro) {
                    printf(",");
                }
                printf("%d", i);
                primeiro = 0;
            }
        }
        printf("}\n");
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
    }

    for (int i = 0; i < af->numEstadosFinais; i++) {
        if (estadosAtuais[af->estadosFinais[i]]) {
            return 1;
        }
    }
    return 0;
}

void inserirAutomato(AF_e *af) {
    printf("Digite o numero de estados: ");
    scanf("%d", &af->numEstados);
    printf("Digite o estado inicial: ");
    scanf("%d", &af->estadoInicial);
    printf("Digite o numero de estados finais: ");
    scanf("%d", &af->numEstadosFinais);
    printf("Digite os estados finais: ");
    for (int i = 0; i < af->numEstadosFinais; i++) {
        scanf("%d", &af->estadosFinais[i]);
    }

    printf("Digite o numero de transicoes: ");
    scanf("%d", &af->numTransicoes);
    printf("Digite as transicoes no formato 'de simbolo para':\n");
    for (int i = 0; i < af->numTransicoes; i++) {
        int de, para;
        char simbolo;
        scanf("%d %c %d", &de, &simbolo, &para);
        af->transicoes[i] = (Transicao){de, simbolo, para};
    }
}

int main(void) {
    AF_e af;
    inserirAutomato(&af);

    int rodando = 1;
    while (rodando) {
        printf("\n1-Imprimir fecho epsilon\n2-Verificar palavra\n3-Inserir novo automato\n4-Sair\n");
        int opcao;
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                imprimirFechoEpsilon(&af);
                break;
            case 2: {
                char palavra[100];
                printf("\nDigite uma palavra: ");
                scanf("%s", palavra);
                if (ehAceito(&af, palavra)) {
                    printf("A palavra eh aceita.\n");
                } else {
                    printf("A palavra nao eh aceita.\n");
                }
                break;
            }
            case 3:
                inserirAutomato(&af);
                break;
            case 4:
                rodando = 0;
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
    }
    return 0;
}
