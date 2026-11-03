#include <stdio.h>
#include <stdlib.h>

#include "leitor.h"

Instrucao *lerInstrucoes(const char *nomeArquivo, int *tamanho){

    if(nomeArquivo == NULL || tamanho == NULL){
        printf("Erro: parametros invalidos em lerInstrucoes.\n");
        return NULL;
    }

    *tamanho = 0;

    FILE *arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL){
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", nomeArquivo);
        return NULL;
    }

    int capacidade = 1024;
    int quantidade = 0;
    Instrucao *programa = malloc(capacidade * sizeof(Instrucao));

    if(programa == NULL){
        printf("Erro ao alocar memoria para o programa.\n");
        fclose(arquivo);
        return NULL;
    }

    char linha[256];
    int numLinha = 0;

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        numLinha++;

        int v[7];
        int lidos = sscanf(linha, "%d:%d:%d:%d:%d:%d:%d",
                           &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6]);

        /* ignora linhas em branco */
        if(lidos <= 0)
            continue;

        if(lidos != 7){
            printf("Aviso: linha %d mal formatada, ignorada.\n", numLinha);
            continue;
        }

        /* aumenta o vetor se necessario (+1 reservado para a instrucao de parada) */
        if(quantidade + 1 >= capacidade){
            capacidade *= 2;
            Instrucao *novo = realloc(programa, capacidade * sizeof(Instrucao));
            if(novo == NULL){
                printf("Erro ao realocar memoria para o programa.\n");
                free(programa);
                fclose(arquivo);
                return NULL;
            }
            programa = novo;
        }

        Instrucao inst;
        inst.opcode         = v[0];
        inst.add1.endBloco  = v[1];
        inst.add1.endPalavra = v[2];
        inst.add2.endBloco  = v[3];
        inst.add2.endPalavra = v[4];
        inst.add3.endBloco  = v[5];
        inst.add3.endPalavra = v[6];

        programa[quantidade++] = inst;
    }

    fclose(arquivo);

    if(quantidade == 0){
        printf("Erro: nenhuma instrucao lida de %s.\n", nomeArquivo);
        free(programa);
        return NULL;
    }

    /* instrucao de parada, para o laco da CPU terminar */
    Instrucao halt = {{0, 0}, {0, 0}, {0, 0}, -1};
    programa[quantidade++] = halt;

    *tamanho = quantidade;
    return programa;
}


int maiorBlocoUsado(Instrucao *programa, int tamanho){

    int maior = -1;

    for(int i = 0; i < tamanho; i++){
        if(programa[i].opcode == -1)
            continue;

        if(programa[i].add1.endBloco > maior) maior = programa[i].add1.endBloco;
        if(programa[i].add2.endBloco > maior) maior = programa[i].add2.endBloco;
        if(programa[i].add3.endBloco > maior) maior = programa[i].add3.endBloco;
    }

    return maior;
}


Instrucao *carregarArquivoNaRAM(const char *nomeArquivo, RAM *ram, int *tamanhoPrograma){

    if(ram == NULL || tamanhoPrograma == NULL){
        printf("Erro: parametros invalidos em carregarArquivoNaRAM.\n");
        return NULL;
    }

    Instrucao *programa = lerInstrucoes(nomeArquivo, tamanhoPrograma);
    if(programa == NULL)
        return NULL;

    int tamanhoRAM = maiorBlocoUsado(programa, *tamanhoPrograma) + 1;
    if(tamanhoRAM <= 0)
        tamanhoRAM = 1;

    criarRAM_aleatoria(ram, tamanhoRAM);

    if(ram->memoria == NULL){
        printf("Erro ao criar a RAM.\n");
        free(programa);
        *tamanhoPrograma = 0;
        return NULL;
    }

    return programa;
}
