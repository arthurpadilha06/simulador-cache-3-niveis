#ifndef RAM_H
#define RAM_H

#include "instrucao.h"

typedef struct {
    BlocoMemoria *memoria;  // DEPOIS
    int tamanho;
} RAM;


/* Criacao da RAM */
void criarRAM(RAM *ram, int tamanho);

void criarRAM_aleatoria(RAM *ram, int tamanho);

/* Leitura e escrita */
void setDadoRAM(RAM *ram, int endereco, BlocoMemoria conteudo);

BlocoMemoria getDadoRAM(RAM *ram, int endereco);

/* Utilidades */
void imprimirRAM(RAM *ram);

void liberarRAM(RAM *ram);


#endif