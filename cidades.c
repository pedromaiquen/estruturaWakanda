#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

// auxiliar para ordenar cidades por posição ao longo da estrada
int compararCidades(const void *a, const void *b) {
    Cidade *c1 = (Cidade *)a;
    Cidade *c2 = (Cidade *)b;
    return c1->Posicao - c2->Posicao;
}

// Lê os dados do arquivo e inicializa a estrada
// Retorna NULL se alguma restrição for violada

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL; //caso de errado abrir o arquivo

    int T, N;
    // Lê o comprimento da estrada e verifica restrição
    if (fscanf(arquivo, "%d", &T) != 1 || T < 3 || T > 1000000) {
        fclose(arquivo);
        return NULL;
    }

// Lê o número de cidades e verifica restrição

    if (fscanf(arquivo, "%d", &N) != 1 || N < 2 || N > 10000) {
        fclose(arquivo);
        return NULL;
    }

    Estrada *estrada = malloc(sizeof(Estrada)); //Aloca memória para a estrada
    if (!estrada) {
        fclose(arquivo);
        return NULL;
    }

    estrada->T = T;
    estrada->N = N;
    estrada->C = malloc(sizeof(Cidade) * N);
    if (!estrada->C) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }
  
// auxiliar para checar posições duplicadas

    int *posicoes = malloc(sizeof(int) * N);
    if (!posicoes) {
        free(estrada->C);
        free(estrada);
        fclose(arquivo);
        return NULL;
    }
// Lê cada cidade e verifica as restrições de posição
    for (int i = 0; i < N; i++) {
        if (fscanf(arquivo, "%d %255s", &estrada->C[i].Posicao, estrada->C[i].Nome) != 2 ||
            estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= T)  {
            free(posicoes);
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        } // posicao invalida ou erro de leitura

         // Verifica se já existe outra cidade na mesma posição
        for (int j = 0; j < i; j++) {
            if (estrada->C[i].Posicao == posicoes[j]) {
                free(posicoes);
                free(estrada->C);
                free(estrada);
                fclose(arquivo);
                return NULL;
            }
        }
        posicoes[i] = estrada->C[i].Posicao;
    }

    free(posicoes);
    fclose(arquivo);

    // Ordena cidades pela posição ao longo da estrada
    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);
    return estrada;
}

// Calcula a menor vizinhança entre as cidades
// A vizinhança é o trecho da estrada mais próximo daquela cidade do que de qualquer outra

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1; // erro de leitura

    double menor = estrada->T; // inicializa o maximo possivel
    // Itera pelas cidades para calcular a vizinhança de cada uma
    for (int i = 0; i < estrada->N; i++) {
        double inicio, fim;
        if (i == 0) { // Primeira cidade: do início da estrada até o ponto médio com a próxima
            inicio = 0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        } else if (i == estrada->N - 1) { // Última cidade: do ponto médio com a anterior até o final da estrada
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = estrada->T;
        } else {  // Demais cidades: entre os pontos médios com as vizinhas
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        }

        double vizinhanca = fim - inicio;
        if (vizinhanca < menor) {
            menor = vizinhanca;
        }
    }
// Libera memória
    free(estrada->C);
    free(estrada);
    return menor;
}
// Retorna o nome da cidade com a menor vizinhança
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    double menor = estrada->T;
    int indice = -1;

// Calcula a vizinhança de cada cidade e encontra a menor

    for (int i = 0; i < estrada->N; i++) {
        double inicio, fim;
        if (i == 0) {
            inicio = 0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        } else if (i == estrada->N - 1) {
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = estrada->T;
        } else {
            inicio = (estrada->C[i - 1].Posicao + estrada->C[i].Posicao) / 2.0;
            fim = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        }

        double vizinhanca = fim - inicio;
        if (vizinhanca < menor) {
            menor = vizinhanca;
            indice = i;
        }
    }

// Copia o nome da cidade com menor vizinhança (alocação dinâmica)

    char *nome = malloc(strlen(estrada->C[indice].Nome) + 1);
    if (nome) {
        strcpy(nome, estrada->C[indice].Nome);
    }

 // Libera memória

    free(estrada->C);
    free(estrada);
    return nome;
