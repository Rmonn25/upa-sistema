// ============================================================
// RESPONSÁVEL: PESSOA 5 — João
// MÓDULO: MAIN — Ponto de entrada do sistema
// ============================================================
//
// O main.c é o "maestro" da orquestra:
// - Cria a fila e a pilha
// - Apresenta o menu para o usuário
// - Chama as funções dos outros módulos
// - No final, libera toda a memória
//
// Todos os outros .c fazem trabalho específico;
// o main.c coordena tudo.
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

// Inclui os headers dos nossos módulos
#include "..\include\paciente.h"
#include "..\include\fila_prioridade.h"
#include "..\include\pilha.h"
#include "..\include\arquivo.h"

// Variável global: suprime printf detalhado durante benchmark
int modo_silencioso = 0;

// ============================================================
// CONSTANTES DE CONFIGURAÇÃO
// ============================================================

#define VERSAO_SISTEMA "1.0.0"
#define NOME_SISTEMA   "UPA — Sistema de Gestão de Pacientes"

// Arquivos de teste para benchmark de performance
#define ARQ_TESTE_100    "testes/entrada_100.txt"
#define ARQ_TESTE_1000   "testes/entrada_1000.txt"
#define ARQ_TESTE_10000  "testes/entrada_10000.txt"

// ============================================================
// PROTÓTIPOS DAS FUNÇÕES DO MAIN
// (declaradas aqui pois só existem neste arquivo)
// ============================================================

void exibirCabecalho(void);
void exibirMenu(void);
int  lerOpcao(void);
void menuEntradaManual(FilaPrioridade* fila);
void executarAtendimento(FilaPrioridade* fila, Pilha* pilha);
void executarDesfazer(FilaPrioridade* fila, Pilha* pilha);
void menuCarregarArquivo(FilaPrioridade* fila, Pilha* pilha);
void executarBenchmark(void);
void limparBuffer(void);
double medirTempo(const char* arquivo, int n);

// ============================================================
// FUNÇÃO PRINCIPAL
// ============================================================

int main(void) {

    exibirCabecalho();

    // --- Cria as estruturas de dados ---
    // IMPORTANTE: sempre verificar se a criação foi bem-sucedida
    FilaPrioridade* fila  = criarFila();
    Pilha*          pilha = criarPilha();

    if (fila == NULL || pilha == NULL) {
        fprintf(stderr, "ERRO FATAL: falha ao inicializar estruturas de dados.\n");
        // Libera o que foi alocado antes de sair
        if (fila)  liberarFila(fila);
        if (pilha) liberarPilha(pilha);
        return 1;  // Código de erro ≠ 0 indica falha
    }

    printf("✅ Sistema inicializado com sucesso!\n\n");

    // --- Loop principal do menu ---
    int opcao;
    int executando = 1;

    while (executando) {
        exibirMenu();
        opcao = lerOpcao();

        switch (opcao) {
            case 1:
                menuEntradaManual(fila);
                break;

            case 2:
                executarAtendimento(fila, pilha);
                break;

            case 3:
                executarDesfazer(fila, pilha);
                break;

            case 4:
                imprimirFila(fila);
                break;

            case 5:
                imprimirPilha(pilha);
                break;

            case 6:
                menuCarregarArquivo(fila, pilha);
                break;

            case 7:
                executarBenchmark();
                break;

            case 0:
                executando = 0;
                break;

            default:
                printf("  [AVISO] Opção inválida. Tente novamente.\n");
        }
    }

    // --- Liberação de memória ---
    // OBRIGATÓRIO: liberar tudo antes de sair!
    // O Valgrind vai verificar isso.
    printf("\n🧹 Liberando memória...\n");
    liberarFila(fila);
    fila = NULL;   // Boa prática: NULL após free (evita dangling pointer)

    liberarPilha(pilha);
    pilha = NULL;

    printf("✅ Memória liberada. Encerrando o sistema.\n");
    printf("   Até logo! 🏥\n\n");

    return 0;  // 0 = sucesso
}

// ============================================================
// FUNÇÃO: exibirCabecalho
// ============================================================

void exibirCabecalho(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                                                  ║\n");
    printf("║   🏥  %-44s  ║\n", NOME_SISTEMA);
    printf("║        Triagem Manchester — Estruturas de Dados  ║\n");
    printf("║        Versão%-36s║\n", VERSAO_SISTEMA);  
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
}

// ============================================================
// FUNÇÃO: exibirMenu
// ============================================================

void exibirMenu(void) {
    printf("┌─────────────────────────────────────────┐\n");
    printf("│              MENU PRINCIPAL             │\n");
    printf("├─────────────────────────────────────────┤\n");
    printf("│  1. Adicionar paciente (entrada manual) │\n");
    printf("│  2. Atender próximo paciente            │\n");
    printf("│  3. Desfazer último atendimento         │\n");
    printf("│  4. Visualizar fila de espera           │\n");
    printf("│  5. Visualizar pilha de atendidos       │\n");
    printf("│  6. Carregar arquivo de comandos        │\n");
    printf("│  7. Executar benchmark de performance   │\n");
    printf("│  0. Sair                                │\n");
    printf("└─────────────────────────────────────────┘\n");
    printf("  Opção: ");
}

// ============================================================
// FUNÇÃO: lerOpcao
// ============================================================

int lerOpcao(void) {
    int opcao;
    // scanf retorna o número de itens lidos com sucesso
    if (scanf("%d", &opcao) != 1) {
        opcao = -1;
    }
    limparBuffer();  // Limpa o '\n' que fica no buffer após o Enter
    return opcao;
}

// ============================================================
// FUNÇÃO: limparBuffer
// ============================================================
//
// Por que precisamos disso?
// Após scanf("%d", ...), o '\n' (Enter) fica no buffer de entrada.
// Se chamarmos fgets() depois, ele leria esse '\n' em branco.
// Este loop descarta todos os caracteres até o próximo '\n'.
// ============================================================

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================================
// FUNÇÃO: menuEntradaManual
// ============================================================

void menuEntradaManual(FilaPrioridade* fila) {
    printf("\n─── ADICIONAR PACIENTE ───\n");

    int  id;
    char nome[TAM_NOME];
    int  gravidade;

    printf("  ID do paciente   : ");
    if (scanf("%d", &id) != 1) {
        printf("  [ERRO] ID inválido.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    printf("  Nome completo    : ");
    if (fgets(nome, sizeof(nome), stdin) == NULL) {
        printf("  [ERRO] Erro ao ler nome.\n");
        return;
    }
    // Remove o '\n' que fgets inclui no final
    nome[strcspn(nome, "\n")] = '\0';

    printf("  Gravidade clínica:\n");
    printf("    1 = VERMELHO (Emergência)\n");
    printf("    2 = LARANJA  (Muito urgente)\n");
    printf("    3 = AMARELO  (Urgente)\n");
    printf("    4 = VERDE    (Pouco urgente)\n");
    printf("    5 = AZUL     (Não urgente)\n");
    printf("  Sua escolha      : ");

    if (scanf("%d", &gravidade) != 1 ||
        gravidade < 1 || gravidade > 5) {
        printf("  [ERRO] Gravidade inválida (deve ser 1 a 5).\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    Paciente* p = criarPaciente(id, nome, gravidade);
    if (p != NULL) {
        inserirNaFila(fila, p);
        printf("\n  ✅ Paciente inserido com sucesso!\n");
        imprimirPaciente(p);
    }
}

// ============================================================
// FUNÇÃO: executarAtendimento
// ============================================================

void executarAtendimento(FilaPrioridade* fila, Pilha* pilha) {
    printf("\n─── ATENDER PRÓXIMO PACIENTE ───\n");

    Paciente* p = removerDaFila(fila);

    if (p != NULL) {
        empilhar(pilha, p);
        printf("\n  ✅ Paciente atendido:\n");
        imprimirPaciente(p);
        printf("  Fila atual: %d paciente(s) restante(s)\n",
               filaVazia(fila) ? 0 : fila->tamanho);
    }
}

// ============================================================
// FUNÇÃO: executarDesfazer
// ============================================================

void executarDesfazer(FilaPrioridade* fila, Pilha* pilha) {
    printf("\n─── DESFAZER ÚLTIMO ATENDIMENTO ───\n");

    if (pilhaVazia(pilha)) {
        printf("  [AVISO] Nenhum atendimento para desfazer.\n");
        return;
    }

    // Mostra quem será desfeito antes de executar
    printf("  Desfazendo atendimento de: %s\n",
           topoSemRemover(pilha)->nome);

    Paciente* p = desempilhar(pilha);
    if (p != NULL) {
        reinserirNoInicio(fila, p);
        printf("  ✅ Operação de desfazer concluída.\n");
    }
}

// ============================================================
// FUNÇÃO: menuCarregarArquivo
// ============================================================

void menuCarregarArquivo(FilaPrioridade* fila, Pilha* pilha) {
    printf("\n─── CARREGAR ARQUIVO DE COMANDOS ───\n");
    printf("  Caminho do arquivo (ex: testes/entrada.txt): ");

    char caminho[256];
    if (fgets(caminho, sizeof(caminho), stdin) == NULL) {
        printf("  [ERRO] Erro ao ler caminho.\n");
        return;
    }
    caminho[strcspn(caminho, "\n")] = '\0';  // Remove '\n'

    processarArquivo(caminho, fila, pilha);
}

// ============================================================
// FUNÇÃO: executarBenchmark
// ============================================================
//
// Mede o tempo de execução para 100, 1.000 e 10.000 registros.
// Usa clock() que mede ciclos de CPU.
// CLOCKS_PER_SEC converte para segundos.
// ============================================================

void executarBenchmark(void) {
    printf("\n─── BENCHMARK DE PERFORMANCE ───\n");
    printf("  Gerando arquivos de teste...\n\n");

    // Cria o diretório de testes se não existir
#ifdef _WIN32
    _mkdir("testes");
#else
    mkdir("testes", 0777);
#endif

    int tamanhos[] = {100, 1000, 10000};
    const char* arquivos[] = {
        ARQ_TESTE_100,
        ARQ_TESTE_1000,
        ARQ_TESTE_10000
    };

    // Gera os arquivos de teste
    for (int i = 0; i < 3; i++) {
        gerarArquivoDeTeste(arquivos[i], tamanhos[i]);
    }

    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║          TABELA DE PERFORMANCE               ║\n");
    printf("╠══════════════╦═══════════════╦═══════════════╣\n");
    printf("║  Registros   ║  Tempo (ms)   ║  Tempo (s)    ║\n");
    printf("╠══════════════╬═══════════════╬═══════════════╣\n");

    // Mede o tempo de cada arquivo (com saída suprimida)
    modo_silencioso = 1;
    for (int i = 0; i < 3; i++) {
        double tempo_ms = medirTempo(arquivos[i], tamanhos[i]);
        printf("║  %-12d║  %-13.2f║  %-13.4f║\n",
               tamanhos[i], tempo_ms, tempo_ms / 1000.0);
    }
    modo_silencioso = 0;

    printf("╚══════════════╩═══════════════╩═══════════════╝\n");
    printf("\n  💡 Complexidade esperada: O(n²) para inserção ordenada\n");
    printf("     (aumenta 4x quando o tamanho dobra)\n\n");
}

// ============================================================
// FUNÇÃO: medirTempo
// ============================================================

double medirTempo(const char* arquivo, int n) {
    (void)n;  // Suprime warning de parâmetro não usado (reservado para uso futuro)
    // Cria estruturas temporárias para o teste
    FilaPrioridade* fila  = criarFila();
    Pilha*          pilha = criarPilha();

    if (fila == NULL || pilha == NULL) {
        if (fila)  liberarFila(fila);
        if (pilha) liberarPilha(pilha);
        return -1.0;
    }

    // Mede o tempo com clock()
    clock_t inicio = clock();
    processarArquivo(arquivo, fila, pilha);
    clock_t fim = clock();

    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

    // Libera a memória temporária
    liberarFila(fila);
    liberarPilha(pilha);

    return tempo_ms;
}
