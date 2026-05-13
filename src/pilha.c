// ============================================================
// RESPONSÁVEL: PESSOA 3 — Wallacy
// MÓDULO: PILHA DE ATENDIDOS — Implementação
// ============================================================

#include "pilha.h"

#include <stdio.h>
#include <stdlib.h>
extern int modo_silencioso;

// ============================================================
// FUNÇÃO: criarPilha
// ============================================================

Pilha* criarPilha(void) {
    Pilha* pilha = (Pilha*) malloc(sizeof(Pilha));
    if (pilha == NULL) {
        fprintf(stderr, "ERRO CRÍTICO: sem memória para criar a pilha\n");
        return NULL;
    }

    pilha->topo    = NULL;  // Pilha começa vazia
    pilha->tamanho = 0;

    return pilha;
}

// ============================================================
// FUNÇÃO: empilhar (PUSH)
// ============================================================
//
// ANTES:  topo → [B] → [A] → NULL
// DEPOIS: topo → [C] → [B] → [A] → NULL
//                  ↑ novo paciente C inserido no topo
//
// O novo nó aponta para o antigo topo.
// O topo passa a apontar para o novo nó.
// ============================================================

void empilhar(Pilha* pilha, Paciente* paciente) {
    if (pilha == NULL || paciente == NULL) return;

    // Aloca memória para o novo nó
    NoPilha* novo = (NoPilha*) malloc(sizeof(NoPilha));
    if (novo == NULL) {
        fprintf(stderr, "ERRO CRÍTICO: sem memória para empilhar paciente\n");
        return;
    }

    novo->paciente = paciente;     // Novo nó aponta para o paciente
    novo->abaixo   = pilha->topo;  // Novo nó aponta para o antigo topo
    pilha->topo    = novo;         // Topo atualiza para o novo nó
    pilha->tamanho++;

    if (!modo_silencioso) printf("  [PILHA] Paciente '%s' empilhado (topo). Total na pilha: %d\n",
           paciente->nome, pilha->tamanho);
}

// ============================================================
// FUNÇÃO: desempilhar (POP)
// ============================================================
//
// ANTES:  topo → [C] → [B] → [A] → NULL
// DEPOIS: topo → [B] → [A] → NULL    retorna [C]
//
// Salvamos o ponteiro do paciente antes de liberar o nó,
// pois depois do free() não podemos mais acessar o nó!
// ============================================================

Paciente* desempilhar(Pilha* pilha) {
    if (pilha == NULL || pilha->topo == NULL) {
        if (!modo_silencioso) {
            printf("  [AVISO] Pilha vazia — nada para desfazer.\n");
        }
        return NULL;
    }

    NoPilha*  no_topo  = pilha->topo;            // Salva referência do topo
    Paciente* paciente = no_topo->paciente;       // Salva o paciente
    pilha->topo        = no_topo->abaixo;         // Topo sobe para o próximo

    free(no_topo);  // Libera o nó (NÃO o paciente — ele vai voltar para a fila)
    pilha->tamanho--;

    if (!modo_silencioso) printf("  [PILHA] Paciente '%s' desempilhado do topo.\n", paciente->nome);

    return paciente;  // Retorna o paciente para reinserir na fila
}

// ============================================================
// FUNÇÃO: topoSemRemover (PEEK)
// ============================================================
//
// Às vezes precisamos ver quem está no topo sem remover.
// É como olhar o prato de cima sem pegar.
// ============================================================

Paciente* topoSemRemover(const Pilha* pilha) {
    if (pilha == NULL || pilha->topo == NULL) return NULL;
    return pilha->topo->paciente;
}

// ============================================================
// FUNÇÃO: pilhaVazia
// ============================================================

int pilhaVazia(const Pilha* pilha) {
    return (pilha == NULL || pilha->topo == NULL);
}

// ============================================================
// FUNÇÃO: imprimirPilha
// ============================================================

void imprimirPilha(const Pilha* pilha) {
    if (pilha == NULL || pilha->topo == NULL) {
        printf("  [Pilha vazia — nenhum paciente atendido]\n");
        return;
    }

    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║    PILHA DE ATENDIDOS — %3d registro(s)  ║\n", pilha->tamanho);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  (topo = último atendido)                ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    NoPilha* atual   = pilha->topo;
    int      posicao = 1;

    while (atual != NULL) {
        if (posicao == 1) printf(" ▲ TOPO:\n");
        else              printf(" │ Posição %d:\n", posicao);
        imprimirPaciente(atual->paciente);
        atual = atual->abaixo;
        posicao++;
    }
    printf("\n");
}

// ============================================================
// FUNÇÃO: liberarPilha
// ============================================================
//
// Percorre do topo até NULL, liberando cada nó e seu paciente.
// ============================================================

void liberarPilha(Pilha* pilha) {
    if (pilha == NULL) return;

    NoPilha* atual = pilha->topo;

    while (atual != NULL) {
        NoPilha* abaixo = atual->abaixo;  // Salva antes do free!

        free(atual->paciente);  // Libera os dados do paciente
        free(atual);            // Libera o nó

        atual = abaixo;
    }

    free(pilha);  // Libera a struct da pilha
}

