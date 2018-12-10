#ifndef LINEAR_H_INCLUDED
#define LINEAR_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#define TAMANHO 256

extern int contador;

typedef struct
{
    char nome[TAMANHO];
    char cpf[TAMANHO];

}Candidato;

const char* getfield(char* line, int num);

Candidato *newCandidato(const char *cpf, const char *name);

int CandidatoCMP(void *a, void *b);

#endif // LINEAR_H_INCLUDED
