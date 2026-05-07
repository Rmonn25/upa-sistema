#ifndef PACIENTE_H
#define PACIENTE_H

// Estrutura do paciente
typedef struct {
    int id;
    char nome[100];

    int gravidade;
    int prioridade;

    char horaChegada[6];
    char horaAtendimento[6];

} Paciente;


// Cria um paciente
Paciente criarPaciente(
    int id,
    char nome[],
    int gravidade,
    int prioridade,
    char horaChegada[]
);


// Imprime os dados do paciente
void imprimirPaciente(Paciente p);


// Compara prioridade entre pacientes
int compararPrioridade(Paciente a, Paciente b);

#endif