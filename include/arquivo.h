// ============================================================
// RESPONSÁVEL: PESSOA 4 — Pedro
// MÓDULO: ARQUIVO — Leitura e processamento de comandos
// ============================================================
//
// PARA QUE SERVE ESTE MÓDULO?
// Em vez de o usuário digitar tudo manualmente, podemos
// ter um arquivo .txt com centenas de comandos e o sistema
// os processa automaticamente.
//
// Exemplo de arquivo de entrada (entrada.txt):
// ---
// ENTRADA,1,Maria Silva,1
// ENTRADA,2,Joao Souza,3
// ENTRADA,3,Ana Lima,2
// ATENDER
// ENTRADA,4,Pedro Costa,5
// DESFAZER
// ---
//
// COMANDOS SUPORTADOS:
//   ENTRADA,<id>,<nome>,<gravidade>  → Adiciona paciente à fila
//   ATENDER                          → Atende o próximo da fila
//   DESFAZER                         → Desfaz o último atendimento
// ============================================================

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "fila_prioridade.h"
#include "pilha.h"

// Tamanho máximo de uma linha do arquivo
#define TAM_LINHA 256

// ============================================================
// PROTÓTIPOS
// ============================================================

/**
 * Lê e processa um arquivo de comandos (.txt).
 *
 * Abre o arquivo, lê linha por linha com fgets(),
 * divide por vírgula com strtok() e chama as funções
 * da fila e pilha conforme o comando.
 *
 * @param caminho_arquivo - caminho para o arquivo .txt
 * @param fila            - fila de prioridade
 * @param pilha           - pilha de atendidos
 * @return 0 se sucesso, -1 se erro ao abrir arquivo
 */
int processarArquivo(const char* caminho_arquivo,
                     FilaPrioridade* fila,
                     Pilha* pilha);

/**
 * Processa uma única linha de comando.
 * Usado internamente por processarArquivo().
 *
 * @param linha  - a linha lida do arquivo (será modificada pelo strtok!)
 * @param fila   - fila de prioridade
 * @param pilha  - pilha de atendidos
 */
void processarLinha(char* linha, FilaPrioridade* fila, Pilha* pilha);

/**
 * Gera um arquivo de teste com N registros aleatórios.
 * Útil para os testes de performance (100, 1000, 10000).
 *
 * @param caminho_arquivo - onde salvar
 * @param n_registros     - quantos comandos gerar
 * @return 0 se sucesso, -1 se erro
 */
int gerarArquivoDeTeste(const char* caminho_arquivo, int n_registros);

#endif  // ARQUIVO_H
