// ============================================================
// RESPONSÁVEL: PESSOA 1 — Ramon
// MÓDULO: PACIENTE — Implementação
// ============================================================
//
// Este arquivo IMPLEMENTA as funções declaradas em paciente.h
// Aqui o código realmente faz alguma coisa.
// ============================================================

#include "paciente.h"

#include <stdio.h>    // printf, scanf
#include <stdlib.h>   // malloc, free
#include <string.h>   // strcpy, strncpy
#include <time.h>     // time(), localtime(), strftime()

// ============================================================
// FUNÇÃO: criarPaciente
// ============================================================
//
// ANALOGIA: Imagine uma fábrica de fichas de papel.
// Você chega com os dados do paciente, a fábrica cria
// uma ficha nova e te entrega o endereço dela na memória.
//
// MALLOC: malloc(tamanho) pede ao sistema operacional
// um bloco de memória do tamanho especificado.
// É como reservar um quarto de hotel: o sistema reserva,
// você usa, e VOCÊ É RESPONSÁVEL por dar free() no final
// (fazer o check-out). Se esquecer → memory leak!
// ============================================================

Paciente* criarPaciente(int id, const char* nome, int gravidade) {

    // --- Validação de entrada ---
    // Nunca confie nos dados que chegam! Sempre valide.
    if (nome == NULL) {
        fprintf(stderr, "ERRO: nome do paciente é NULL\n");
        return NULL;
    }
    if (gravidade < PRIORIDADE_MINIMA || gravidade > PRIORIDADE_MAXIMA) {
        fprintf(stderr, "ERRO: gravidade %d inválida (deve ser 1 a 5)\n", gravidade);
        return NULL;
    }

    // --- Alocação dinâmica de memória ---
    // sizeof(Paciente) = quantos bytes a struct precisa
    // malloc retorna void* (ponteiro genérico), convertemos para Paciente*
    Paciente* p = (Paciente*) malloc(sizeof(Paciente));

    // SEMPRE verifique se malloc retornou NULL!
    // Pode falhar se o sistema não tiver memória suficiente.
    if (p == NULL) {
        fprintf(stderr, "ERRO: falha ao alocar memória para paciente\n");
        return NULL;
    }

    // --- Preenchimento dos campos ---
    p->id        = id;
    p->gravidade = gravidade;

    // A prioridade inicial = 6 - gravidade
    // Exemplos:
    //   Vermelho (gravidade=1) → prioridade=5 (máxima)
    //   Azul    (gravidade=5) → prioridade=1 (mínima)
    // Assim: maior prioridade = mais urgente ✓
    p->prioridade = PRIORIDADE_MAXIMA + 1 - gravidade;

    // strncpy é mais segura que strcpy: copia no máximo TAM_NOME-1 caracteres
    // evitando buffer overflow (invasão de memória)
    strncpy(p->nome, nome, TAM_NOME - 1);
    p->nome[TAM_NOME - 1] = '\0';  // Garante terminador nulo

    // time(NULL) retorna o tempo atual em segundos desde 01/01/1970
    // Isso se chama "timestamp Unix"
    p->hora_chegada     = time(NULL);
    p->hora_atendimento = 0;  // 0 = ainda não foi atendido

    return p;  // Retorna o endereço de memória onde o paciente mora
}

// ============================================================
// FUNÇÃO: imprimirPaciente
// ============================================================

void imprimirPaciente(const Paciente* p) {

    // const Paciente* = "prometo não modificar o paciente"
    // Boa prática: sempre use const quando não precisa modificar

    if (p == NULL) {
        printf("  [paciente inválido]\n");
        return;
    }

    // Formatar hora de chegada para string legível
    char buf_chegada[20];
    struct tm* tm_chegada = localtime(&p->hora_chegada);
    strftime(buf_chegada, sizeof(buf_chegada), "%H:%M:%S", tm_chegada);

    printf("  ┌─────────────────────────────────────┐\n");
    printf("  │ ID: %-4d  Nome: %-20s│\n", p->id, p->nome);
    printf("  │ Gravidade: %s (%-7s)      │\n",
           nomeCor(p->gravidade),
           nomeCor(p->gravidade));
    printf("  │ Prioridade: %d  │  Chegada: %s │\n",
           p->prioridade, buf_chegada);

    if (p->hora_atendimento != 0) {
        char buf_atend[20];
        struct tm* tm_atend = localtime(&p->hora_atendimento);
        strftime(buf_atend, sizeof(buf_atend), "%H:%M:%S", tm_atend);
        printf("  │ Atendimento: %s                  │\n", buf_atend);
    }

    printf("  └─────────────────────────────────────┘\n");
}

// ============================================================
// FUNÇÃO: compararPrioridade
// ============================================================
//
// ANALOGIA: É como perguntar "quem deve entrar primeiro
// nessa consulta?" O sistema usa duas regras:
// 1. Quem está mais grave (maior prioridade) entra primeiro.
// 2. Se estão igualmente graves, quem chegou antes entra primeiro.
// ============================================================

int compararPrioridade(const Paciente* a, const Paciente* b) {

    if (a == NULL || b == NULL) return 0;

    // Regra 1: maior prioridade vence
    if (a->prioridade != b->prioridade) {
        return a->prioridade > b->prioridade;  // 1 se a tem mais prioridade
    }

    // Regra 2: mesma prioridade → quem chegou primeiro
    // hora_chegada menor = chegou antes = deve ser atendido antes
    return a->hora_chegada < b->hora_chegada;
}

// ============================================================
// FUNÇÃO: nomeCor
// ============================================================

const char* nomeCor(int gravidade) {
    switch (gravidade) {
        case 1: return "VERMELHO";
        case 2: return "LARANJA ";
        case 3: return "AMARELO ";
        case 4: return "VERDE   ";
        case 5: return "AZUL    ";
        default: return "DESCONHE";
    }
}
