// ============================================================
// RESPONSÁVEL: PESSOA 4 — Pedro
// MÓDULO: ARQUIVO — Implementação
// ============================================================

#include "arquivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
extern int modo_silencioso;

// Nomes fictícios para gerar dados de teste realistas
static const char* NOMES_TESTE[] = {
    "Ana Silva", "Bruno Costa", "Carla Souza", "Daniel Lima",
    "Elisa Martins", "Felipe Rocha", "Gabriela Nunes", "Hugo Ferreira",
    "Isabela Carvalho", "Jonas Pereira", "Karina Alves", "Lucas Mendes",
    "Mariana Gomes", "Nicolas Barbosa", "Olivia Ribeiro", "Paulo Teixeira",
    "Quezia Santos", "Rafael Azevedo", "Sabrina Castro", "Thiago Oliveira"
};
static const int N_NOMES = 20;

// ============================================================
// FUNÇÃO: processarArquivo
// ============================================================
//
// COMO LER UM ARQUIVO EM C:
//
// 1. fopen(caminho, modo) — abre o arquivo
//    modo "r" = somente leitura (read)
//    Retorna NULL se o arquivo não existe ou não pode abrir.
//
// 2. fgets(buffer, tamanho, arquivo) — lê uma linha
//    Lê até '\n' ou até tamanho-1 caracteres.
//    Retorna NULL quando chega no fim do arquivo (EOF).
//
// 3. fclose(arquivo) — fecha o arquivo
//    SEMPRE feche! Senão os dados podem não ser salvos
//    e você desperdiça recursos do sistema.
// ============================================================

int processarArquivo(const char* caminho_arquivo,
                     FilaPrioridade* fila,
                     Pilha* pilha) {

    if (caminho_arquivo == NULL || fila == NULL || pilha == NULL) {
        fprintf(stderr, "ERRO: parâmetros inválidos em processarArquivo\n");
        return -1;
    }

    // Abre o arquivo para leitura
    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO: não foi possível abrir o arquivo '%s'\n",
                caminho_arquivo);
        perror("Detalhes do erro");  // Mostra a razão do sistema (ex: "No such file")
        return -1;
    }

    if (!modo_silencioso) {
        printf("\n📂 Processando arquivo: %s\n", caminho_arquivo);
        printf("══════════════════════════════════════════\n");
    }

    char   linha[TAM_LINHA];
    int    numero_linha   = 0;
    int    total_entradas = 0;
    int    total_atend    = 0;
    int    total_desfazer = 0;
    int    total_erros    = 0;

    // fgets retorna NULL ao chegar no fim do arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        numero_linha++;

        // Remove o '\n' do final da linha (se existir)
        // strchr encontra o caractere na string; se achou, substitui por '\0'
        char* newline = strchr(linha, '\n');
        if (newline) *newline = '\0';

        // Remove '\r' (para arquivos criados no Windows)
        char* cr = strchr(linha, '\r');
        if (cr) *cr = '\0';

        // Pula linhas vazias e comentários (começam com #)
        if (linha[0] == '\0' || linha[0] == '#') continue;

        // Conta o tipo de comando para o relatório final
        if (strncmp(linha, "ENTRADA", 7) == 0)  total_entradas++;
        else if (strcmp(linha, "ATENDER")  == 0) total_atend++;
        else if (strcmp(linha, "DESFAZER") == 0) total_desfazer++;
        else                                      total_erros++;

        processarLinha(linha, fila, pilha);
    }

    fclose(arquivo);  // SEMPRE fechar o arquivo!

    if (!modo_silencioso) {
        printf("\n📊 Resumo do arquivo processado:\n");
        printf("   Linhas lidas    : %d\n", numero_linha);
        printf("   Entradas        : %d\n", total_entradas);
        printf("   Atendimentos    : %d\n", total_atend);
        printf("   Desfazimentos   : %d\n", total_desfazer);
        printf("   Linhas inválidas: %d\n", total_erros);
    }

    return 0;
}

// ============================================================
// FUNÇÃO: processarLinha
// ============================================================
//
// COMO FUNCIONA O strtok?
//
// strtok(string, delimitador) divide uma string em pedaços.
// Na PRIMEIRA chamada: passe a string original.
// Nas CHAMADAS SEGUINTES: passe NULL (continua de onde parou).
//
// EXEMPLO:
//   linha = "ENTRADA,1,Maria Silva,2"
//   token1 = strtok(linha, ",") → "ENTRADA"
//   token2 = strtok(NULL, ",")  → "1"
//   token3 = strtok(NULL, ",")  → "Maria Silva"
//   token4 = strtok(NULL, ",")  → "2"
//
// ATENÇÃO: strtok MODIFICA a string original (insere '\0')
// Por isso o parâmetro é char* (não const char*).
// ============================================================

void processarLinha(char* linha, FilaPrioridade* fila, Pilha* pilha) {
    if (linha == NULL || fila == NULL || pilha == NULL) return;

    // Pega o primeiro token (o comando)
    char* comando = strtok(linha, ",");
    if (comando == NULL) return;

    // ---- Comando ENTRADA ----
    if (strcmp(comando, "ENTRADA") == 0) {

        char* str_id       = strtok(NULL, ",");
        char* str_nome     = strtok(NULL, ",");
        char* str_grav     = strtok(NULL, ",");

        // Valida que todos os campos estão presentes
        if (str_id == NULL || str_nome == NULL || str_grav == NULL) {
            fprintf(stderr, "  [AVISO] Linha ENTRADA malformada — pulando.\n");
            return;
        }

        // atoi() converte string para inteiro ("42" → 42)
        int id        = atoi(str_id);
        int gravidade = atoi(str_grav);

        Paciente* p = criarPaciente(id, str_nome, gravidade);
        if (p != NULL) {
            if (!modo_silencioso) printf("  ➕ ENTRADA: %s (ID=%d, %s)\n",
                   p->nome, p->id, nomeCor(p->gravidade));
            inserirNaFila(fila, p);
        }

    // ---- Comando ATENDER ----
    } else if (strcmp(comando, "ATENDER") == 0) {

        Paciente* p = removerDaFila(fila);
        if (p != NULL) {
            if (!modo_silencioso) printf("  ✅ ATENDENDO: %s (ID=%d, Prio=%d)\n",
                   p->nome, p->id, p->prioridade);
            empilhar(pilha, p);
        }

    // ---- Comando DESFAZER ----
    } else if (strcmp(comando, "DESFAZER") == 0) {

        Paciente* p = desempilhar(pilha);
        if (p != NULL) {
            if (!modo_silencioso) printf("  ↩️  DESFAZENDO: %s volta à fila com prioridade máxima\n",
                   p->nome);
            reinserirNoInicio(fila, p);
        }

    // ---- Comando desconhecido ----
    } else {
        fprintf(stderr, "  [AVISO] Comando desconhecido: '%s'\n", comando);
    }
}

// ============================================================
// FUNÇÃO: gerarArquivoDeTeste
// ============================================================
//
// Gera um arquivo .txt com n_registros comandos aleatórios
// para testar performance do sistema.
//
// fprintf(arquivo, ...) = escreve no arquivo (como printf mas para arquivo)
// rand() % N = número aleatório entre 0 e N-1
// srand(time(NULL)) = inicializa o gerador com semente baseada no tempo
// ============================================================

int gerarArquivoDeTeste(const char* caminho_arquivo, int n_registros) {
    FILE* arquivo = fopen(caminho_arquivo, "w");  // "w" = escrita (write)
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO: não foi possível criar '%s'\n", caminho_arquivo);
        return -1;
    }

    srand((unsigned int) time(NULL));  // Semente para números aleatórios

    fprintf(arquivo, "# Arquivo de teste gerado automaticamente\n");
    fprintf(arquivo, "# Total de registros: %d\n\n", n_registros);

    int id_atual = 1;

    for (int i = 0; i < n_registros; i++) {

        // A cada ~4 entradas, faz 1 ATENDER (proporção realista)
        int acao = rand() % 10;

        if (acao < 7) {
            // ENTRADA (70% das vezes)
            const char* nome  = NOMES_TESTE[rand() % N_NOMES];
            int         grav  = (rand() % 5) + 1;  // 1 a 5
            fprintf(arquivo, "ENTRADA,%d,%s,%d\n", id_atual++, nome, grav);

        } else if (acao < 9) {
            // ATENDER (20% das vezes)
            fprintf(arquivo, "ATENDER\n");

        } else {
            // DESFAZER (10% das vezes)
            fprintf(arquivo, "DESFAZER\n");
        }
    }

    fclose(arquivo);  // SEMPRE fechar!

    printf("✅ Arquivo de teste gerado: %s (%d registros)\n",
           caminho_arquivo, n_registros);
    return 0;
}
