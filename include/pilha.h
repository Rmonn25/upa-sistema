// ============================================================
// RESPONSÁVEL: PESSOA 3 — Wallacy
// MÓDULO: PILHA DE ATENDIDOS
// ============================================================
//
// O QUE É UMA PILHA?
// Imagine uma pilha de pratos:
// - Você COLOCA (push) um prato por cima.
// - Você REMOVE (pop) o prato de CIMA.
//
// Isso se chama LIFO: Last In, First Out
// (Último a entrar, Primeiro a sair)
//
// POR QUE PILHA AQUI?
// Quando um paciente É ATENDIDO, ele vai para a pilha.
// Se o médico errou e quer DESFAZER, ele pega
// exatamente o ÚLTIMO atendido (o do topo da pilha).
//
// Visualização:
//
//   TOPO
//    │
//    ▼
//   [Paciente C] ← último atendido (será desfeito primeiro)
//   [Paciente B]
//   [Paciente A] ← primeiro atendido
//    NULL
//
// ============================================================

#ifndef PILHA_H
#define PILHA_H

#include "paciente.h"

// ============================================================
// STRUCT: NoPilha — nó da lista da pilha
// ============================================================
//
// Assim como a fila, a pilha também é implementada
// como lista encadeada, mas inserimos e removemos
// sempre pelo INÍCIO (que é o TOPO).
// ============================================================

typedef struct NoPilha {
    Paciente*       paciente;  // Ponteiro para o paciente atendido
    struct NoPilha* abaixo;    // Ponteiro para o nó abaixo na pilha
} NoPilha;

// ============================================================
// STRUCT: Pilha
// ============================================================

typedef struct {
    NoPilha* topo;    // Aponta para o nó mais recente (topo da pilha)
    int      tamanho; // Quantidade de pacientes na pilha
} Pilha;

// ============================================================
// PROTÓTIPOS
// ============================================================

/**
 * Cria e inicializa uma pilha vazia.
 * @return Pilha* alocada e pronta para uso
 */
Pilha* criarPilha(void);

/**
 * PUSH: Empilha um paciente atendido no topo.
 * @param pilha   - a pilha
 * @param paciente - o paciente que acabou de ser atendido
 */
void empilhar(Pilha* pilha, Paciente* paciente);

/**
 * POP: Remove e retorna o paciente do topo da pilha.
 * Usado pelo DESFAZER.
 * O nó é liberado, mas o Paciente* retornado NÃO é liberado —
 * o chamador assume a responsabilidade.
 *
 * @param pilha - a pilha
 * @return Paciente* do topo, ou NULL se pilha vazia
 */
Paciente* desempilhar(Pilha* pilha);

/**
 * Olha o topo sem remover (peek).
 * @param pilha - a pilha
 * @return Paciente* do topo, ou NULL se vazia
 */
Paciente* topoSemRemover(const Pilha* pilha);

/**
 * Verifica se a pilha está vazia.
 * @return 1 se vazia, 0 caso contrário
 */
int pilhaVazia(const Pilha* pilha);

/**
 * Imprime todos os pacientes da pilha (do topo para baixo).
 * @param pilha - a pilha
 */
void imprimirPilha(const Pilha* pilha);

/**
 * Libera toda a memória da pilha (nós + pacientes + struct).
 * DEVE ser chamada no final do programa!
 * @param pilha - a pilha a ser destruída
 */
void liberarPilha(Pilha* pilha);

#endif  // PILHA_H
