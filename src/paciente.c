#include <stdio.h>
#include <string.h>

#include "../include/paciente.h"


// Cria um paciente
Paciente criarPaciente(
    int id,
    char nome[],
    int gravidade,
    int prioridade,
    char horaChegada[]
) {

    Paciente p;

    p.id = id;

    strcpy(p.nome, nome);

    p.gravidade = gravidade;
    p.prioridade = prioridade;

    strcpy(p.horaChegada, horaChegada);

    // Inicialmente vazio
    strcpy(p.horaAtendimento, "--:--");

    return p;
}


// Imprime paciente
void imprimirPaciente(Paciente p) {

    printf("\n===== PACIENTE =====\n");

    printf("ID: %d\n", p.id);

    printf("Nome: %s\n", p.nome);

    printf("Gravidade: %d\n", p.gravidade);

    printf("Prioridade: %d\n", p.prioridade);

    printf("Hora Chegada: %s\n", p.horaChegada);

    printf("Hora Atendimento: %s\n", p.horaAtendimento);

    printf("====================\n");
}


// Compara prioridade
int compararPrioridade(Paciente a, Paciente b) {

    // Maior prioridade primeiro
    if (a.prioridade > b.prioridade) {
        return 1;
    }

    if (a.prioridade < b.prioridade) {
        return -1;
    }

    // Se empatar prioridade
    // usa ordem de chegada

    if (strcmp(a.horaChegada, b.horaChegada) < 0) {
        return 1;
    }

    if (strcmp(a.horaChegada, b.horaChegada) > 0) {
        return -1;
    }

    return 0;
}