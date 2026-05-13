// ============================================================
// RESPONSÁVEL: PESSOA 1 — Ramon
// MÓDULO: PACIENTE
// ============================================================
//
// O que é um .h (header)?
// Pensa num cardápio de restaurante: ele lista o que existe,
// mas não explica como o prato é feito. O .h é igual:
// declara as structs e funções, o .c implementa.
//
// Por que usamos #ifndef / #define / #endif?
// Para evitar que o compilador leia este arquivo duas vezes.
// É como uma trava: "se já abri essa porta, não abre de novo."
// ============================================================

#ifndef PACIENTE_H   // Se PACIENTE_H ainda não foi definido...
#define PACIENTE_H   // ...define agora (trava a porta)

#include <time.h>    // Para usar o tipo time_t (data e hora)

// ============================================================
// CONSTANTES — valores fixos do sistema Manchester
// ============================================================

#define PRIORIDADE_MAXIMA  5  // Vermelho: atendimento imediato
#define PRIORIDADE_MINIMA  1  // Azul: não urgente

// Tamanho máximo do nome do paciente
#define TAM_NOME 100

// ============================================================
// ENUM — Gravidade Clínica (Triagem de Manchester)
// ============================================================
//
// Um enum é uma lista nomeada de números inteiros.
// Em vez de lembrar que 1 = vermelho, você escreve VERMELHO.
// Muito mais legível!
//
// Manchester define 5 cores de urgência:
// ============================================================

typedef enum {
    VERMELHO = 1,  // Emergência — risco de vida imediato
    LARANJA  = 2,  // Muito urgente
    AMARELO  = 3,  // Urgente
    VERDE    = 4,  // Pouco urgente
    AZUL     = 5   // Não urgente
} GravidadeManchester;

// ============================================================
// STRUCT — Paciente
// ============================================================
//
// Uma struct é como uma ficha de papel com vários campos.
// Cada paciente tem a SUA ficha com seus dados específicos.
//
// Aqui usamos typedef para criar um "apelido":
// em vez de escrever "struct Paciente", escrevemos só "Paciente".
// ============================================================

typedef struct {
    int    id;                  // Número único do paciente
    char   nome[TAM_NOME];      // Nome completo
    int    gravidade;           // Gravidade clínica (1=vermelho ... 5=azul)
    int    prioridade;          // Grau de prioridade na fila (pode aumentar pelo Aging)
    time_t hora_chegada;        // Quando o paciente chegou (timestamp Unix)
    time_t hora_atendimento;    // Quando começou a ser atendido (0 = ainda não atendido)
} Paciente;

// ============================================================
// PROTÓTIPOS — declarações das funções
// ============================================================
//
// Um protótipo é como avisar: "essa função existe, confie em mim".
// A implementação real fica no paciente.c
// ============================================================

/**
 * Cria um paciente alocando memória dinâmica com malloc.
 *
 * Por que ponteiro? Porque precisamos que esse paciente
 * "exista" além desta função. Sem ponteiro, ele morreria
 * ao sair do escopo da função (memória na pilha de execução).
 *
 * @param id        - Identificador único
 * @param nome      - Nome do paciente
 * @param gravidade - Gravidade clínica (1 a 5)
 * @return Paciente* - ponteiro para o paciente criado, ou NULL se falhou
 */
Paciente* criarPaciente(int id, const char* nome, int gravidade);

/**
 * Imprime todos os dados de um paciente no terminal.
 * @param p - ponteiro para o paciente (não pode ser NULL)
 */
void imprimirPaciente(const Paciente* p);

/**
 * Compara dois pacientes para decidir quem deve ser atendido primeiro.
 *
 * Regras:
 * 1. Maior prioridade vai primeiro.
 * 2. Se mesma prioridade, quem chegou antes vai primeiro.
 *
 * @return  1 se 'a' deve ser atendido antes de 'b'
 *          0 caso contrário
 */
int compararPrioridade(const Paciente* a, const Paciente* b);

/**
 * Retorna o nome da cor Manchester baseado na gravidade.
 * @param gravidade - valor de 1 a 5
 * @return string com o nome da cor
 */
const char* nomeCor(int gravidade);

#endif  // PACIENTE_H — fecha a trava
