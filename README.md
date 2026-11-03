# Simulador de Hierarquia de Memória Cache (3 níveis)

Trabalho Prático 2 da disciplina **BCC 266 – Organização de Computadores** (Prof. Joubert Lima).

O projeto parte da máquina do TP1 e acrescenta **três memórias cache (L1, L2 e L3)** entre a CPU e a RAM. Cada acesso da CPU à memória passa por uma **UCM (Unidade de Controle de Memória)**. A UCM procura o bloco em L1, depois em L2, em L3 e por último na RAM, e leva o bloco encontrado até a cache 1. No fim da execução, o simulador mostra quantos acertos (hits) e faltas (misses) houve em cada nível e o **tempo hipotético** total.

## Funcionamento

| Característica | Implementação |
|---|---|
| Níveis | Cache 1, Cache 2, Cache 3 e RAM |
| Bloco | 4 palavras |
| Mapeamento | Associativo: a busca tenta primeiro a linha `endBloco % tamanho` e depois varre a cache inteira |
| Escrita | Write-back: a escrita fica na cache 1 e só desce para os outros níveis quando o bloco sai dela |
| Substituição | Usa uma linha livre se houver. Se não houver, troca um bloco de custo acumulado alto. Se nenhum servir, usa a linha "direta" (`endBloco % tamanho`) |
| Programa | 10.000 instruções geradas por `instructionsGenerator.c` |

### Custo de cada acesso

| Onde o bloco foi encontrado | Custo |
|---|---|
| Cache 1 | 10 |
| Cache 2 | 110 |
| Cache 3 | 1.110 |
| RAM | 11.110 |

O tempo hipotético é a soma dos custos de todos os acessos.

### Instruções suportadas

Cada linha do arquivo de instruções tem o formato `opcode:b1:p1:b2:p2:b3:p3`, em que `b` é o endereço do bloco e `p` é a palavra dentro do bloco (0 a 3).

| Opcode | Operação |
|---|---|
| 0 | Soma: `mem[b3][p3] = mem[b1][p1] + mem[b2][p2]` |
| 1 | Subtração: `mem[b3][p3] = mem[b1][p1] - mem[b2][p2]` |
| 2 | Registrador → memória. Se `b1` for par, usa o registrador 1; se for ímpar, o registrador 2 |
| 3 | Memória → registrador. Mesma regra de par/ímpar |
| -1 | Fim do programa. O leitor acrescenta essa instrução automaticamente |

## Estrutura

```
.
├── instructionsGenerator.c   # gera tp2/instructions2.txt
└── tp2/
    ├── main.c                # ponto de entrada; define o tamanho das caches
    ├── cpu.c / cpu.h         # ciclo fetch-decode-execute, UCM e estatísticas
    ├── cache.c / cache.h     # caches e movimentação de blocos entre os níveis
    ├── ram.c / ram.h         # RAM (iniciada com valores aleatórios)
    ├── leitor.c / leitor.h   # leitura do arquivo de instruções
    ├── instrucao.h           # structs BlocoMemoria, Endereco e Instrucao
    └── instructions2.txt     # programa de exemplo
```

## Como compilar e executar

Os comandos precisam de `gcc` e começam na raiz do repositório.

**1. (Opcional) Gerar um novo arquivo de instruções.** O gerador grava em `tp2/instructions2.txt`, por isso deve ser executado a partir da raiz:

```bash
gcc instructionsGenerator.c -o gerador
```

```bash
./gerador
```

**2. Compilar e executar o simulador.** O `main` lê `instructions2.txt` da pasta atual, por isso deve ser executado dentro de `tp2/`:

```bash
cd tp2
```

```bash
gcc -Wall *.c -o simulador
```

```bash
./simulador
```

### Exemplo de saída (caches de 16/32/64 blocos)

```
Memoria         Hit       Miss   Taxa hit
Cache 1       11360       3620     75.83%
Cache 2         205       3415      5.66%
Cache 3         262       3153      7.67%
RAM            3153

Tempo hipotetico de execucao: 35456800
```

Os números mudam a cada arquivo de instruções gerado.

### Mudar o tamanho das caches

Os tamanhos (em blocos) ficam em `tp2/main.c`:

```c
iniciarCPU(&cpu, &ram, 16, 32, 64);   // cache 1, cache 2, cache 3
```

### Parâmetros do gerador

Os parâmetros ficam em `#define` no início de `instructionsGenerator.c`:

| Parâmetro | Padrão | Significado |
|---|---|---|
| `N_INST` | 10000 | número de instruções |
| `N_FOR` | 5 | tamanho do grupo de instruções que se repete |
| `N_MEM` | 1000 | quantidade de blocos de memória usados |
| `N_PROB` | 50 | probabilidade (%) de repetir o grupo |

## Resultados

Média de 5 execuções com 50% de repetição de instruções:

| Máquina (cache 1/2/3) | Hit cache 1 | Hit cache 2 | Hit cache 3 | Acessos à RAM | Tempo hipotético |
|---|---|---|---|---|---|
| M1 (4/8/16) | 2.291 | 12.261 | 1.203 | 5.238 | 60,9 mi |
| M2 (16/32/64) | 17.038 | 312 | 325 | 3.319 | 37,4 mi |
| M3 (64/128/256) | 17.290 | 656 | 970 | 2.077 | 24,4 mi |
| M4 (256/512/1024) | 18.245 | 1.541 | 247 | 960 | 11,3 mi |

O que os testes mostraram:

- O tempo cai bastante quando a cache começa a crescer e quase para de cair depois de 16 blocos. A partir desse tamanho, o grupo de instruções repetido cabe na cache.
- Com 16 blocos já se obtém **84% de todo o ganho**. Ir até 512 blocos deixa a cache 32 vezes mais cara para ganhar os 16% restantes.
- Um grupo repetido maior (`N_FOR`) precisa de mais cache para caber. Quando o programa usa muita memória (`N_MEM`), aumentar a cache ajuda pouco.

**Conclusão:** a cache não precisa ser grande, basta caber o que o programa está usando no momento. A análise completa, com gráficos, está no [relatório](relatorio_caches.pdf).

## Autores

- Nome do integrante – [@usuario](https://github.com/usuario)
- Nome do integrante – [@usuario](https://github.com/usuario)

Gerador de instruções fornecido na disciplina (Luísa Calegari, Thiago Cecote e Josué Vila Real).
