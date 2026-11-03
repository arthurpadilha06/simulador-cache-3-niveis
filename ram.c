
#include <stdio.h>
#include <stdlib.h>

#include "ram.h"

void criarRAM(RAM *ram, int tamanho){

    if(ram == NULL){
        printf("Erro: ponteiro da RAM invalido.\n");
        return;
    }

    if(tamanho <= 0){
        printf("Erro: tamanho da RAM invalido.\n");
        return;
    }

    BlocoMemoria *novaMemoria = malloc(tamanho * sizeof(BlocoMemoria));

    if(novaMemoria == NULL){
        printf("Erro ao alocar memoria para a RAM.\n");
        return;
    }

    // Inicializa todos os blocos
    for(int i = 0; i < tamanho; i++){

        novaMemoria[i].endBloco = i;
        novaMemoria[i].atualizado = 1;   // 1 = bloco presente nesta memoria (a busca exige 1)
        novaMemoria[i].custo = 0;
        novaMemoria[i].cacheHit = 4;

        for(int j = 0; j < PALAVRAS_POR_BLOCO; j++){
            novaMemoria[i].palavras[j] = 0;
        }
    }

    free(ram->memoria);

    ram->memoria = novaMemoria;
    ram->tamanho = tamanho;
}

void criarRAM_aleatoria(RAM *ram, int tamanho){

    criarRAM(ram, tamanho);

    if(ram == NULL || ram->memoria == NULL)
        return;

    for(int i = 0; i < tamanho; i++){

        for(int j = 0; j < PALAVRAS_POR_BLOCO; j++){
            BlocoMemoria bloco = getDadoRAM(ram, i);
            bloco.palavras[j] = rand();
            setDadoRAM(ram, i, bloco);
        }
    }
}

void setDadoRAM(RAM *ram, int endereco, BlocoMemoria conteudo){

    if(ram == NULL || ram->memoria == NULL){
        printf("Erro: RAM invalida.\n");
        return;
    }

    if(endereco < 0 || endereco >= ram->tamanho){
        printf("Erro: endereco %d fora da RAM.\n", endereco);
        return;
    }

    ram->memoria[endereco] = conteudo;
}

BlocoMemoria getDadoRAM(RAM *ram, int endereco){

    BlocoMemoria blocoInvalido = {0};
    blocoInvalido.endBloco = -1;

    if(ram == NULL || ram->memoria == NULL){
        printf("Erro: RAM invalida.\n");
        return blocoInvalido;
    }

    if(endereco < 0 || endereco >= ram->tamanho){
        printf("Erro: endereco %d fora da RAM.\n", endereco);
        return blocoInvalido;
    }

    return ram->memoria[endereco];
}

void imprimirRAM(RAM *ram){

    if(ram == NULL || ram->memoria == NULL){
        printf("Erro: RAM invalida.\n");
        return;
    }

    printf("Conteudo da RAM:\n");

    for(int i = 0; i < ram->tamanho; i++){

        printf("Bloco %d: ", i);

        for(int j = 0; j < PALAVRAS_POR_BLOCO; j++){
            printf("%d ", getDadoRAM(ram, i).palavras[j]);
        }

        printf("\n");
    }
}

void liberarRAM(RAM *ram){

    if(ram == NULL)
        return;

    free(ram->memoria);

    ram->memoria = NULL;
    ram->tamanho = 0;
}
