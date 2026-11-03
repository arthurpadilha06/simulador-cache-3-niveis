#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "ram.h"
#include "instrucao.h"
#include "leitor.h"

#define TAM_RAM 1024

int main () {
    RAM ram = {NULL, 0};
    int tamanhoPrograma;

    Instrucao *programa = carregarArquivoNaRAM("instructions2.txt", &ram, &tamanhoPrograma);
    
    if (programa == NULL)
        return 1;   /* erro ao ler o arquivo */

    CPU cpu;
    cpuInicializar(&cpu);
    setPrograma(&cpu, programa, tamanhoPrograma);

    /* tamanhos da cache 1, 2 e 3 */
    iniciarCPU(&cpu, &ram, 16, 32, 64);

    liberarCPU(&cpu);
    free(programa);
    liberarRAM(&ram);

    return 0;
}