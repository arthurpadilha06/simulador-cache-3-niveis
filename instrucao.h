#ifndef INSTRUCAO_H
#define INSTRUCAO_H

#define PALAVRAS_POR_BLOCO 4

typedef struct {
	int palavras[PALAVRAS_POR_BLOCO];
	int endBloco;    // qual bloco da RAM é este
	int atualizado;  // 1 = modificado, precisa voltar para a RAM
	int custo;       // 10, 110 ou 1110
	int cacheHit;    // 1 = cache1, 2 = cache2, 3 = cache3, 4 = ram;
} BlocoMemoria;

typedef struct {
	int endBloco;
	int endPalavra;
} Endereco;

typedef struct {
	Endereco add1;
	Endereco add2;
	Endereco add3;
	int opcode;
} Instrucao;

#endif