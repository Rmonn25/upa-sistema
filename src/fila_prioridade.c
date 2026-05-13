// ============================================================
// RESPONSÁVEL: PESSOA 2 — Daniel
// MÓDULO: FILA DE PRIORIDADE — Implementação
// ============================================================

#include "../include/fila_prioridade.h"

#include <stdio.h>
#include <stdlib.h>

// Referência à variável global do main.c
// extern significa: "essa variável existe em outro .c, use ela"
extern int modo_silencioso;

// ============================================================
// FUNÇÃO AUXILIAR (privada — só usada aqui)
// Cria um novo nó para a lista encadeada
// ============================================================
//
// Por que "static"? O static aqui significa que essa função
// só existe neste arquivo .c. Ela é privada, como um helper
// interno que ninguém de fora precisa saber que existe.
// ============================================================

static NoPaciente* criarNo(Paciente* p) {
    NoPaciente* novo = (NoPaciente*) malloc(sizeof(NoPaciente));
    if (novo == NULL) {
        fprintf(stderr, "ERRO CRÍTICO: sem memória para criar nó da fila\n");
        return NULL;
    }
    novo->paciente = p;
    novo->proximo  = NULL;  // Novo nó não aponta para ninguém ainda
    return novo;
}

// ============================================================
// FUNÇÃO: criarFila
// ============================================================

FilaPrioridade* criarFila(void) {
    FilaPrioridade* fila = (FilaPrioridade*) malloc(sizeof(FilaPrioridade));
    if (fila == NULL) {
        fprintf(stderr, "ERRO CRÍTICO: sem memória para criar a fila\n");
        return NULL;
    }

    fila->inicio         = NULL;  // Fila começa vazia (nenhum nó)
    fila->tamanho        = 0;
    fila->contador_novos = 0;

    return fila;
}

// ============================================================
// FUNÇÃO: inserirNaFila
// ============================================================
//
// COMO FUNCIONA A INSERÇÃO ORDENADA?
//
// Exemplo: fila atual = [Prio=5] → [Prio=4] → [Prio=2] → NULL
// Novo paciente: Prio=3
//
// Percorremos a fila comparando prioridades:
// - [Prio=5] > 3? Sim → continua
// - [Prio=4] > 3? Sim → continua
// - [Prio=2] > 3? NÃO → insere ANTES deste
//
// Resultado: [Prio=5] → [Prio=4] → [Prio=3] → [Prio=2] → NULL
//                                      ↑ novo
//
// O ponteiro "anterior" guarda o nó de antes para
// conseguirmos encadear corretamente.
// ============================================================

void inserirNaFila(FilaPrioridade* fila, Paciente* p) {
    if (fila == NULL || p == NULL) return;

    NoPaciente* novo = criarNo(p);
    if (novo == NULL) return;

    // --- Encontra a posição correta para inserir ---
    NoPaciente* atual    = fila->inicio;
    NoPaciente* anterior = NULL;

    // Avança enquanto o paciente atual DEVE ser atendido antes do novo
    while (atual != NULL && compararPrioridade(atual->paciente, p)) {
        anterior = atual;
        atual    = atual->proximo;
    }

    // --- Realiza a inserção encadeando os ponteiros ---
    novo->proximo = atual;          // Novo aponta para quem estava nesta posição

    if (anterior == NULL) {
        // Inserindo no INÍCIO da fila
        fila->inicio = novo;
    } else {
        // Inserindo no MEIO ou FINAL
        anterior->proximo = novo;   // Anterior aponta para o novo
    }

    fila->tamanho++;
    fila->contador_novos++;

    // --- Verifica Aging: a cada 5 novos pacientes ---
    // O operador % (módulo) retorna o resto da divisão.
    // contador_novos % 5 == 0 significa "é múltiplo de 5"
    if (fila->contador_novos % 5 == 0) {
        if (!modo_silencioso)
            printf("  [AGING] Disparado após %d pacientes. "
                   "Aumentando prioridade a partir do 6º...\n",
                   fila->contador_novos);
        aplicarAging(fila);
    }
}

// ============================================================
// FUNÇÃO: removerDaFila
// ============================================================
//
// Remove o PRIMEIRO da fila (maior prioridade).
//
// Antes: início → [P1] → [P2] → [P3] → NULL
// Depois: início → [P2] → [P3] → NULL
//
// O nó [P1] é liberado com free(),
// mas o Paciente* dentro dele NÃO é liberado aqui —
// ele vai para a Pilha de atendidos.
// ============================================================

Paciente* removerDaFila(FilaPrioridade* fila) {
    if (fila == NULL || fila->inicio == NULL) {
        if (!modo_silencioso) {
            printf("  [AVISO] Fila vazia — nenhum paciente para atender.\n");
        }
        return NULL;
    }

    NoPaciente* no_removido = fila->inicio;         // Salva referência para o 1º nó
    Paciente*   paciente    = no_removido->paciente; // Salva o paciente

    fila->inicio = no_removido->proximo;  // Início agora aponta para o 2º

    // Marca hora do atendimento
    paciente->hora_atendimento = time(NULL);

    free(no_removido);  // Libera APENAS o nó (envelope), não o paciente
    fila->tamanho--;

    return paciente;  // Retorna o paciente para quem chamou
}

// ============================================================
// FUNÇÃO: reinserirNoInicio
// ============================================================
//
// Usada pelo DESFAZER: coloca o paciente de volta
// NO INÍCIO da fila com prioridade MÁXIMA.
// ============================================================

void reinserirNoInicio(FilaPrioridade* fila, Paciente* p) {
    if (fila == NULL || p == NULL) return;

    NoPaciente* novo = criarNo(p);
    if (novo == NULL) return;

    // Força prioridade máxima (requisito do DESFAZER)
    p->prioridade       = PRIORIDADE_MAXIMA;
    p->hora_atendimento = 0;  // Reseta hora de atendimento

    // Insere no início (antes de todos)
    novo->proximo = fila->inicio;
    fila->inicio  = novo;
    fila->tamanho++;

    if (!modo_silencioso) {
        printf("  [DESFAZER] Paciente '%s' reinserido no início com prioridade máxima.\n",
               p->nome);
    }
}

// ============================================================
// FUNÇÃO: aplicarAging
// ============================================================
//
// ANALOGIA: Imagine que você está esperando há muito tempo
// num banco. O gerente vê você esperando e te dá uma senha
// "especial" que faz você subir na fila.
// O Aging faz isso: eleva a prioridade de quem espera muito.
//
// REGRA DO PROFESSOR:
// A cada 5 novos pacientes, aumentar em +1 a prioridade
// de todos os pacientes a partir do 6º da fila.
// ============================================================

void aplicarAging(FilaPrioridade* fila) {
    if (fila == NULL || fila->inicio == NULL) return;

    NoPaciente* atual   = fila->inicio;
    int         posicao = 1;

    while (atual != NULL) {
        if (posicao >= 6) {
            // Aumenta prioridade, mas não passa do máximo
            if (atual->paciente->prioridade < PRIORIDADE_MAXIMA) {
                atual->paciente->prioridade++;
                if (!modo_silencioso) {
                    printf("    → Paciente '%s' (pos %d): prioridade agora %d\n",
                           atual->paciente->nome,
                           posicao,
                           atual->paciente->prioridade);
                }
            }
        }
        posicao++;
        atual = atual->proximo;
    }

    // Após mudar prioridades, a fila pode estar fora de ordem.
    // Precisamos reordenar!
    reordenarFila(fila);
}

// ============================================================
// FUNÇÃO: reordenarFila
// ============================================================
//
// Implementa INSERTION SORT em lista encadeada.
//
// Complexidade: O(n²) — aceitável para listas pequenas.
//
// Como funciona:
// Pegamos cada nó e o reinserimos na posição correta
// numa nova lista auxiliar.
// ============================================================

void reordenarFila(FilaPrioridade* fila) {
    if (fila == NULL) return;
    if (fila->inicio == NULL || fila->inicio->proximo == NULL) return;

    NoPaciente* ordenada = NULL;  // Nova lista ordenada (começa vazia)
    NoPaciente* atual    = fila->inicio;

    while (atual != NULL) {
        NoPaciente* proximo = atual->proximo;  // Salva o próximo ANTES de cortar
        atual->proximo = NULL;                  // Desconecta o nó atual

        // Insere 'atual' na lista 'ordenada' na posição correta
        if (ordenada == NULL || compararPrioridade(atual->paciente, ordenada->paciente)) {
            // Vai para o início da lista ordenada
            atual->proximo = ordenada;
            ordenada       = atual;
        } else {
            // Procura a posição correta na lista ordenada
            NoPaciente* s = ordenada;
            while (s->proximo != NULL &&
                   !compararPrioridade(atual->paciente, s->proximo->paciente)) {
                s = s->proximo;
            }
            atual->proximo = s->proximo;
            s->proximo     = atual;
        }

        atual = proximo;  // Avança para o próximo nó original
    }

    fila->inicio = ordenada;  // Atualiza o início da fila
}

// ============================================================
// FUNÇÃO: filaVazia
// ============================================================

int filaVazia(const FilaPrioridade* fila) {
    return (fila == NULL || fila->inicio == NULL);
}

// ============================================================
// FUNÇÃO: imprimirFila
// ============================================================

void imprimirFila(const FilaPrioridade* fila) {
    if (fila == NULL || fila->inicio == NULL) {
        printf("  [Fila vazia]\n");
        return;
    }

    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║      FILA DE ESPERA — %3d paciente(s)    ║\n", fila->tamanho);
    printf("╚══════════════════════════════════════════╝\n");

    NoPaciente* atual  = fila->inicio;
    int         posicao = 1;

    while (atual != NULL) {
        printf(" Posição #%d:\n", posicao);
        imprimirPaciente(atual->paciente);
        atual = atual->proximo;
        posicao++;
    }
    printf("\n");
}

// ============================================================
// FUNÇÃO: liberarFila
// ============================================================
//
// MUITO IMPORTANTE: libera toda a memória alocada.
//
// Fazemos isso percorrendo a lista nó por nó e liberando:
// 1. O Paciente* dentro do nó
// 2. O NoPaciente* (o próprio nó)
//
// Por fim, liberamos a FilaPrioridade* em si.
//
// CUIDADO: nunca acesse memória após dar free() nela!
// Por isso salvamos 'proximo' antes de liberar 'atual'.
// ============================================================

void liberarFila(FilaPrioridade* fila) {
    if (fila == NULL) return;

    NoPaciente* atual = fila->inicio;

    while (atual != NULL) {
        NoPaciente* proximo = atual->proximo;  // Salva o próximo ANTES do free!

        free(atual->paciente);  // Libera os dados do paciente
        free(atual);            // Libera o nó

        atual = proximo;        // Avança (agora é seguro)
    }

    free(fila);  // Libera a struct da fila
    // Dica: no main.c, após chamar isso, faça: fila = NULL;
    // para não ter ponteiro inválido (dangling pointer)
}
