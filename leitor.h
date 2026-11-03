#ifndef LEITOR_H
#define LEITOR_H

#include "instrucao.h"
#include "ram.h"

Instrucao *lerInstrucoes(const char *nomeArquivo, int *tamanho);

int maiorBlocoUsado(Instrucao *programa, int tamanho);

Instrucao *carregarArquivoNaRAM(const char *nomeArquivo, RAM *ram, int *tamanhoPrograma);

#endif