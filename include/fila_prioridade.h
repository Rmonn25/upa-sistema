// ============================================================
// RESPONSÁVEL: PESSOA 2 — Daniel
// MÓDULO: FILA DE PRIORIDADE
// ============================================================
//
// O QUE É UMA FILA?
// Imagine a fila do banco: quem chega primeiro, sai primeiro.
// FILA DE PRIORIDADE: quem tem maior urgência sai primeiro.
// Se dois têm mesma urgência, aí sim quem chegou antes sai.
//
// COMO IMPLEMENTAMOS?
// Usamos uma LISTA ENCADEADA ORDENADA.
//
// O que é lista encadeada?
// Cada nó (nozinho) guarda:
//   - um paciente
//   - um ponteiro para o próximo nó
//
// Visualização:
//
//   INÍCIO
//     │
//     ▼
//   [Prio=5, chegou 10h] → [Prio=4, chegou 09h] → [Prio=3, chegou 11h] → NULL
//
// Cada → é um ponteiro apontando para o próximo.
// NULL significa "não tem próximo" (fim da fila).
// ============================================================

#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

#include "paciente.h"

// ============================================================
// STRUCT: NoPaciente — um "nó" da lista encadeada
// ============================================================
//
// Por que usamos struct dentro de struct?
// O NoPaciente é o "envelope" que carrega o Paciente
// e sabe onde está o próximo envelope.
//
// ATENÇÃO: usamos ponteiro para Paciente (Paciente*)
// e não Paciente direto. Por quê?
// Para não copiar os dados toda vez que movemos o nó.
// O nó só guarda o ENDEREÇO de onde o paciente mora.
// ============================================================

typedef struct NoPaciente {
    Paciente*          paciente;  // Ponteiro para os dados do paciente
    struct NoPaciente* proximo;   // Ponteiro para o próximo nó
                                  // (self-referential struct — aponta pra si mesma!)
} NoPaciente;

// ============================================================
// STRUCT: FilaPrioridade — a fila em si
// ============================================================

typedef struct {
    NoPaciente* inicio;         // Ponteiro para o primeiro da fila (maior prioridade)
    int         tamanho;        // Quantos pacientes estão na fila
    int         contador_novos; // Conta novos pacientes para o mecanismo de Aging
} FilaPrioridade;

// ============================================================
// PROTÓTIPOS
// ============================================================

/**
 * Cria e inicializa uma fila de prioridade vazia.
 * @return FilaPrioridade* — fila alocada e pronta para uso
 */
FilaPrioridade* criarFila(void);

/**
 * Insere um paciente na fila na posição correta (por prioridade/chegada).
 * Também verifica se é hora de aplicar o Aging (a cada 5 novos).
 *
 * @param fila - a fila
 * @param p    - o paciente a inserir (transfere posse do ponteiro para a fila)
 */
void inserirNaFila(FilaPrioridade* fila, Paciente* p);

/**
 * Remove e retorna o paciente com maior prioridade (o primeiro da fila).
 * A fila libera o nó, mas NÃO libera o Paciente* — o chamador fica responsável.
 *
 * @param fila - a fila
 * @return Paciente* removido, ou NULL se a fila estiver vazia
 */
Paciente* removerDaFila(FilaPrioridade* fila);

/**
 * Insere um paciente no INÍCIO da fila com prioridade máxima.
 * Usado pelo mecanismo de DESFAZER.
 *
 * @param fila - a fila
 * @param p    - o paciente a reinserir
 */
void reinserirNoInicio(FilaPrioridade* fila, Paciente* p);

/**
 * Aplica o mecanismo de Aging:
 * Aumenta em +1 a prioridade dos pacientes a partir do 6º da fila.
 * Depois reordena a fila pois as prioridades mudaram.
 *
 * @param fila - a fila
 */
void aplicarAging(FilaPrioridade* fila);

/**
 * Reordena a fila após o Aging (implementação de insertion sort em lista).
 * @param fila - a fila
 */
void reordenarFila(FilaPrioridade* fila);

/**
 * Verifica se a fila está vazia.
 * @return 1 se vazia, 0 se tem pacientes
 */
int filaVazia(const FilaPrioridade* fila);

/**
 * Imprime todos os pacientes da fila, em ordem.
 * @param fila - a fila
 */
void imprimirFila(const FilaPrioridade* fila);

/**
 * Libera toda a memória da fila (nós + pacientes + struct da fila).
 * DEVE ser chamado no final do programa para evitar memory leak!
 *
 * @param fila - a fila a ser destruída
 */
void liberarFila(FilaPrioridade* fila);

#endif  // FILA_PRIORIDADE_H
